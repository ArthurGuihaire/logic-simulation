#include <openglPCH.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint loadTexture(const char* path) {
    GLFWwindow* ctx = glfwGetCurrentContext();
    if (!ctx) {
        std::cerr << "[ERROR] No current OpenGL context!" << std::endl;
        return 0;
    }

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "[ERROR] GLAD not initialized!" << std::endl;
        return 0;
    }

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "[ERROR] Failed to load texture: " << path << std::endl;
        return 0;
    }

    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Fix alignment for non-4-byte row size
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    std::cout << "Loaded texture: " << path << " (" << width << "x" << height << ", "
              << nrChannels << " channels)" << std::endl;

    return textureID;
}
