#include "componentStructs.hpp"
#include "gl.h"
#include <cstdint>
#include <gpuBuffer.hpp>
#include <shaderType.hpp>
#include <sys/types.h>
#include <utils.hpp>
#include <cstring> //For std::memcpy
#include <arrayUtils.hpp>
#include <constants.hpp>

/*
 * Tiny optimization:
 * Have a single vector for components and don't copy components on moveComponents instead just copy the indices and update the correct component
*/

ComponentSystem::ComponentSystem(Renderer& renderer) 
 : indicesFreeMemoryMaybe(false), vertexBuffer(GL_ARRAY_BUFFER)
{
    uint32_t vertexArrayObject[numShaders];
    glGenVertexArrays(numShaders, &vertexArrayObject[0]);
    for (uint32_t i = 0; i < numShaders; i++) {
        drawCountArray[i].push_back(0);
        drawFirstIndexArray[i].push_back(0);
        glBindVertexArray(vertexArrayObject[i]);
        indexBufferPerShader[i].createBuffer(GL_ELEMENT_ARRAY_BUFFER);
    }
    vertexBuffer.bind();
    renderer.init(&vertexArrayObject[0], &drawCountArray[0], &drawFirstIndexArray[0], &componentsPerShader[0]);
}

void ComponentSystem::createComponent(const float* componentVertices, const uint32_t numVertexFloats, const LogicType logicType) { //Pass array by pointer
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

uint32_t ComponentSystem::addComponent(const uint32_t* newIndices, uint32_t numIndices, const shaderType shaderID) {
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
        /*DrawElementsCommand* oldCommandPtr = findLastCommand(drawCommands[shaderID], firstComponentIndex);
        oldCommandPtr->count += numIndices;*/
        const uint32_t index = findLastCommand(drawCountArray[shaderID], drawFirstIndexArray[shaderID], firstComponentIndex);
        drawCountArray[shaderID][index] += numIndices;
    }
    else {
        firstComponentIndex = freeMemoryRegion.second;

        std::memcpy(&indices[freeMemoryRegion.second], newIndices, numIndices * sizeof(uint32_t));
        ib.updateData(freeMemoryRegion.first * sizeof(uint32_t), newIndices, numIndices * sizeof(uint32_t));
        //Now we need to merge the draw commands
        /*const std::pair edgeCommands = findEdgeCommands(drawCommands[shaderID], freeMemoryRegion.second, numIndices);
        edgeCommands.first->count += edgeCommands.second->count + numIndices; //Expand the first command over the memory region*/
        const std::pair edgeCommands = findEdgeCommands(drawCountArray[shaderID], drawFirstIndexArray[shaderID], freeMemoryRegion.second, numIndices * sizeof(uint32_t));
        drawCountArray[shaderID][edgeCommands.first] += drawCountArray[shaderID][edgeCommands.second] + numIndices;
        
        std::vector<GLsizei>& counts = drawCountArray[shaderID];
        std::vector<const void*>& indices = drawFirstIndexArray[shaderID];
        //Delete the second command
        if (counts.size() < 3) { //Its the last element, we can safely delete
            counts.pop_back();
            indices.pop_back();
            //If there is only one draw command, guaranteed no more free memory
            if (counts.size() == 1)
                indicesFreeMemoryMaybe = false;
        }
        else { //Its somewhere in the middle, memcpy the last command over it, then delete the last command
            //std::memcpy(edgeCommands.second, &(drawCommands[shaderID].back()), sizeof(DrawElementsCommand));
            counts[edgeCommands.second] = counts.back();
            indices[edgeCommands.second] = indices.back();
            counts.pop_back();
            indices.pop_back();
        }
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
        /*DrawElementsCommand* command = findLastCommand(drawCommands[removedComponent.shaderID], indices.size());
        command->count -= removedComponent.numIndices;*/
        const uint32_t index = findLastCommand(drawCountArray[removedComponent.shaderID], drawFirstIndexArray[removedComponent.shaderID], indices.size());
        drawCountArray[removedComponent.shaderID][index] -= removedComponent.numIndices;
        
        indices.resize(indices.size() - removedComponent.numIndices);
        indexBufferPerShader[removedComponent.shaderID].removeData(removedComponent.numIndices * sizeof(uint32_t));
    }
    else if (components.back().numIndices == removedComponent.numIndices) { 
        //If the last component is the same size, we can simply overwrite and resize
        Component& movedComponent = components.back();

        //Find correct command to resize before memory shenanigans
        /*DrawElementsCommand* command = findLastCommand(drawCommands[movedComponent.shaderID], movedComponent.firstIndex + movedComponent.numIndices);
        command->count -= movedComponent.numIndices;*/
        const uint32_t index = findLastCommand(drawCountArray[removedComponent.shaderID], drawFirstIndexArray[removedComponent.shaderID], movedComponent.firstIndex + movedComponent.numIndices);
        drawCountArray[removedComponent.shaderID][index] -= movedComponent.numIndices;

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
        /*DrawElementsCommand* oldCommandPtr = findContainingCommand(drawCommands[removedComponent.shaderID], removedComponent.firstIndex, removedComponent.numIndices);*/
        const uint32_t index = findContainingCommand(drawCountArray[removedComponent.shaderID], drawFirstIndexArray[removedComponent.shaderID], removedComponent.firstIndex, removedComponent.numIndices);
        //First command can overwrite the one we found, second one will be appended
        /*const uint32_t oldCount = oldCommandPtr->count; //Store, needed for second command
        oldCommandPtr->count = removedComponent.firstIndex - oldCommandPtr->firstIndex;*/ //Simply change the count, location is fine
        const uint32_t oldCount = drawCountArray[removedComponent.shaderID][index];
        drawCountArray[removedComponent.shaderID][index] = removedComponent.firstIndex - drawCountArray[removedComponent.shaderID][index];

        //second command: count needs to be the old count minus this components end (so it is everything after). Location is the index right after the removed component
        /*DrawElementsCommand newCommand {oldCount - removedComponent.firstIndex - removedComponent.numIndices, removedComponent.firstIndex + removedComponent.numIndices};
        drawCommands[removedComponent.shaderID].push_back(newCommand);*/
        drawCountArray[removedComponent.shaderID].push_back(oldCount - removedComponent.firstIndex - removedComponent.numIndices);
        drawFirstIndexArray[removedComponent.shaderID].push_back((const void*)(uintptr_t)(removedComponent.firstIndex + removedComponent.numIndices));
    }

    const uint32_t componentIndex = &removedComponent - &components[0];
    if (componentIndex == components.size())
        components.pop_back();
    else {
        components[componentIndex] = components.back();
        components.pop_back();
    }
}

void ComponentSystem::moveComponent(Component& component, const shaderType newShader) {
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

void ComponentSystem::printIndirectDraw(const uint32_t shaderID, const uint32_t index) const {
    std::cout << "indirect draw attribs:\nCount: " << drawCountArray[shaderID][index] << "\nFirst index: " << (uintptr_t)drawFirstIndexArray[shaderID][index] << std::endl;
}
