#include <glm/glm.hpp>
#ifndef CONSTANTS
#define CONSTANTS
constexpr uint16_t windowWidth = 800;
constexpr uint16_t windowHeight = 600;

constexpr glm::mat4 identity_mat4 = glm::mat4(1.0f);

constexpr float movementSpeed = 0.015f;

constexpr float textureVertices[4][2] = {
    {1.0f, 1.0f}, // top-right
    {0.0f, 1.0f}, // top-left
    {0.0f, 0.0f}, // bottom-left
    {1.0f, 0.0f}  // bottom-right
};
constexpr glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
constexpr float sensitivity = 0.5f;

constexpr unsigned int gpuBufferMultiple = 1024;

#endif