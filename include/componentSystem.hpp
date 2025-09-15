#ifndef COMPONENT
#define COMPONENT
#include <shaderType.hpp>
#include <vector>
#include <openglPCH.hpp>
#include <gpuBuffer.hpp>
#include <constants.hpp>
#include <renderer.hpp>
#include <componentStructs.hpp>

class ComponentSystem {
    public:
        ComponentSystem(Renderer& renderer);
        void createComponent(const float* vertices, const uint32_t numVertices, const LogicType logic);
        uint32_t addComponent(const uint32_t* newIndices, uint32_t numIndices, shaderType shaderID);
        void removeComponent(Component& removedComponent);
        void moveComponent(Component& movedComponent, shaderType newShader);
        void printIndirectDraw(uint32_t shaderID, uint32_t index) const;
        std::vector<Component> componentsPerShader[5];
    private:
        std::vector<float> vertices;
        bool indicesFreeMemoryMaybe;
        std::vector<uint32_t> indicesPerShader[numShaders];
        std::vector<std::pair<uint32_t, uint32_t>> freeIndicesPerShader[numShaders];
        std::vector<DrawElementsIndirectCommand> multiDrawCommands[numShaders];
        gpuBuffer vertexBuffer;
        gpuBuffer indexBufferPerShader[numShaders];
        gpuBuffer commandsBufferPerShader[numShaders];
};

#endif