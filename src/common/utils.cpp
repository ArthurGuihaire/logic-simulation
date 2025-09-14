#include "component.hpp"
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
