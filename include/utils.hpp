#ifndef UTILS
#define UTILS
#include <vector>
#include <component.hpp>

template <typename integerType>
integerType roundUpInt(const integerType input, const uint32_t cutoff) {
    static_assert(std::is_integral<integerType>::value, "T must be an integer type!");
    const unsigned int higherValue = input + cutoff - 1;
    const unsigned int bitmask = ~cutoff;
    return (higherValue & bitmask);
}

// cutoff must be a power of 2
unsigned int roundUpUInt(const unsigned int input, const unsigned int cutoff);

bool almostEqual(float a, float b);

std::pair<bool, uint32_t> getFreeMemoryRegion(std::vector<std::pair<uint32_t, uint32_t>>& freeMemoryMap, const uint32_t requestedSize);
std::pair<bool, uint32_t> getFreeMemoryRegionExact(std::vector<std::pair<uint32_t, uint32_t>>& freeMemoryMap, const uint32_t requestedSize);

DrawElementsIndirectCommand* findLastCommand(std::vector<DrawElementsIndirectCommand>& commands, uint32_t firstIndex);

DrawElementsIndirectCommand* findContainingCommand(std::vector<DrawElementsIndirectCommand>& commands, uint32_t firstIndex, uint32_t numIndices);

std::pair<DrawElementsIndirectCommand*, DrawElementsIndirectCommand*> findEdgeCommands(std::vector<DrawElementsIndirectCommand>& commands, uint32_t edgeLocation, uint32_t edgeSize);

#endif
