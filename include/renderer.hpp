#ifndef RENDERER
#define RENDERER
#include <gpuBuffer.hpp>
#include <constants.hpp>
#include <componentStructs.hpp>
#include <camera.hpp>

class Renderer {
    public:
        Renderer(Camera& cameraReference);
        void init(uint32_t* vao, gpuBuffer* bufferPointer);
        void renderFrame();
    private:
        uint32_t vao[numShaders];
        uint32_t shaderProgram;
        uint32_t uniformLocationColor;
        uint32_t uniformLocationModel;
        uint32_t uniformLocationProjectionView;
        gpuBuffer* commandBufferArrayPointer;
        Camera& camera;
};

#endif