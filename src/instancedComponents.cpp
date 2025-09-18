#include <instancedComponents.hpp>
#include <cstring> // For std::memcpy

ComponentSystem::ComponentSystem() {
    glGenVertexArrays(numMeshes, &vao[0]);
    vertexBuffer.createBuffer(GL_ARRAY_BUFFER, &GoodGeo::vertices[0], GoodGeo::totalSizeVertices);
    for (uint32_t i = 0; i < numMeshes; i++) {
        glBindVertexArray(vao[i]);
        vertexBuffer.bind();
        indexBufferPerMesh[i].createBuffer(GL_ELEMENT_ARRAY_BUFFER, &GoodGeo::indices[GoodGeo::indexStart[i]], GoodGeo::totalSizeIndices);
        instanceAttribsBuffer[i].createBuffer(GL_ARRAY_BUFFER);
    }
}

void ComponentSystem::createComponent(const glm::vec3 coordianates, const glm::vec4 color, const uint32_t mesh, const LogicType logic) {
    //Create a new component
    componentsPerMesh[mesh].push_back({false, logic, mesh});
    //Add the translation matrix to list of instance attributes
    instanceAttribs[mesh].push_back({glm::translate(identity_mat4, coordianates), color});
    //Update GPU buffer
    gpuBuffer& iab = instanceAttribsBuffer[mesh];
    if (iab.getUsedMemorySize() + sizeof(InstanceAttribute) > iab.getBufferSize()) {
        iab.uploadBuffer(&instanceAttribs[mesh][0], instanceAttribs[mesh].size() * sizeof(InstanceAttribute));
    }
    else
        iab.addData(&instanceAttribs[mesh].back(), sizeof(InstanceAttribute));
}

void ComponentSystem::removeComponent(Component& removedComponent) {
    const uint32_t componentIndex = &removedComponent - &componentsPerMesh[removedComponent.meshType][0];
    const uint32_t mesh = removedComponent.meshType;
    //If its the last component just remove it
    if (componentIndex == componentsPerMesh[mesh].size() - 1) {
        componentsPerMesh[mesh].pop_back();
        instanceAttribs[mesh].pop_back();
        //Update GPU buffer
        instanceAttribsBuffer[mesh].removeData(sizeof(InstanceAttribute));
    }
    else {
        std::memcpy(&componentsPerMesh[mesh][componentIndex], &componentsPerMesh[mesh].back(), sizeof(Component));
        std::memcpy(&instanceAttribs[mesh][componentIndex], &instanceAttribs[mesh].back(), sizeof(Component));
        //Update GPU buffer
        instanceAttribsBuffer[mesh].updateData(componentIndex * sizeof(InstanceAttribute), &instanceAttribs[mesh].back(), sizeof(InstanceAttribute));
        instanceAttribsBuffer[mesh].removeData(sizeof(InstanceAttribute));
    }
}

InstanceAttribute& ComponentSystem::getComponentAttributes(const Component& component) {
    const uint32_t componentIndex = &component - &(componentsPerMesh[component.meshType][0]);
    return instanceAttribs[component.meshType][componentIndex];
}
