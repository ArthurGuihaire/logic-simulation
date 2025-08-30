#ifndef RENDERER
#define RENDERER

#include <openglPCH.hpp>
#include <vertexBuffer.hpp>
#include <indexBuffer.hpp>
#include <component.hpp>
#include <vector>

class Renderer {
    private:
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
        std::vector<Component> components;
        VertexBuffer vertexBufferPerShader[5];
    public:
        Renderer();
        void addComponent(float vertices[], uint32_t numVertices, LogicType logic);
};

#endif