#include <renderer.hpp>
#include <shaderLoader.hpp>
#include <cstring> //For memcpy

Renderer::Renderer(Camera& cameraReference)
 : camera(cameraReference)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    ShaderProgramSource worldShaderSource = parseShader("shaders/renderWorldTexture.shader");
    shaderProgram = createShader(worldShaderSource.VertexSource, worldShaderSource.FragmentSource);
    glUseProgram(shaderProgram);
    
    uniformLocationColor = glGetUniformLocation(shaderProgram, "color");
    uniformLocationModel = glGetUniformLocation(shaderProgram, "model");
    uniformLocationProjectionView = glGetUniformLocation(shaderProgram, "projectionView");
}

void Renderer::init(uint32_t* newVao, gpuBuffer* bufferPointer) {
    std::memcpy(&vao[0], newVao, numShaders * sizeof(uint32_t));
    commandBufferArrayPointer = bufferPointer;
}

void Renderer::renderFrame() {
    for (uint32_t shaderGroup = 0; shaderGroup < numShaders; shaderGroup++) {
        glBindVertexArray(vao[shaderGroup]);
        commandBufferArrayPointer[shaderGroup].bind();
        
        glUniform3f(uniformLocationColor, Colors::blue.x, Colors::blue.y, Colors::blue.z);
        glUniformMatrix4fv(uniformLocationProjectionView, 1, GL_FALSE, &camera.getViewProjection()[0][0]);
    }
}
