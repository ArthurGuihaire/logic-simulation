#ifndef COMPONENT
#define COMPONENT
#include "shaderType.hpp"
#include <vector>
#include <openglPCH.hpp>
#include <vertexBuffer.hpp>
#include <indexBuffer.hpp>

enum class LogicType {AND, NAND, OR, NOR, XOR, NOT, TRAN};

struct Component {
    bool active;
    LogicType type;
    std::vector<uint32_t>* indicesPointer;
    uint32_t firstIndex;
    uint32_t numIndices;
    shaderType shaderID;
    bool evaluate(bool a, bool b) {
        switch (type) {
            case LogicType::TRAN: return a;
            case LogicType::NOT: return !a;
            case LogicType::AND: return a && b;
            case LogicType::NAND: return !(a && b);
            case LogicType::OR: return a || b;
            case LogicType::NOR: return !(a || b);
            case LogicType::XOR: return a != b;
        }
    }
};


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