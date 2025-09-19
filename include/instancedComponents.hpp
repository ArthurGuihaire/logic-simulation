#pragma once
#include <componentStructs.hpp>
#include <gpuBuffer.hpp>
#include <renderer.hpp>
class ComponentSystem {
    public:
        ComponentSystem(Renderer& renderer);
        void createComponent(const glm::vec3 coordianates, const glm::vec4 color, const uint32_t mesh, const LogicType logic);
        void removeComponent(Component& removedComponent);
        InstanceAttribute& getComponentAttributes(const Component& component);
        //Needed for renderer
        std::vector<Component> componentsPerMesh[numMeshes];
        std::vector<InstanceAttribute> instanceAttribs[numMeshes];
        uint32_t instanceCount[numMeshes];
    private:
        float meshVertices[GoodGeo::totalSizeVertices];
        std::vector<uint16_t> indicesPerMesh[numMeshes];
        gpuBuffer vertexBuffer;
        gpuBuffer indexBufferPerMesh[numMeshes];
        gpuBuffer instanceAttribsBuffer[numMeshes];
};
