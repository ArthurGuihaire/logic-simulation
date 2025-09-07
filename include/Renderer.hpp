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
        bool indicesAreFragmented;
        bool indicesFreeMemoryMaybe;
        std::vector<uint32_t> indicesPerShader[5];
        std::vector<std::pair<uint32_t, uint32_t>> freeIndicesPerShader[5];
        std::vector<Component> components;
        std::vector<DrawElementsIndirectCommand> multiDrawCommands[5];
        VertexBuffer vertexBuffer;
        IndexBuffer indexBufferPerShader[5];
    public:
        Renderer();
        void addComponent(float vertices[], uint32_t numVertices, LogicType logic);
        void removeComponent(uint32_t componentIndex);
};

#endif