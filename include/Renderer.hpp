#ifndef RENDERER
#define RENDERER

#include <openglPCH.hpp>
#include <vertexBuffer.hpp>
#include <indexBuffer.hpp>
#include <component.hpp>
#include <vector>

class Renderer {
    private:
        std::vector<float> verticesPerShader[5];
        std::vector<uint32_t> indicesPerShader[5];
        bool maybeFreeVertices = false;
        bool maybeFreeIndices = false;
        std::vector<Component> components;
        VertexBuffer vertexBufferPerShader[5];
        IndexBuffer indexBufferPerShader[5];
    public:
        Renderer();
        void addComponent(float vertices[], uint32_t numVertices, LogicType logic);
};

#endif