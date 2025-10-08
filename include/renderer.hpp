#pragma once
#include <gpuBuffer.hpp>
#include <constants.hpp>
#include <componentStructs.hpp>
#include <camera.hpp>

class Renderer {
    public:
        Renderer(Camera& cameraReference);
        void initUnique(uint32_t* uniqueVaoArray);
        void initInstanced(uint32_t* instancedVaoArray, gpuBuffer& vertexBuffer, gpuBuffer* instanceAttribsBufferArray);
        void renderFrame();
    private:
        uint32_t uniqueVAO[numShaders];
        uint32_t instancedVAO[numMeshes];

        uint32_t shaderUnique;
        uint32_t shaderInstanced;
        uint32_t shaderSelected;

        uint32_t uniformColorUnique;
        uint32_t uniformProjectionViewUnique;
        uint32_t uniformProjectionViewInstanced;
        uint32_t uniformModelCube;
        uint32_t uniformProjectionViewCube;
        uint32_t uniformColorCube;

        uint32_t selectionCubeVAO; //reuse the vertex buffer, vertex buffer is already a cube
        gpuBuffer selectionCubeEBO;

        Camera& camera;
};
