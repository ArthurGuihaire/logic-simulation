#pragma once
#include <instancedComponents.hpp>

class UniqueComponentSystem : public ComponentSystem {
    public:
        UniqueComponentSystem(Renderer& renderer);
        void createUniqueComponent(const float* vertices, const uint32_t numVertices, const LogicType logic);
        uint32_t addUniqueComponent(const uint16_t* newIndices, uint32_t numIndices, shaderType shaderID);
        void removeUniqueComponent(UniqueComponent& removedComponent);
        void moveUniqueComponent(UniqueComponent& movedComponent, shaderType newShader);

        void printIndirectDraw(uint32_t shaderID, uint32_t index) const;
        //Needed for renderer
        std::vector<UniqueComponent> componentsPerShader[5];
        std::vector<GLsizei> drawCountArray[numShaders];
        std::vector<const void*> drawFirstIndexArray[numShaders];
    private:
        std::vector<float> vertices;
        bool indicesFreeMemoryMaybe;
        std::vector<uint16_t> indicesPerShader[numShaders];
        std::vector<std::pair<uint32_t, uint32_t>> freeIndicesPerShader[numShaders];
        gpuBuffer vertexBuffer;
        gpuBuffer indexBufferPerShader[numShaders];
};

extern UniqueComponentSystem* g_componentSystem;
