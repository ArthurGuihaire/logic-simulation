#ifndef UTILS
#define UTILS
#include <componentStructs.hpp>

template <typename integerType>
integerType roundUpInt(const integerType input, const uint32_t cutoff) {
    static_assert(std::is_integral<integerType>::value, "T must be an integer type!");
    const unsigned int higherValue = input + cutoff - 1;
    const unsigned int bitmask = ~(cutoff - 1);
    return (higherValue & bitmask);
}

// cutoff must be a power of 2
unsigned int roundUpUInt(const unsigned int input, const unsigned int cutoff);

bool almostEqual(float a, float b);

void printOpenGLErrors(const char* printString);

void validateDrawSetup(GLsizei drawCount, GLsizei stride, GLenum indexType);



void glfwErrorCallback(int error, const char* description);

#endif
