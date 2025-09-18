#ifndef COMPONENT
#define COMPONENT
#include <shaderType.hpp>
#include <vector>
#include <openglPCH.hpp>
#include <gpuBuffer.hpp>
#include <constants.hpp>
#include <renderer.hpp>
#include <componentStructs.hpp>
#include <instancedComponents.hpp>

class ComponentSystem {
    public:
        ComponentSystem(Renderer& renderer);
        void createUniqueComponent(const float* vertices, const uint32_t numVertices, const LogicType logic);
        uint32_t addUniqueComponent(const uint16_t* newIndices, uint32_t numIndices, shaderType shaderID);
        void removeUniqueComponent(UniqueComponent& removedComponent);
        void moveUniqueComponent(UniqueComponent& movedComponent, shaderType newShader);

        void createComponent(const glm::vec3 coordianates, const glm::vec4 color, const uint32_t mesh, const LogicType logic);
        void removeComponent(Component& removedComponent);
        void getComponentAttributes(const Component& component);

        void printIndirectDraw(uint32_t shaderID, uint32_t index) const;
        std::vector<UniqueComponent> componentsPerShader[5];
    private:
        std::vector<float> vertices;
        bool indicesFreeMemoryMaybe;
        std::vector<uint16_t> indicesPerShader[numShaders];
        std::vector<std::pair<uint32_t, uint32_t>> freeIndicesPerShader[numShaders];
        std::vector<GLsizei> drawCountArray[numShaders];
        std::vector<const void*> drawFirstIndexArray[numShaders];
        gpuBuffer vertexBuffer;
        gpuBuffer indexBufferPerShader[numShaders];

        InstancedComponentSystem instancedSystem;
};

#endif