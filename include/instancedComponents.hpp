#pragma once
#include <componentStructs.hpp>
#include <gpuBuffer.hpp>
#include <renderer.hpp>
#include <utils.hpp>
#include <unordered_map>
class ComponentSystem {
    public:
        ComponentSystem(Renderer& renderer);
        void createComponent(const glm::vec3& coordianates, const glm::mat4& startingModel, const glm::vec4& color, const uint32_t mesh, const LogicType logic);
        void removeComponent(Component& removedComponent);
        InstanceAttribute& getComponentAttributes(const Component& component);
        //Needed for renderer
        std::vector<Component> componentsPerMesh[numMeshes];
        std::vector<InstanceAttribute> instanceAttribs[numMeshes];
        uint32_t instanceCount[numMeshes];
        std::unordered_map<glm::ivec3, Component*> hashMap;
    private:
        float meshVertices[Geometry::totalSizeVertices];
        std::vector<uint16_t> indicesPerMesh[numMeshes];
        gpuBuffer vertexBuffer;
        gpuBuffer indexBufferPerMesh[numMeshes];
        gpuBuffer instanceAttribsBuffer[numMeshes];
};
