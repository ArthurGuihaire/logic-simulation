#pragma once
#include <componentStructs.hpp>

template <typename integerType>
constexpr integerType roundUpInt(const integerType input, const uint32_t cutoff) {
    static_assert(std::is_integral<integerType>::value, "T must be an integer type!");
    const unsigned int higherValue = input + cutoff - 1;
    const unsigned int bitmask = ~(cutoff - 1);
    return (higherValue & bitmask);
}

namespace std {
    template<>
    struct hash<glm::ivec3> {
        int operator()(const glm::ivec3& vec) const noexcept {
            int h1 = std::hash<int>()(vec.x);
            int h2 = std::hash<int>()(vec.y);
            int h3 = std::hash<int>()(vec.z);

            return (h1 ^ (h2<<1) ^ (h3<<2));
        }
    };
}

// cutoff must be a power of 2
constexpr unsigned int roundUpUInt(const unsigned int input, const unsigned int cutoff);

bool almostEqual(float a, float b);

void printOpenGLErrors(const char* printString);

void validateDrawSetup(GLsizei drawCount, GLsizei stride, GLenum indexType);

void glfwErrorCallback(int error, const char* description);

bool detectIntelGPU();

bool componentExists(glm::ivec3 position);

constexpr glm::mat4 scaleMatrix(const glm::vec3 scaleFactor) {
    return glm::mat4(scaleFactor.x, 0.0f, 0.0f, 0.0f, 0.0f, scaleFactor.y, 0.0f, 0.0f, 0.0f, 0.0f, scaleFactor.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}
