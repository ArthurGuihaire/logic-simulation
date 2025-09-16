#include "componentStructs.hpp"
#include "gl.h"
#include "utils.hpp"
#include <renderer.hpp>
#include <shaderLoader.hpp>
#include <cstring> //For memcpy
#include <functional>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(Camera& cameraReference)
 : camera(cameraReference)
{
    ShaderProgramSource worldShaderSource = parseShader("shaders/renderWorldTexture.shader");
    shaderProgram = createShader(worldShaderSource.VertexSource, worldShaderSource.FragmentSource);
    
    uniformLocationColor = glGetUniformLocation(shaderProgram, "color");
    uniformLocationProjectionView = glGetUniformLocation(shaderProgram, "projectionView");
}

void Renderer::init(uint32_t* newVao, gpuBuffer* bufferPointer, std::vector<Component>* components) {
    std::memcpy(&vao[0], newVao, numShaders * sizeof(uint32_t));
    commandBufferArrayPointer = bufferPointer;
    componentsPerShader = components;

    printOpenGLErrors("init");

    glBindVertexArray(vao[0]);
    
    for (uint32_t vertexArray = 0; vertexArray < numShaders; vertexArray++) {
        glBindVertexArray(vao[vertexArray]);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

}

void Renderer::renderFrameGeneric() {
    for (uint32_t shaderGroup = 0; shaderGroup < numShaders; shaderGroup++) {
        if (componentsPerShader[shaderGroup].size() > 0) {
            glBindVertexArray(vao[shaderGroup]);
            commandBufferArrayPointer[shaderGroup].bind();
            glUseProgram(shaderProgram);
            
            glUniform4fv(uniformLocationColor, 1, glm::value_ptr(Colors::blue));

            glUniformMatrix4fv(uniformLocationProjectionView, 1, GL_FALSE, &camera.getViewProjection()[0][0]);
            glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, componentsPerShader[shaderGroup].size(), 0);
            
            printOpenGLErrors("OpenGL Error");
        }
    }
}

void Renderer::renderFrameIntelGPU() {
    for (uint32_t shaderGroup = 0; shaderGroup < numShaders; shaderGroup++) {
        size_t numComponents = componentsPerShader[shaderGroup].size();
        if (numComponents > 0) {
            glBindVertexArray(vao[shaderGroup]);
            commandBufferArrayPointer[shaderGroup].bind(); // still binds index buffer
            glUseProgram(shaderProgram);

            glUniform4fv(uniformLocationColor, 1, glm::value_ptr(Colors::blue));
            glUniformMatrix4fv(
                uniformLocationProjectionView,
                1,
                GL_FALSE,
                &camera.getViewProjection()[0][0]
            );

            // 36 indices per component
            GLsizei indexCount = static_cast<GLsizei>(numComponents * 36);

            glDrawElements(
                GL_TRIANGLES,
                indexCount,
                GL_UNSIGNED_INT,
                nullptr // start at beginning of bound index buffer
            );

            printOpenGLErrors("OpenGL Error");
        }
    }
}
