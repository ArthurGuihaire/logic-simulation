#pragma once
#include <utils.hpp>
#include <glm/glm.hpp>
#include <componentStructs.hpp>
constexpr uint16_t windowWidth = 800;
constexpr uint16_t windowHeight = 600;

constexpr inline glm::mat4 identityMat4 = glm::mat4(1.0f);

constexpr float movementSpeed = 0.015f;
constexpr float blockInteractRange = 3.0f;

constexpr inline float textureVertices[4][2] = {
    {1.0f, 1.0f}, // top-right
    {0.0f, 1.0f}, // top-left
    {0.0f, 0.0f}, // bottom-left
    {1.0f, 0.0f}  // bottom-right
};
constexpr inline glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
constexpr float sensitivity = 0.5f;

constexpr unsigned int gpuBufferMultiple = 4096;

constexpr unsigned int numMeshes = 2; //Support for more meshes is already here
constexpr unsigned int numShaders = 2;
constexpr unsigned int numOptions = 9;

namespace Color {
    constexpr inline glm::vec4 null {0.0f, 0.0f, 0.0f, 0.0f};
    constexpr inline glm::vec4 gray {0.4f, 0.4f, 0.4f, 1.0f};
    constexpr inline glm::vec4 green {0.0f, 1.0f, 0.0f, 1.0f};
    constexpr inline glm::vec4 blue {0.0f, 0.0f, 1.0f, 1.0f};
    constexpr inline glm::vec4 darkRed {0.4f, 0.0f, 0.0f, 1.0f};
    constexpr inline glm::vec4 red {1.0f, 0.0f, 0.0f, 1.0f};
    constexpr inline glm::vec4 darkGreen {0.0f, 0.4f, 0.0f, 1.0f};
    constexpr inline glm::vec4 darkBlue {0.0f, 0.0f, 0.4f, 1.0f};
    constexpr inline glm::vec4 purple {0.5f, 0.0f, 0.5f, 1.0f};
    constexpr inline glm::vec4 magenta{1.0f, 0.0f, 0.5f, 1.0f};
}

struct meshAttributes {
    uint32_t meshIndex;
    glm::mat4 startingMatrix;
    glm::vec4 color;
    LogicType logic;
};

namespace Geometry {
    constexpr inline float vertices[] = {
        // Cube vertices
        -0.5f, -0.5f, -0.5f, // 0
        0.5f, -0.5f, -0.5f, // 1
        0.5f,  0.5f, -0.5f, // 2
        -0.5f,  0.5f, -0.5f, // 3
        -0.5f, -0.5f,  0.5f, // 4
        0.5f, -0.5f,  0.5f, // 5
        0.5f,  0.5f,  0.5f, // 6
        -0.5f,  0.5f,  0.5f  // 7
    };

    constexpr inline uint16_t indices[] = {
        // Back face (z = -0.5)
        0, 1, 2,
        2, 3, 0,

        // Front face (z = +0.5)
        4, 7, 6,
        6, 5, 4,

        // Left face (x = -0.5)
        0, 3, 7,
        7, 4, 0,

        // Right face (x = +0.5)
        1, 5, 6,
        6, 2, 1,

        // Bottom face (y = -0.5)
        0, 4, 5,
        5, 1, 0,

        // Top face (y = +0.5)
        3, 2, 6,
        6, 7, 3
    };

    //Mesh index 0 is a mesh with no vertices
    constexpr inline uint32_t indexStart[] = {
        0, 0
    };
    constexpr inline uint32_t numIndices[] = {
        0, 36
    };

    constexpr inline meshAttributes meshAttribArray[numOptions] = {
        //Nothing selected
        {0, identityMat4, Color::null, LogicType::NUL},
        //Wire selected
        {1, scaleMatrix({0.5f, 0.5f, 1.0f}), Color::gray, LogicType::TRAN},
        {1, scaleMatrix({0.5f, 0.5f, 1.0f}), Color::darkRed, LogicType::NOT},
        {1, scaleMatrix({1.0f, 0.5f, 1.0f}), Color::green, LogicType::AND},
        {1, scaleMatrix({1.0f, 0.5f, 1.0f}), Color::blue, LogicType::OR},
        {1, scaleMatrix({1.0f, 0.5f, 1.0f}), Color::darkGreen, LogicType::NAND},
        {1, scaleMatrix({1.0f, 0.5f, 1.0f}), Color::darkBlue, LogicType::NOR},
        {1, scaleMatrix({1.0f, 0.5f, 1.0f}), Color::purple, LogicType::XOR},
        {1, scaleMatrix({1.0f, 0.5f, 1.0f}), Color::magenta, LogicType::XNOR}
    };

    constexpr bool multiTile[9] = {false, true, false, false, false, false, false, false, false};

    constexpr uint32_t totalSizeVertices = sizeof(vertices);
    constexpr uint32_t totalSizeIndices = sizeof(indices);
}


