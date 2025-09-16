#include <utils.hpp>
#include <arrayUtils.hpp>
#include <cstring> //For std::memcpy

std::pair<bool, uint32_t> getFreeMemoryRegion(std::vector<std::pair<uint32_t, uint32_t>>& freeMemoryMap, const uint32_t requestedSize) {
    bool freeMemoryExists = false;
    uint32_t firstFreeRegionIndex;
    for (std::pair<uint32_t, uint32_t>& freeMemory : freeMemoryMap) {
        if (freeMemory.second >= requestedSize) { //Is there a large enough memory gap?
            firstFreeRegionIndex = freeMemory.first;
            freeMemoryExists = true;

            if (freeMemory.second == requestedSize) { //Memory gap is just large enough?
                freeMemoryMap.erase(freeMemoryMap.begin() + freeMemory.first); //Erase the gap
                break; //Nothing else to be done, break early
            }
            //If program gets here, memory gap is larger than the required memory
            freeMemory.first += requestedSize; //Move index forward
            freeMemory.second -= requestedSize; //Decrease size accordingly
        }
    }
    return std::make_pair(freeMemoryExists, firstFreeRegionIndex);
}

std::pair<bool, uint32_t> getFreeMemoryRegionExact(std::vector<std::pair<uint32_t, uint32_t>>& freeMemoryMap, const uint32_t requestedSize) {
    for (std::pair<uint32_t, uint32_t>& freeMemory : freeMemoryMap) {
        if (freeMemory.second == requestedSize) { //Is there a large enough memory gap?
            if (freeMemory.second == requestedSize) { //Memory gap is just large enough?
                freeMemoryMap.erase(freeMemoryMap.begin() + freeMemory.first); //Erase the gap
                return std::make_pair(true, freeMemory.first); //Nothing else to be done, return early
            }
        }
    }
    return std::make_pair(false, 0);
}

unsigned int roundUpUInt(const unsigned int input, const unsigned int cutoff) {
    const unsigned int higherValue = input + cutoff - 1;
    const unsigned int bitmask = ~(cutoff - 1);
    return (higherValue & bitmask);
}

constexpr uint32_t floatBitmask = 0xFF;
bool almostEqualFast(float a, float b) {
    uint32_t aBytes, bBytes;
    std::memcpy(&aBytes, &a, sizeof(float));
    std::memcpy(&bBytes, &b, sizeof(float));
    aBytes |= floatBitmask;
    bBytes |= floatBitmask;
    return aBytes == bBytes;
}

bool almostEqual(float a, float b) {
    int32_t ia, ib;
    std::memcpy(&ia, &a, sizeof(float));
    std::memcpy(&ib, &b, sizeof(float));

    if (ia < 0) ia = 0x80000000 - ia;
    if (ib < 0) ib = 0x80000000 - ib;

    return std::abs(ia - ib) <= 8;
}

DrawElementsIndirectCommand* findLastCommand(std::vector<DrawElementsIndirectCommand>& commands, uint32_t commandEnd) {
    for (DrawElementsIndirectCommand& command : commands) {
        if (command.firstIndex + command.count == commandEnd)
            return &command;
    }
    return nullptr;
}

DrawElementsIndirectCommand* findContainingCommand(std::vector<DrawElementsIndirectCommand>& commands, uint32_t firstIndex, uint32_t numIndices) {
    for (DrawElementsIndirectCommand& command : commands) {
        if (command.firstIndex <= firstIndex && command.firstIndex + command.count >= firstIndex + numIndices) {
            return &command;
        }
    }
    return nullptr;
}

std::pair<DrawElementsIndirectCommand*, DrawElementsIndirectCommand*> findEdgeCommands(std::vector<DrawElementsIndirectCommand>& commands, uint32_t edgeLocation, uint32_t edgeSize) {
    DrawElementsIndirectCommand* firstCommand;
    DrawElementsIndirectCommand* secondCommand;
    bool temp = false;
    for (DrawElementsIndirectCommand& command : commands) {
        if (command.firstIndex + command.count == edgeLocation) {
            firstCommand = &command;
            if (temp) break;
            else temp = true;
        }
        else if (command.firstIndex == edgeLocation  + edgeSize) {
            secondCommand = &command;
            if (temp) break;
            else temp = true;
        }
    }
    return std::make_pair(firstCommand, secondCommand);
}

void printOpenGLErrors(const char* printString) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("%s: 0x%x\n", printString, err);
    }
}

void validateDrawSetup(GLsizei drawCount, GLsizei stride, GLenum indexType) {
    // --- 1. Indirect command buffer ---
    GLint64 indirectSize = 0;
    glGetBufferParameteri64v(GL_DRAW_INDIRECT_BUFFER, GL_BUFFER_SIZE, &indirectSize);
    if (indirectSize < drawCount * (stride ? stride : sizeof(DrawElementsIndirectCommand))) {
        std::cerr << "[ERROR] Indirect buffer too small (" << indirectSize << " bytes)" << std::endl;
    }

    // Map and dump first few commands
    if (indirectSize > 0) {
        auto* cmds = (const DrawElementsIndirectCommand*)glMapBufferRange(
            GL_DRAW_INDIRECT_BUFFER, 0,
            std::min<GLint64>(indirectSize, 10 * sizeof(DrawElementsIndirectCommand)),
            GL_MAP_READ_BIT
        );
        if (cmds) {
            for (int i = 0; i < std::min<GLsizei>(drawCount, 10); i++) {
                std::cout << "Command[" << i << "]: "
                          << "count=" << cmds[i].count
                          << " primCount=" << cmds[i].instanceCount
                          << " firstIndex=" << cmds[i].firstIndex
                          << " baseVertex=" << cmds[i].baseVertex
                          << " baseInstance=" << cmds[i].baseInstance
                          << std::endl;
            }
            glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
        }
    }

    // --- 2. Index buffer ---
    GLint elementArrayBuffer = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementArrayBuffer);
    if (elementArrayBuffer == 0) {
        std::cerr << "[ERROR] No index buffer bound!" << std::endl;
    } else {
        GLint64 indexBufSize = 0;
        glGetBufferParameteri64v(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexBufSize);
        std::cout << "Index buffer size: " << indexBufSize << " bytes" << std::endl;

        GLint bytesPerIndex = (indexType == GL_UNSIGNED_BYTE ? 1 :
                              (indexType == GL_UNSIGNED_SHORT ? 2 : 4));
        if (indexBufSize % bytesPerIndex != 0) {
            std::cerr << "[WARN] Index buffer size not divisible by index type size" << std::endl;
        }
    }

    // --- 3. Vertex attrib buffers ---
    GLint maxAttribs = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
    for (GLint i = 0; i < maxAttribs; i++) {
        GLint enabled = 0;
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        if (!enabled) continue;

        GLint buf = 0;
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &buf);
        GLint size = 0;
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
        GLint strideAttr = 0;
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &strideAttr);
        GLint type = 0;
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
        GLintptr offset = 0;
        glGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, (void**)&offset);

        std::cout << "Attrib " << i << ": buf=" << buf
                  << " size=" << size
                  << " stride=" << strideAttr
                  << " type=0x" << std::hex << type << std::dec
                  << " offset=" << offset
                  << std::endl;

        if (buf != 0) {
            GLint64 bufSize = 0;
            glBindBuffer(GL_ARRAY_BUFFER, buf);
            glGetBufferParameteri64v(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufSize);
            if (bufSize == 0) {
                std::cerr << "[ERROR] Attribute " << i << " bound to empty buffer" << std::endl;
            }
        }
    }
}

void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "[GLFW ERROR] code=%d msg=%s\n", error, description);
}

static bool detectIntelGPU() {
    const char* vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    if (vendor && std::string(vendor).find("Intel") != std::string::npos) {
        return true;
    }
    if (renderer && std::string(renderer).find("Intel") != std::string::npos) {
        return true;
    }
    return false;
}