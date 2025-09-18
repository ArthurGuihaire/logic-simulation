#ifndef CONSTANTS
#define CONSTANTS
#include <glm/glm.hpp>
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

constexpr unsigned int numMeshes = 2;
constexpr unsigned int numShaders = 2;

namespace Colors {
    constexpr inline glm::vec4 red {1.0f, 0.0f, 0.0f, 1.0f};
    constexpr inline glm::vec4 darkRed {0.25f, 0.0f, 0.0f, 1.0f};
    constexpr inline glm::vec4 green {0.0f, 1.0f, 0.0f, 1.0f};
    constexpr inline glm::vec4 transparentGreen {0.0f, 1.0f, 0.0f, 0.5f};
    constexpr inline glm::vec4 blue {0.0f, 0.0f, 1.0f, 1.0f};
}

namespace BadGeometry {
    constexpr inline float triangle[9] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 
    };
    constexpr inline float cube[108] = {
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

    enum size {
        Triangle = sizeof(triangle) / sizeof(float),
        Cube = sizeof(cube) / sizeof(float)
    };

    constexpr uint32_t totalSize = size::Cube + size::Triangle;
}

namespace GoodGeo {
    constexpr inline float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f 
    };
    constexpr inline uint16_t indices[] = {
        0, 1, 2, 
        7, 8, 9,
        7, 9, 10,
        3, 4, 5,
        3, 5, 6,
        3, 7, 10,
        3, 10, 6,
        4, 8, 9,
        4, 9, 5,
        6, 5, 9,
        6, 9, 10,
        3, 4, 8,
        3, 8, 7
    };
    constexpr inline uint32_t indexStart[] = {
        0, 3
    };

    constexpr uint32_t totalSizeVertices = sizeof(vertices);
    constexpr uint32_t totalSizeIndices = sizeof(indices);
}

#endif