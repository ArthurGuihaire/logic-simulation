#ifndef COMPONENT
#define COMPONENT
#include <shaderType.hpp>
#include <vector>
#include <openglPCH.hpp>
#include <gpuBuffer.hpp>

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

class ComponentSystem {
    public:
        ComponentSystem();
        void createComponent(float* vertices, uint32_t numVertices, LogicType logic);
        uint32_t addComponent(uint32_t* newIndices, uint32_t numIndices, shaderType shaderID);
        void removeComponent(Component& removedComponent);
        void moveComponent(Component& movedComponent, shaderType newShader);
        std::vector<Component> componentsPerShader[5];
    private:
        constexpr static inline unsigned int numShaders = 5;
        std::vector<float> vertices;
        bool indicesFreeMemoryMaybe;
        std::vector<uint32_t> indicesPerShader[numShaders];
        std::vector<std::pair<uint32_t, uint32_t>> freeIndicesPerShader[numShaders];
        std::vector<DrawElementsIndirectCommand> multiDrawCommands[numShaders];
        gpuBuffer vertexBuffer;
        gpuBuffer indexBufferPerShader[numShaders];
        gpuBuffer commandsBufferPerShader[numShaders];
        unsigned int vertexArrayObject[numShaders];
};

#endif