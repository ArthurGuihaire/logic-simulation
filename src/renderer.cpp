#include "componentStructs.hpp"
#include "gl.h"
#include "utils.hpp"
#include <renderer.hpp>
#include <shaderLoader.hpp>
#include <cstring> //For std::memcpy
#include <glm/gtc/type_ptr.hpp>

/*
 * WHY is there only one shader program????
 * NOTE: in future add support for multiple shader programs and loop through them in renderFrame
*/

Renderer::Renderer(Camera& cameraReference)
 : camera(cameraReference)
{
    ShaderProgramSource worldShaderSource = parseShader("shaders/renderWorldTexture.shader");
    shaderProgram = createShader(worldShaderSource.VertexSource, worldShaderSource.FragmentSource);
    
    uniformLocationColor = glGetUniformLocation(shaderProgram, "color");
    uniformLocationProjectionView = glGetUniformLocation(shaderProgram, "projectionView");
}

void Renderer::init(uint32_t* newVao, std::vector<GLsizei>* countArrayPointer, std::vector<const void*>* firstIndexArrayPointer, std::vector<Component>* components) {
    std::memcpy(&uniqueVAO[0], newVao, numShaders * sizeof(uint32_t));

    countArray = countArrayPointer;
    firstIndexArray = firstIndexArrayPointer;
    componentsPerShader = components;

    printOpenGLErrors("init");

    glBindVertexArray(uniqueVAO[0]);
    
    for (uint32_t vertexArray = 0; vertexArray < numShaders; vertexArray++) {
        glBindVertexArray(uniqueVAO[vertexArray]);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

}

void Renderer::renderFrame() {
    /*for (uint32_t shaderGroup = 0; shaderGroup < numShaders; shaderGroup++) {
        if (componentsPerShader[shaderGroup].size() > 0) {
            glBindVertexArray(vao[shaderGroup]);
            commandBufferArrayPointer[shaderGroup].bind();
            glUseProgram(shaderProgram);
            
            glUniform4fv(uniformLocationColor, 1, glm::value_ptr(Colors::blue));

            glUniformMatrix4fv(uniformLocationProjectionView, 1, GL_FALSE, &camera.getViewProjection()[0][0]);
            glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, componentsPerShader[shaderGroup].size(), 0);
            
            printOpenGLErrors("OpenGL Error");
        }
    }*/

    for (uint32_t shaderGroup = 0; shaderGroup < numShaders; shaderGroup++) {
        if (componentsPerShader[shaderGroup].size() > 0) {
            glBindVertexArray(uniqueVAO[shaderGroup]);
            glUseProgram(shaderProgram);
            glUniform4fv(uniformLocationColor, 1, glm::value_ptr(Colors::blue));

            glUniformMatrix4fv(uniformLocationProjectionView, 1, GL_FALSE, &camera.getViewProjection()[0][0]);

            glMultiDrawElements(GL_TRIANGLES, &(countArray[shaderGroup][0]), GL_UNSIGNED_INT, &(firstIndexArray[shaderGroup][0]), countArray[shaderGroup].size());
        }
    }
}
