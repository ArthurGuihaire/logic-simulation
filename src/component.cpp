#include "gpuBuffer.hpp"
#include "shaderType.hpp"
#include <utils.hpp>
#include <component.hpp>
#include <cstring> //For std::memcpy

ComponentSystem::ComponentSystem() 
 : indicesFreeMemoryMaybe(false), vertexBuffer(GL_ARRAY_BUFFER)
{
    DrawElementsIndirectCommand defaultCommand {0, 1, 0, 0, 0};
    glGenVertexArrays(numShaders, &vertexArrayObject[0]);
    for (uint32_t i = 0; i < numShaders; i++) {
        multiDrawCommands[i].push_back(defaultCommand);
        glBindVertexArray(vertexArrayObject[i]);
        indexBufferPerShader[i].createBuffer(GL_ELEMENT_ARRAY_BUFFER);
        commandsBufferPerShader[i].createBuffer(GL_DRAW_INDIRECT_BUFFER, &multiDrawCommands[i][0], sizeof(DrawElementsIndirectCommand));
    }
}

void ComponentSystem::addComponent(float* componentVertices, uint32_t numVertexFloats, LogicType logicType) { //Pass array by pointer
    std::vector<uint32_t>& indices = indicesPerShader[shaderType::Ethereal];
    //Get size variables that are needed later
    const uint32_t vertexCount = numVertexFloats / 3;
    const uint32_t firstComponentVertex = vertices.size();
    uint32_t firstComponentIndex;

    bool needMoreMemory = true;
    std::pair<bool, unsigned int> freeMemoryRegion;
    if (indicesFreeMemoryMaybe) { //Maybe there is an existing hole we can fill in
        freeMemoryRegion = getFreeMemoryRegion(freeIndicesPerShader[shaderType::Ethereal], vertexCount);
        needMoreMemory = !(freeMemoryRegion.first);
    }

    vertices.reserve(numVertexFloats + firstComponentVertex); // Reserve in advance to avoid repetitive allocation
    uint32_t indicesToBeAdded[vertexCount];
    uint32_t numVerticesAdded = 0;

    for (uint32_t i = 0; i < vertexCount; i++) {
        bool vertexAlreadyExists = false;
        //For each vertex check if its already in the buffer
        for (uint32_t j = 0; j < firstComponentVertex / 3; j++) { //Check all the existing vertices
            if (almostEqual(componentVertices[3*i], vertices[3*j]) && almostEqual(componentVertices[3*i+1], vertices[3*j+1]) && almostEqual(componentVertices[3*i+2], vertices[3*j+2])) {
                //If its in the buffer point to it with an index
                vertexAlreadyExists = true;
                indicesToBeAdded[i] = j;
                break;
            }
        }
        if (!vertexAlreadyExists) {
            //If its not in the buffer add it to the buffer and point to it with an index
            indicesToBeAdded[i] = vertices.size() / 3;
            vertices.push_back(componentVertices[3*i]);
            vertices.push_back(componentVertices[3*i+1]);
            vertices.push_back(componentVertices[3*i+2]);
            numVerticesAdded++;
        }
    }

    if (needMoreMemory) {
        firstComponentIndex = indices.size();
        indices.resize(firstComponentIndex + vertexCount);
        std::memcpy(&indices[firstComponentIndex], &indicesToBeAdded[0], sizeof(indicesToBeAdded));
        //We need to update the command to include the new indices
        DrawElementsIndirectCommand* oldCommandPtr = findLastCommand(multiDrawCommands[shaderType::Ethereal], firstComponentIndex);
        oldCommandPtr->count += vertexCount;
        const uint32_t offset = (oldCommandPtr - &multiDrawCommands[shaderType::Ethereal][0]) * sizeof(DrawElementsIndirectCommand);
        commandsBufferPerShader[shaderType::Ethereal].updateData(offset, oldCommandPtr, sizeof(DrawElementsIndirectCommand));
    }
    else {
        firstComponentIndex = freeMemoryRegion.second;
        std::vector<DrawElementsIndirectCommand>& drawCommands = multiDrawCommands[shaderType::Ethereal];
        gpuBuffer& commandBuffer = commandsBufferPerShader[shaderType::Ethereal];

        std::memcpy(&indices[freeMemoryRegion.second], &indicesToBeAdded[0], sizeof(indicesToBeAdded));
        //Now we need to merge the draw commands
        const std::pair edgeCommands = findEdgeCommands(drawCommands, freeMemoryRegion.second, vertexCount);

        edgeCommands.first->count += edgeCommands.second->count + vertexCount; //Expand the first command over the memory region
        uint32_t bufferOffset = (edgeCommands.first - &drawCommands[0]) * sizeof(DrawElementsIndirectCommand);
        commandBuffer.updateData(bufferOffset, edgeCommands.first, sizeof(DrawElementsIndirectCommand));
        
        //Delete the second command
        if (drawCommands.size() < 3) { //Its the last element, we can safely delete
            drawCommands.pop_back();
            commandBuffer.removeData(sizeof(DrawElementsIndirectCommand));
        }
        else { //Its somewhere in the middle, memcpy the last command over it, then delete the last command
            std::memcpy(edgeCommands.second, &drawCommands.back(), sizeof(DrawElementsIndirectCommand));
            drawCommands.pop_back();
            bufferOffset = (edgeCommands.second - &drawCommands[0]) * sizeof(DrawElementsIndirectCommand);
            commandBuffer.updateData(bufferOffset, edgeCommands.second, sizeof(DrawElementsIndirectCommand));
            commandBuffer.removeData(sizeof(DrawElementsIndirectCommand));
        }
        //If there is only one draw command, guaranteed no more free memory
        if (drawCommands.size() == 1)
            indicesFreeMemoryMaybe = false;
    }

    //Update the buffers GPU-side
    if (numVerticesAdded != 0) {
        if (vertexBuffer.getBufferSize() < vertexBuffer.getUsedMemorySize() + numVerticesAdded * 3 * sizeof(float)) //Need a larger buffer, upload everything
            vertexBuffer.uploadBuffer(&vertices[0], vertices.size() * sizeof(vertices[0]));
        else
            vertexBuffer.addData(&vertices[firstComponentVertex], numVerticesAdded * 3 * sizeof(float)); //Upload only vertices to be added
    }

    gpuBuffer& ib = indexBufferPerShader[shaderType::Ethereal];
    if (ib.getBufferSize() < ib.getUsedMemorySize() + numVertexFloats * sizeof(float))
        ib.uploadBuffer(&indices[0], indices.size() * sizeof(indices[0]));
    else
        ib.addData(&indices[firstComponentIndex], vertexCount * sizeof(uint32_t));

    //New component
    //NOTE: reference must be updated when component switches shader, points to index array for specific shader
    components.push_back(Component{false, logicType, &indices, firstComponentIndex, vertexCount, shaderType::Ethereal});
}

void ComponentSystem::removeComponent(Component& removedComponent) {
    //Get a reference to the component. index is passed in because we need the index to remove the component itself.
    std::vector<uint32_t>& indices = *(removedComponent.indicesPointer);
    const uint32_t componentIndex = &removedComponent - &components[0];

    //Removing unused vertices is O(n^2) so we don't do it everytime we remove an element
    //Easy case first: Are we removing the last element
    if (componentIndex == components.size() - 1) {
        //Since the component is at the end we simply resize the index array to exclude it
        DrawElementsIndirectCommand* command = findLastCommand(multiDrawCommands[removedComponent.shaderID], indices.size());
        command->count -= removedComponent.numIndices;
        const uint32_t bufferOffset = (command - &multiDrawCommands[removedComponent.shaderID][0]) * sizeof(DrawElementsIndirectCommand);
        commandsBufferPerShader[shaderType::Ethereal].updateData(bufferOffset, command, sizeof(DrawElementsIndirectCommand));
        
        indices.resize(indices.size() - removedComponent.numIndices);
        indexBufferPerShader[removedComponent.shaderID].removeData(removedComponent.numIndices * sizeof(uint32_t));
        components.pop_back(); //Also delete the component itself
    }
    else if (components.back().numIndices == removedComponent.numIndices) { 
        //If the last component is the same size, we can simply overwrite and resize
        Component& movedComponent = components.back();

        //Find correct command to resize before memory shenanigans
        DrawElementsIndirectCommand* command = findLastCommand(multiDrawCommands[movedComponent.shaderID], movedComponent.firstIndex + movedComponent.numIndices);
        command->count -= movedComponent.numIndices;
        const uint32_t bufferOffset = (command - &multiDrawCommands[removedComponent.shaderID][0]) * sizeof(DrawElementsIndirectCommand);
        commandsBufferPerShader[removedComponent.shaderID].updateData(bufferOffset, command, sizeof(DrawElementsIndirectCommand));
        //Pass in the data to the new location before freeing the memory
        indexBufferPerShader[removedComponent.shaderID].updateData(removedComponent.firstIndex * sizeof(uint32_t), &((*movedComponent.indicesPointer)[movedComponent.firstIndex]), movedComponent.numIndices * sizeof(uint32_t));
        indexBufferPerShader[removedComponent.shaderID].removeData(removedComponent.numIndices * sizeof(uint32_t)); //Then free GPU memory
        std::memcpy(&(indices[removedComponent.firstIndex]), &(indices[movedComponent.firstIndex]), removedComponent.numIndices * sizeof(uint32_t)); //Copy indices
        movedComponent.firstIndex = removedComponent.firstIndex; //Update so it matches the copied memory
        indices.resize(indices.size() - removedComponent.numIndices); // Delete old indices
        components[componentIndex] = components.back(); //Overwite
        components.pop_back(); //Delete after copying
    }

    else {
        //Really annoying case, we can't simply delete or overwrite, instead we define a hole
        indicesFreeMemoryMaybe = true;
        std::pair freeMemoryPair(removedComponent.firstIndex, removedComponent.numIndices);
        freeIndicesPerShader[removedComponent.shaderID].push_back(freeMemoryPair);
        //Then we need to split the draw command that includes that component in 2
        DrawElementsIndirectCommand* oldCommandPtr = findContainingCommand(multiDrawCommands[removedComponent.shaderID], removedComponent.firstIndex, removedComponent.numIndices);
        //First command can overwrite the one we found, second one will be appended
        const uint32_t oldCount = oldCommandPtr->count; //Store, needed for second command
        oldCommandPtr->count = removedComponent.firstIndex - oldCommandPtr->firstIndex; //Simply change the count, location is fine
        const uint32_t bufferOffset = (oldCommandPtr - &multiDrawCommands[removedComponent.shaderID][0]) * sizeof(DrawElementsIndirectCommand);
        gpuBuffer& commandBuffer = commandsBufferPerShader[removedComponent.shaderID];
        commandBuffer.updateData(bufferOffset, oldCommandPtr, sizeof(DrawElementsIndirectCommand));
        //second command: count needs to be the old count minus this components end (so it is everything after). Location is the index right after the removed component
        DrawElementsIndirectCommand newCommand {oldCount - removedComponent.firstIndex - removedComponent.numIndices, 1, removedComponent.firstIndex + removedComponent.numIndices, 0, 0};
        multiDrawCommands[removedComponent.shaderID].push_back(newCommand);
        if (commandBuffer.getBufferSize() < commandBuffer.getUsedMemorySize() + sizeof(DrawElementsIndirectCommand))
            commandBuffer.uploadBuffer(&multiDrawCommands[removedComponent.shaderID], multiDrawCommands[removedComponent.shaderID].size() * sizeof(DrawElementsIndirectCommand));
        else
            commandBuffer.addData(&newCommand, sizeof(DrawElementsIndirectCommand));
    }
}

void ComponentSystem::moveComponent(Component& movedComponent, shaderType newShader) {
    
}