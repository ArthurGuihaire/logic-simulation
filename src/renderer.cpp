#include <componentSystem.hpp>
#include "gl.h"
#include "utils.hpp"
#include <renderer.hpp>
#include <shaderLoader.hpp>
#include <cstring> //For std::memcpy
#include <glm/gtc/type_ptr.hpp>
#include <constants.hpp>

/*
 * WHY is there only one shader program????
 * NOTE: in future add support for multiple shader programs and loop through them in renderFrame
*/

/*
 * NOTE: NEED TO CORRECTLY INITIALIZE VERTEX ATTRIBUTE POINTERS FOR INSTANCED
*/

Renderer::Renderer(Camera& cameraReference)
 : camera(cameraReference)
{
    ShaderProgramSource uniqueShaderSource = parseShader("shaders/renderUniqueMesh.shader");
    shaderUnique = createShader(uniqueShaderSource.VertexSource, uniqueShaderSource.FragmentSource);
    
    uniformColorUnique = glGetUniformLocation(shaderUnique, "color");
    uniformProjectionViewUnique = glGetUniformLocation(shaderUnique, "projectionView");

    ShaderProgramSource instancedShaderSource = parseShader("shaders/renderInstancedMesh.shader");
    shaderInstanced = createShader(instancedShaderSource.VertexSource, instancedShaderSource.FragmentSource);

    uniformProjectionViewInstanced = glGetUniformLocation(shaderInstanced, "projectionView");
}

void Renderer::initUnique(uint32_t* uniqueVaoArray) {
    std::memcpy(&uniqueVAO, uniqueVaoArray, numMeshes * sizeof(uint32_t));

    printOpenGLErrors("initUnique");

    glBindVertexArray(uniqueVAO[0]);
    
    for (uint32_t vertexArray = 0; vertexArray < numShaders; vertexArray++) {
        glBindVertexArray(uniqueVAO[vertexArray]);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }
}

void Renderer::initInstanced(uint32_t* instancedVaoArray, gpuBuffer& vertexBuffer, gpuBuffer* instanceAttribsBufferArray) {
    printOpenGLErrors("1");
    std::memcpy(&instancedVAO, instancedVaoArray, numMeshes * sizeof(uint32_t));
    
    for (uint32_t vertexArray = 0; vertexArray < numMeshes; vertexArray++) {
        glBindVertexArray(instancedVAO[vertexArray]);
        //First vertexAttribPointer for vertex position
        vertexBuffer.bind(); //Switch to actual vertex buffer
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        //Second vertexAttribPointer for color
        instanceAttribsBufferArray[vertexArray].bind(); //Need the attribs buffer
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceAttribute), 0);
        glVertexAttribDivisor(1, 1);
        //Next 4 vertexAttribPointers for model matrix
        for (uint32_t i = 0; i < 4; i++) {
            std::cout << i << "\n";
            printOpenGLErrors("before");
            glEnableVertexAttribArray(2 + i);
            printOpenGLErrors("2");
            glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceAttribute), (void*)((i+1) * sizeof(glm::vec4)));
            printOpenGLErrors("3");
            glVertexAttribDivisor(2 + i, 1);
            printOpenGLErrors("4");
            std::cout << i << std::endl;
        }
    }
}

void Renderer::renderFrame() {
    for (uint32_t shaderGroup = 0; shaderGroup < numShaders; shaderGroup++) {
        if (g_componentSystem->componentsPerShader[shaderGroup].size() > 0) {
            glBindVertexArray(uniqueVAO[shaderGroup]);
            glUseProgram(shaderUnique);
            glUniform4fv(uniformColorUnique, 1, glm::value_ptr(Colors::blue));

            glUniformMatrix4fv(uniformProjectionViewUnique, 1, GL_FALSE, &camera.getViewProjection()[0][0]);

            glMultiDrawElements(GL_TRIANGLES, &(g_componentSystem->drawCountArray[shaderGroup][0]), GL_UNSIGNED_SHORT, &(g_componentSystem->drawFirstIndexArray[shaderGroup][0]), g_componentSystem->drawCountArray[shaderGroup].size());
        }
    }

    for (uint32_t meshGroup = 0; meshGroup < numMeshes; meshGroup++) {
        //if (g_componentSystem->componentsPerMesh[meshGroup].size() > 0) {
            glBindVertexArray(instancedVAO[meshGroup]);
            glUseProgram(shaderInstanced);

            glUniformMatrix4fv(uniformProjectionViewInstanced, 1, GL_FALSE, &camera.getViewProjection()[0][0]);

            glDrawElementsInstanced(GL_TRIANGLES, GoodGeo::numIndices[meshGroup], GL_UNSIGNED_SHORT, (void*)0, g_componentSystem->instanceCount[meshGroup]);
        //}
    }
}
