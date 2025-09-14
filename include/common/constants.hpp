#include <glm/glm.hpp>
#ifndef CONSTANTS
#define CONSTANTS
constexpr uint16_t windowWidth = 800;
constexpr uint16_t windowHeight = 600;

constexpr inline glm::mat4 identity_mat4 = glm::mat4(1.0f);

constexpr float movementSpeed = 0.015f;

constexpr inline float textureVertices[4][2] = {
    {1.0f, 1.0f}, // top-right
    {0.0f, 1.0f}, // top-left
    {0.0f, 0.0f}, // bottom-left
    {1.0f, 0.0f}  // bottom-right
};
constexpr inline glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
constexpr float sensitivity = 0.5f;

constexpr unsigned int gpuBufferMultiple = 4096;
constexpr unsigned int numShaders = 5;

namespace Colors {
    constexpr inline glm::vec4 red {1.0f, 0.0f, 0.0f, 1.0f};
    constexpr inline glm::vec4 darkRed {0.25f, 0.0f, 0.0f, 1.0f};
    constexpr inline glm::vec4 green {0.0f, 1.0f, 0.0f, 1.0f};
    constexpr inline glm::vec4 transparentGreen {0.0f, 1.0f, 0.0f, 0.5f};
    constexpr inline glm::vec4 blue {0.0f, 0.0f, 1.0f, 1.0f};
}

namespace Geometry {
    constexpr inline float triangle[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 
    };
    constexpr inline float cubeVertices[] = {
        // Front face (z = 1)
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,

        -1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,

        // Back face (z = -1)
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,

        // Left face (x = -1)
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        // Right face (x = 1)
        1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,

        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,

        // Top face (y = 1)
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,

        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        // Bottom face (y = -1)
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f
    };

}

#endif