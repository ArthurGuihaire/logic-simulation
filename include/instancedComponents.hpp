#pragma once
#include <componentStructs.hpp>
#include <gpuBuffer.hpp>
#include <renderer.hpp>
class ComponentSystem {
    public:
        ComponentSystem();
        void createComponent(const glm::vec3 coordianates, const glm::vec4 color, const uint32_t mesh, const LogicType logic);
        void removeComponent(Component& removedComponent);
        InstanceAttribute& getComponentAttributes(const Component& component);
    private:
        float meshVertices[GoodGeo::totalSizeVertices];
        std::vector<Component> componentsPerMesh[numMeshes];
        std::vector<uint16_t> indicesPerMesh[numMeshes];
        std::vector<InstanceAttribute> instanceAttribs[numMeshes];
        uint32_t vao[numMeshes];
        gpuBuffer vertexBuffer;
        gpuBuffer indexBufferPerMesh[numMeshes];
        gpuBuffer instanceAttribsBuffer[numMeshes];
};
