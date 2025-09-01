#include "indexBuffer.hpp"
#include "vertexBuffer.hpp"
#include <glm/geometric.hpp>
#include <memory>
#include <textureRenderer.hpp>
#include <openglPCH.hpp>
#include <constants.hpp>
#include <shaderLoader.hpp>
#include <textureLoader.hpp>

unsigned int TexturedObject::shader = 0;
unsigned int TexturedObject::uniformModelLocation = 0;
unsigned int TexturedObject::uniformProjectionViewLocation = 0;
unsigned int TexturedObject::uniformTextureLocation = 0;
glm::vec3 TexturedObject::cameraPosition = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 TexturedObject::cameraAngle = glm::vec3(0.0f, 0.0f, 1.0f);
glm::mat4 TexturedObject::view = identity_mat4;
glm::mat4 TexturedObject::projection = identity_mat4;
float TexturedObject::lastMouseX = 0.0f;
float TexturedObject::lastMouseY = 0.0f;
float TexturedObject::pitch = 90.0f;
float TexturedObject::yaw = 90.0f;
bool TexturedObject::doUpdateMouse = true;

TexturedObject::TexturedObject(float (&startingVertices)[12], unsigned int textureID) {
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
    ShaderProgramSource shaderSource = parseShader("shaders/renderWorldTexture.shader");
    shader = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);

    uniformModelLocation = glGetUniformLocation(shader, "model");
    uniformProjectionViewLocation = glGetUniformLocation(shader, "projectionView");
    uniformTextureLocation = glGetUniformLocation(shader, "inputTexture");

    projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.01f, 100.0f);

    cameraAngle.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    cameraAngle.y = sin(glm::radians(pitch));
    cameraAngle.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
}

void TexturedObject::updateProjection(const int width, const int height) {
    projection = glm::perspective(glm::radians(90.0f), (float) width / (float) height, 0.01f, 100.0f);
}

void TexturedObject::updateView() {
    cameraAngle.x = sin(glm::radians(pitch)) * cos(glm::radians(yaw));
    cameraAngle.y = cos(glm::radians(pitch));
    cameraAngle.z = sin(glm::radians(pitch)) * sin(glm::radians(yaw));

    view = glm::lookAt(cameraPosition, cameraPosition + cameraAngle, upVector);
}

void TexturedObject::updateMouse(const float mouseX, const float mouseY) {
    const float xOffset = mouseX - lastMouseX;
    const float yOffset = mouseY - lastMouseY; // y is inverted since pitch=0 is up
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    if (doUpdateMouse) {
        pitch += yOffset * sensitivity;
        yaw += xOffset * sensitivity;

        if (pitch < 1.0f)
            pitch = 1.0f;
        else if (pitch > 179.0f)
            pitch = 179.0f;

        updateView();
    }
    else {
        doUpdateMouse = true;
    }
}

void TexturedObject::moveCamera(const glm::vec3 relativeMovement) {
    cameraPosition += glm::normalize(glm::vec3(cameraAngle.x, 0.0f, cameraAngle.z)) * relativeMovement.z;
    cameraPosition += glm::vec3(0.0f, relativeMovement.y, 0.0f);
    if (relativeMovement.x != 0.0f) {
        glm::vec3 cameraAngleCross = glm::normalize(glm::cross(cameraAngle, upVector));
        cameraPosition += cameraAngleCross * relativeMovement.x;
    }

    updateView();
}

void TexturedObject::render() {
    glBindVertexArray(vao);
    glUseProgram(shader);

    glm::mat4 projectionView = projection * view;

    glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(uniformProjectionViewLocation, 1, GL_FALSE, &projectionView[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(uniformTextureLocation, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << std::hex << err << std::endl;
    }
}
