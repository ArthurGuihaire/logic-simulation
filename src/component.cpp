#include <gpuBuffer.hpp>
#include <shaderType.hpp>
#include <utils.hpp>
#include <component.hpp>
#include <cstring> //For std::memcpy
#include <arrayUtils.hpp>

/*
 * Tiny optimization:
 * Have a single vector for components and don't copy components on moveComponents instead just copy the indices and update the correct component
*/

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

void ComponentSystem::createComponent(float* componentVertices, uint32_t numVertexFloats, LogicType logicType) { //Pass array by pointer
    std::vector<uint32_t>& indices = indicesPerShader[shaderType::Ethereal];
    //Get size variables that are needed later
    const uint32_t vertexCount = numVertexFloats / 3;
    const uint32_t firstComponentVertex = vertices.size();

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

    const uint32_t firstComponentIndex = addComponent(&indicesToBeAdded[0], vertexCount, shaderType::Ethereal);

    //Update the buffers GPU-side
    if (numVerticesAdded != 0) {
        if (vertexBuffer.getBufferSize() < vertexBuffer.getUsedMemorySize() + numVerticesAdded * 3 * sizeof(float)) //Need a larger buffer, upload everything
            vertexBuffer.uploadBuffer(&vertices[0], vertices.size() * sizeof(vertices[0]));
        else
            vertexBuffer.addData(&vertices[firstComponentVertex], numVerticesAdded * 3 * sizeof(float)); //Upload only vertices to be added
    }

    //New component
    //NOTE: reference must be updated when component switches shader, points to index array for specific shader
    componentsPerShader[shaderType::Ethereal].push_back({false, logicType, &indices, firstComponentIndex, vertexCount, shaderType::Ethereal});
}

uint32_t ComponentSystem::addComponent(uint32_t* newIndices, uint32_t numIndices, shaderType shaderID) {
    std::vector<uint32_t>& indices = indicesPerShader[shaderID];
    bool needMoreMemory = true;
    std::pair<bool, unsigned int> freeMemoryRegion;
    uint32_t firstComponentIndex;
    gpuBuffer& ib = indexBufferPerShader[shaderID];

    if (indicesFreeMemoryMaybe) { //Maybe there is an existing hole we can fill in
        freeMemoryRegion = getFreeMemoryRegion(freeIndicesPerShader[shaderID], numIndices);
        needMoreMemory = !(freeMemoryRegion.first);
    }

    if (needMoreMemory) {
        firstComponentIndex = indices.size();
        indices.resize(firstComponentIndex + numIndices);
        std::memcpy(&indices[firstComponentIndex], newIndices, numIndices * sizeof(uint32_t));
        //We need to update the command to include the new indices
        DrawElementsIndirectCommand* oldCommandPtr = findLastCommand(multiDrawCommands[shaderID], firstComponentIndex);
        oldCommandPtr->count += numIndices;
        const uint32_t offset = (oldCommandPtr - &multiDrawCommands[shaderID][0]) * sizeof(DrawElementsIndirectCommand);
        commandsBufferPerShader[shaderID].updateData(offset, oldCommandPtr, sizeof(DrawElementsIndirectCommand));
    }
    else {
        firstComponentIndex = freeMemoryRegion.second;
        std::vector<DrawElementsIndirectCommand>& drawCommands = multiDrawCommands[shaderID];
        gpuBuffer& commandBuffer = commandsBufferPerShader[shaderID];

        std::memcpy(&indices[freeMemoryRegion.second], newIndices, numIndices * sizeof(uint32_t));
        ib.updateData(freeMemoryRegion.first * sizeof(uint32_t), newIndices, numIndices * sizeof(uint32_t));
        //Now we need to merge the draw commands
        const std::pair edgeCommands = findEdgeCommands(drawCommands, freeMemoryRegion.second, numIndices);

        edgeCommands.first->count += edgeCommands.second->count + numIndices; //Expand the first command over the memory region
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

    if (needMoreMemory){
        if (ib.getBufferSize() < ib.getUsedMemorySize() + numIndices * sizeof(float))
            ib.uploadBuffer(&indices[0], indices.size() * sizeof(indices[0]));
        else
            ib.addData(&indices[firstComponentIndex], numIndices * sizeof(uint32_t));
    }
    return firstComponentIndex;
}

void ComponentSystem::removeComponent(Component& removedComponent) {
    //Get a reference to the component. index is passed in because we need the index to remove the component itself.
    std::vector<uint32_t>& indices = *(removedComponent.indicesPointer);
    std::vector<Component>& components = componentsPerShader[removedComponent.shaderID];

    //Removing unused vertices is O(n^2) so we don't do it everytime we remove an element
    //Easy case first: Are we removing the last element
    if (removedComponent.firstIndex + removedComponent.numIndices == indices.size()) {
        //Since the component is at the end we simply resize the index array to exclude it
        DrawElementsIndirectCommand* command = findLastCommand(multiDrawCommands[removedComponent.shaderID], indices.size());
        command->count -= removedComponent.numIndices;
        const uint32_t bufferOffset = (command - &multiDrawCommands[removedComponent.shaderID][0]) * sizeof(DrawElementsIndirectCommand);
        commandsBufferPerShader[removedComponent.shaderID].updateData(bufferOffset, command, sizeof(DrawElementsIndirectCommand));
        
        indices.resize(indices.size() - removedComponent.numIndices);
        indexBufferPerShader[removedComponent.shaderID].removeData(removedComponent.numIndices * sizeof(uint32_t));
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
        //Update GPU buffer, since we're adding a command check capacity of the buffer
        if (commandBuffer.getBufferSize() < commandBuffer.getUsedMemorySize() + sizeof(DrawElementsIndirectCommand))
            commandBuffer.uploadBuffer(&multiDrawCommands[removedComponent.shaderID], multiDrawCommands[removedComponent.shaderID].size() * sizeof(DrawElementsIndirectCommand));
        else
            commandBuffer.addData(&newCommand, sizeof(DrawElementsIndirectCommand));
    }

    const uint32_t componentIndex = &removedComponent - &components[0];
    if (componentIndex == components.size())
        components.pop_back();
    else {
        components[componentIndex] = components.back();
        components.pop_back();
    }
}

void ComponentSystem::moveComponent(Component& component, shaderType newShader) {
    //STEP 1: add the component to the new list
    const uint32_t firstComponentIndex = addComponent(&(*(component.indicesPointer))[component.firstIndex], component.numIndices, newShader);
    componentsPerShader[newShader].push_back(component);
    //STEP 2: remove the component from the old list
    removeComponent(component);
    //old reference is invalid (overwritten in removeComponent) so new reference:
    Component& newComponent = componentsPerShader[newShader].back();
    newComponent.indicesPointer = &indicesPerShader[newShader];
    newComponent.firstIndex = firstComponentIndex;
}
