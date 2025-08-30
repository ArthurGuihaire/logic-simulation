#include <Renderer.hpp>

Renderer::Renderer() {
    
}

void Renderer::addComponent(float componentVertices[], uint32_t numVertices, LogicType logicType) {
    uint32_t vertexCount = numVertices / 3;
    std::vector<uint32_t> vertexIndices;
    vertexIndices.reserve(vertexCount);
    vertices.reserve(numVertices + vertices.size()); // Reserve in advance to avoid repetitive allocation
    for (uint32_t i = 0; i < vertexCount; i++) {
        bool vertexAlreadyExists = false;
        for (uint32_t j = 0; j < sizeof(vertices) / (3 * sizeof(float)); j++) {
            if (componentVertices[3*i] == vertices[3*j] && componentVertices[3*i+1] == vertices[3*j+1] && componentVertices[3*i+2] == vertices[3*j+2]) {
                vertexAlreadyExists = true;
                vertexIndices[i] = j;
            }
        }
        if (!vertexAlreadyExists) {
            vertexIndices[i] = vertices.size();
            vertices.push_back(componentVertices[3*i]);
            vertices.push_back(componentVertices[3*i+1]);
            vertices.push_back(componentVertices[3*i+2]);
        }
    }

    Component newComponent{false, logicType, vertexIndices};
    components.push_back(newComponent);

    indices.reserve(indices.size() + vertexCount);
    indices.insert(indices.end(), vertexIndices.begin(), vertexIndices.end());
}
