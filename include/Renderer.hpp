#ifndef RENDERER
#define RENDERER

#include <openglPCH.hpp>
#include <vertexBuffer.hpp>
#include <indexBuffer.hpp>
#include <component.hpp>
#include <vector>

struct DrawElementsIndirectCommand {
    GLuint count;
    GLuint instanceCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint baseInstance;
};

class Renderer {
    private:
        std::vector<float> vertices;
        std::vector<uint32_t> indicesPerShader[5];
        std::vector<Component> components;
        std::vector<DrawElementsIndirectCommand> multiDrawCommands;
        VertexBuffer vertexBuffer;
        IndexBuffer indexBufferPerShader[5];
    public:
        Renderer();
        void addComponent(float vertices[], uint32_t numVertices, LogicType logic);
        void removeComponent(uint32_t componentIndex);
};

#endif