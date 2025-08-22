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
unsigned int TexturedObject::uniformViewLocation = 0;
unsigned int TexturedObject::uniformProjectionLocation = 0;
unsigned int TexturedObject::uniformTextureLocation = 0;

TexturedObject::TexturedObject(float startingVertices[12], unsigned int textureID) {
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

void TexturedObject::init() {
    ShaderProgramSource shaderSource = parseShader("shaders/renderTexture.shader");
    shader = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);

    uniformModelLocation = glGetUniformLocation(shader, "model");
    uniformViewLocation = glGetUniformLocation(shader, "view");
    uniformProjectionLocation = glGetUniformLocation(shader, "projection");
    uniformTextureLocation = glGetUniformLocation(shader, "inputTexture");
}

void TexturedObject::render(glm::mat4 view, glm::mat4 projection) {
    glBindVertexArray(vao);
    glUseProgram(shader);

    glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, &projection[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(uniformTextureLocation, 0); // 0 = GL_TEXTURE0

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << std::hex << err << std::endl;
    }
}