#include <Renderer.hpp>
#include <shaderType.hpp>

Renderer::Renderer() {
    
}

void Renderer::addComponent(float componentVertices[], uint32_t numVertexFloats, LogicType logicType) {
    std::vector<float>& vertices = verticesPerShader[shaderType::Ethereal];
    std::vector<uint32_t>& indices = indicesPerShader[shaderType::Ethereal];
    uint32_t vertexCount = numVertexFloats / 3;
    uint32_t firstComponentVertex = vertices.size();
    uint32_t firstComponentIndex = indices.size();
    vertices.reserve(numVertexFloats + firstComponentVertex); // Reserve in advance to avoid repetitive allocation
    indices.reserve(vertexCount + firstComponentIndex);

    for (uint32_t i = 0; i < vertexCount; i++) {
        bool vertexAlreadyExists = false;
        for (uint32_t j = 0; j < sizeof(shaderType::Ethereal) / (3 * sizeof(float)); j++) {
            if (componentVertices[3*i] == vertices[3*j] && componentVertices[3*i+1] == vertices[3*j+1] && componentVertices[3*i+2] == vertices[3*j+2]) {
                vertexAlreadyExists = true;
                indices.push_back(j);
            }
        }
        if (!vertexAlreadyExists) {
            indices.push_back(vertices.size() / 3);
            vertices.push_back(componentVertices[3*i]);
            vertices.push_back(componentVertices[3*i+1]);
            vertices.push_back(componentVertices[3*i+2]);
        }
    }

    VertexBuffer& vb = vertexBufferPerShader[shaderType::Ethereal];
    if (vb.getBufferSize() < vb.getUsedMemorySize() + numVertexFloats * sizeof(float)) //Need a larger buffer, upload everything
        vb.UploadBuffer(&vertices[0], vertices.size() * sizeof(vertices[0]));
    else
        vb.AddData(&vertices[firstComponentVertex], numVertexFloats * sizeof(float)); //Upload only vertices to be added

    IndexBuffer& ib = indexBufferPerShader[shaderType::Ethereal];
    if (ib.getBufferSize() < ib.getUsedMemorySize() + numVertexFloats * sizeof(float))
        ib.UploadBuffer(&vertices[0], vertices.size() * sizeof(vertices[0]));
    else
        ib.AddData(&vertices[firstComponentVertex], numVertexFloats * sizeof(float));

}
