#include "indexBuffer.hpp"
#include "vertexBuffer.hpp"
#include <memory>
#include <textureRenderer.hpp>
#include <openglPCH.hpp>
#include <constants.hpp>
#include <renderer_constants.hpp>
#include <shaderLoader.hpp>
#include <textureLoader.hpp>

unsigned int TexturedObject::shader = 0;
unsigned int TexturedObject::uniformModelLocation = 0;
unsigned int TexturedObject::uniformProjectionViewLocation = 0;
unsigned int TexturedObject::uniformTextureLocation = 0;
glm::mat4 TexturedObject::view = identity_mat4;
glm::mat4 TexturedObject::projection = identity_mat4;
float TexturedObject::lastMouseX = 0.0f;
float TexturedObject::lastMouseY = 0.0f;

TexturedObject::TexturedObject(float (&startingVertices)[12], unsigned int textureID) {
    for(uint8_t i = 0; i < 3; i++) {
        rotationDegrees[i] = 0;
    }

    texture = textureID;

    model = identity_mat4;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    for(uint8_t vertex = 0; vertex < 4; vertex++){
        for(uint8_t i = 0; i < 3; i++){
            vertices[5*vertex+i] = startingVertices[3*vertex+i];
        }
        vertices[5*vertex + 3] = textureVertices[vertex][0];
        vertices[5*vertex + 4] = textureVertices[vertex][1];
    }
    vb = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

    //Vertex coordinate pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Texture coordinate pointer
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    unsigned int indices[6] = {
        0, 1, 2, 
        0, 2, 3
    };
    ib = std::make_unique<IndexBuffer>(indices, 6);
}

void TexturedObject::init(const int width, const int height) {
    ShaderProgramSource shaderSource = parseShader("shaders/renderTexture.shader");
    shader = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);

    uniformModelLocation = glGetUniformLocation(shader, "model");
    uniformProjectionViewLocation = glGetUniformLocation(shader, "projectionView");
    uniformTextureLocation = glGetUniformLocation(shader, "inputTexture");

    view = glm::translate(identity_mat4, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.01f, 100.0f);
}

void TexturedObject::updateProjection(const int width, const int height) {
    projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.01f, 100.0f);
}

void TexturedObject::updateView(const glm::mat4 viewUpdateMatrix) {
    view = viewUpdateMatrix * view;
}

void TexturedObject::updateMouse(const float mouseX, const float mouseY) {
    const float xOffset = mouseX - lastMouseX;
    const float yOffset = mouseY - lastMouseY;
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    const float magnitude = 0.004f * glm::sqrt(xOffset * xOffset + yOffset * yOffset);
    glm::mat4 viewUpdateMatrix = glm::rotate(identity_mat4, magnitude, glm::vec3(yOffset, xOffset, 0.0f));
    TexturedObject::updateView(viewUpdateMatrix);
}

void TexturedObject::render() {
    glBindVertexArray(vao);
    glUseProgram(shader);

    glm::mat4 projectionView = projection * view;

    glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(uniformProjectionViewLocation, 1, GL_FALSE, &projectionView[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(uniformTextureLocation, 0); // 0 = GL_TEXTURE0

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << std::hex << err << std::endl;
    }
}