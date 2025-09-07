#include "gpuBuffer.hpp"
#include "utils.hpp"
#include <component.hpp>
#include <shaderType.hpp>
#include <cstring> //For std::memcpy
#include <strings.h>

Renderer::Renderer() 
 : indicesAreFragmented(false), indicesFreeMemoryMaybe(false), vertexBuffer(GL_ARRAY_BUFFER), indexBufferPerShader { {GL_ELEMENT_ARRAY_BUFFER}, {GL_ELEMENT_ARRAY_BUFFER}, {GL_ELEMENT_ARRAY_BUFFER}, {GL_ELEMENT_ARRAY_BUFFER}, {GL_ELEMENT_ARRAY_BUFFER} }
{
    DrawElementsIndirectCommand defaultCommand {0, 1, 0, 0, 0};
    for (int i = 0; i < 5; i++) {
        multiDrawCommands[i].push_back(defaultCommand);
    }
}

void Renderer::addComponent(float componentVertices[], uint32_t numVertexFloats, LogicType logicType) {
    std::vector<uint32_t>& indices = indicesPerShader[shaderType::Ethereal];
    //Get size variables that are needed later
    const uint32_t vertexCount = numVertexFloats / 3;
    const uint32_t firstComponentVertex = vertices.size();
    const uint32_t firstComponentIndex = indices.size();

    bool needMoreMemory = true;
    std::pair<bool, unsigned int> freeMemoryRegion;
    if (indicesFreeMemoryMaybe) { //Maybe there is an existing hole we can fill in
        freeMemoryRegion = getFreeMemoryRegion(freeIndicesPerShader[shaderType::Ethereal], vertexCount);
        needMoreMemory = !(freeMemoryRegion.first);
    }

    vertices.reserve(numVertexFloats + firstComponentVertex); // Reserve in advance to avoid repetitive allocation
    uint32_t indicesToBeAdded[vertexCount];

    for (uint32_t i = 0; i < vertexCount; i++) {
        bool vertexAlreadyExists = false;
        //For each vertex check if its already in the buffer
        for (uint32_t j = 0; j < vertices.size(); j++) {
            if (componentVertices[3*i] == vertices[3*j] && componentVertices[3*i+1] == vertices[3*j+1] && componentVertices[3*i+2] == vertices[3*j+2]) {
                //If its in the buffer point to it with an index
                vertexAlreadyExists = true;
                indicesToBeAdded[i] = j;
            }
        }
        if (!vertexAlreadyExists) {
            //If its not in the buffer add it to the buffer and point to it with an index
            indicesToBeAdded[i] = vertices.size() / 3;
            vertices.push_back(componentVertices[3*i]);
            vertices.push_back(componentVertices[3*i+1]);
            vertices.push_back(componentVertices[3*i+2]);
        }
    }

    if (needMoreMemory) {
        indices.reserve(vertexCount + firstComponentIndex);
        std::memcpy(&indices[firstComponentIndex], &indicesToBeAdded[0], sizeof(indicesToBeAdded));
    }
    else {
        std::vector<DrawElementsIndirectCommand>& drawCommands = multiDrawCommands[shaderType::Ethereal];

        std::memcpy(&indices[freeMemoryRegion.second], &indicesToBeAdded[0], sizeof(indicesToBeAdded));
        //Now we need to merge the draw commands
        DrawElementsIndirectCommand* firstOldCommand;
        DrawElementsIndirectCommand* secondOldCommand;
        bool temp;
        for (DrawElementsIndirectCommand& command : drawCommands) {
            if (command.firstIndex + command.count == freeMemoryRegion.second * sizeof(uint32_t) && command.count != 0) {
                firstOldCommand = &command;
                if (temp) break;
                else temp = true;
            }
            else if (command.firstIndex == (freeMemoryRegion.second  + vertexCount) * sizeof(uint32_t)) {
                secondOldCommand = &command;
                if (temp) break;
                else temp = true;
            }
        }

        (*firstOldCommand).count += (*secondOldCommand).count + vertexCount;

        if (drawCommands.size() < 3)
            drawCommands.pop_back();
        else {
            std::memcpy(secondOldCommand, &drawCommands.back(), sizeof(DrawElementsIndirectCommand));
            drawCommands.pop_back();
        }
        
    }

    //Update the buffers GPU-side
    if (vertexBuffer.getBufferSize() < vertexBuffer.getUsedMemorySize() + numVertexFloats * sizeof(float)) //Need a larger buffer, upload everything
        vertexBuffer.UploadBuffer(&vertices[0], vertices.size() * sizeof(vertices[0]));
    else
        vertexBuffer.AddData(&vertices[firstComponentVertex], numVertexFloats * sizeof(float)); //Upload only vertices to be added

    gpuBuffer& ib = indexBufferPerShader[shaderType::Ethereal];
    if (ib.getBufferSize() < ib.getUsedMemorySize() + numVertexFloats * sizeof(float))
        ib.UploadBuffer(&vertices[0], vertices.size() * sizeof(vertices[0]));
    else
        ib.AddData(&vertices[firstComponentVertex], numVertexFloats * sizeof(float));

    //New component
    //NOTE: reference must be updated when component switches shader, points to index array for specific shader
    components.push_back(Component{false, logicType, &indices, firstComponentIndex, vertexCount, shaderType::Ethereal});
}

void Renderer::removeComponent(uint32_t componentIndex) {
    //Get a reference to the component. index is passed in because we need the index to remove the component itself.
    Component& removedComponent = components[componentIndex];
    std::vector<uint32_t>& indicesReference = *(removedComponent.indicesPointer);

    //Removing unused vertices is O(n^2) so we don't do it everytime we remove an element
    //Easy case first: Are we removing the last element
    if (componentIndex == components.size() - 1) {
        //Since the component is at the end we simply resize the index array to exclude it
        indicesReference.resize(indicesReference.size() - removedComponent.numIndices);
        indexBufferPerShader[removedComponent.shaderID].RemoveData(removedComponent.numIndices * sizeof(uint32_t));
        components.pop_back(); //Also delete the component itself
    }
    else if (components.back().numIndices == removedComponent.numIndices) { 
        //If the last component is the same size, we can simply overwrite and resize
        Component& movedComponent = components.back();
        //Pass in the data to the new location before freeing the memory
        indexBufferPerShader[removedComponent.shaderID].UpdateData(removedComponent.firstIndex * sizeof(uint32_t), &((*movedComponent.indicesPointer)[movedComponent.firstIndex]), movedComponent.numIndices * sizeof(uint32_t));
        indexBufferPerShader[removedComponent.shaderID].RemoveData(removedComponent.numIndices * sizeof(uint32_t)); //Then free GPU memory
        std::memcpy(&(indicesReference[removedComponent.firstIndex]), &(indicesReference[removedComponent.firstIndex]), removedComponent.numIndices * sizeof(uint32_t)); //Copy indices
        indicesReference.resize(indicesReference.size() - removedComponent.numIndices); // Delete old indices
        components[componentIndex] = components.back(); //Overwite
        components.pop_back(); //Delete after copying
    }

    else {
        //Really annoying case, we can't simply delete or overwrite, instead we define a hole
        std::pair freeMemoryPair(removedComponent.firstIndex, removedComponent.numIndices);
        freeIndicesPerShader[removedComponent.shaderID].push_back(freeMemoryPair);
        //Then we need to split the draw command that includes that component in 2
        DrawElementsIndirectCommand* oldCommandPtr = nullptr;
        for (DrawElementsIndirectCommand& command : multiDrawCommands[removedComponent.shaderID]) {
            if (command.firstIndex <= removedComponent.firstIndex && command.firstIndex + command.count >= removedComponent.firstIndex + removedComponent.numIndices) {
                oldCommandPtr = &command;
                break;
            }
        }

        //First command can overwrite the one we found, second one will be appended
        uint32_t oldCount = (*oldCommandPtr).count; //Store, needed for second command
        (*oldCommandPtr).count = removedComponent.firstIndex - (*oldCommandPtr).firstIndex; //Simply change the count, location is fine
        //second command: count needs to be the old count minus this components end (so it is everything after). Location is the index right after the removed component
        DrawElementsIndirectCommand newCommand {oldCount - removedComponent.firstIndex - removedComponent.numIndices, 1, removedComponent.firstIndex + removedComponent.numIndices, 0, 0};

        multiDrawCommands[removedComponent.shaderID].push_back(newCommand);
    }
}
