#include "component.hpp"
#include <Renderer.hpp>
#include <shaderType.hpp>
#include <cstring> //For memcpy

Renderer::Renderer() {
    
}

void Renderer::addComponent(float componentVertices[], uint32_t numVertexFloats, LogicType logicType) {
    std::vector<uint32_t>& indices = indicesPerShader[shaderType::Ethereal];
    //Get size variables that are needed later
    uint32_t vertexCount = numVertexFloats / 3;
    uint32_t firstComponentVertex = vertices.size();
    uint32_t firstComponentIndex = indices.size();
    vertices.reserve(numVertexFloats + firstComponentVertex); // Reserve in advance to avoid repetitive allocation
    indices.reserve(vertexCount + firstComponentIndex);

    for (uint32_t i = 0; i < vertexCount; i++) {
        bool vertexAlreadyExists = false;
        //For each vertex check if its already in the buffer
        for (uint32_t j = 0; j < vertices.size(); j++) {
            if (componentVertices[3*i] == vertices[3*j] && componentVertices[3*i+1] == vertices[3*j+1] && componentVertices[3*i+2] == vertices[3*j+2]) {
                //If its in the buffer point to it with an index
                vertexAlreadyExists = true;
                indices.push_back(j);
            }
        }
        if (!vertexAlreadyExists) {
            //If its not in the buffer add it to the buffer and point to it with an index
            indices.push_back(vertices.size() / 3);
            vertices.push_back(componentVertices[3*i]);
            vertices.push_back(componentVertices[3*i+1]);
            vertices.push_back(componentVertices[3*i+2]);
        }
    }

    //Update the buffers GPU-side
    if (vertexBuffer.getBufferSize() < vertexBuffer.getUsedMemorySize() + numVertexFloats * sizeof(float)) //Need a larger buffer, upload everything
        vertexBuffer.UploadBuffer(&vertices[0], vertices.size() * sizeof(vertices[0]));
    else
        vertexBuffer.AddData(&vertices[firstComponentVertex], numVertexFloats * sizeof(float)); //Upload only vertices to be added

    IndexBuffer& ib = indexBufferPerShader[shaderType::Ethereal];
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
    if (componentIndex == components.size() - 1) { //Are we removing the last element?
        //Since the component is at the end we simply resize the index array to exclude it
        indicesReference.resize(indicesReference.size() - removedComponent.numIndices);
        indexBufferPerShader[removedComponent.shaderID].RemoveData(removedComponent.numIndices * sizeof(unsigned int));
        components.pop_back(); //Also delete the component itself
    }
    else { //We are removing an element that isn't the last element
        //If the last component is the same size, we can simply overwrite and resize
        if (components.back().numIndices == removedComponent.numIndices) {
            Component& movedComponent = components.back();
            //Pass in the data to the new location before freeing the memory
            indexBufferPerShader[removedComponent.shaderID].UpdateData(removedComponent.firstIndex * sizeof(unsigned int), &((*movedComponent.indicesPointer)[movedComponent.firstIndex]), movedComponent.numIndices * sizeof(unsigned int));
            indexBufferPerShader[removedComponent.shaderID].RemoveData(removedComponent.numIndices * sizeof(unsigned int)); //Then free GPU memory
            //std::memcpy(&indicesReference[removedComponent.])
            indicesReference.resize(indicesReference.size() - removedComponent.numIndices); //
            components[componentIndex] = components.back(); //Overwite
            
        }
    }
}
