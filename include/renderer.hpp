#ifndef RENDERER
#define RENDERER
#include <gpuBuffer.hpp>
#include <constants.hpp>
#include <componentStructs.hpp>
#include <camera.hpp>

class Renderer {
    public:
        Renderer(Camera& cameraReference);
        void init(uint32_t* newVao, std::vector<GLsizei>* countArray, std::vector<const void*>* firstIndexArray, std::vector<UniqueComponent>* components);
        void renderFrame();
    private:
        uint32_t uniqueVAO[numShaders];
        uint32_t shaderProgram;
        uint32_t uniformLocationColor;
        uint32_t uniformLocationProjectionView;
        std::vector<GLsizei>* countArray;
        std::vector<const void*>* firstIndexArray;
        Camera& camera;
        std::vector<UniqueComponent>* componentsPerShader;
};

#endif