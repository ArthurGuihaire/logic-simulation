#ifndef ARRAY_UTILS
#define ARRAY_UTILS
#include <vector>
#include <componentSystem.hpp>

std::pair<bool, uint32_t> getFreeMemoryRegion(std::vector<std::pair<uint32_t, uint32_t>>& freeMemoryMap, const uint32_t requestedSize);
std::pair<bool, uint32_t> getFreeMemoryRegionExact(std::vector<std::pair<uint32_t, uint32_t>>& freeMemoryMap, const uint32_t requestedSize);

DrawElementsIndirectCommand* findLastCommand(std::vector<DrawElementsIndirectCommand>& commands, uint32_t firstIndex);

DrawElementsIndirectCommand* findContainingCommand(std::vector<DrawElementsIndirectCommand>& commands, uint32_t firstIndex, uint32_t numIndices);

std::pair<DrawElementsIndirectCommand*, DrawElementsIndirectCommand*> findEdgeCommands(std::vector<DrawElementsIndirectCommand>& commands, uint32_t edgeLocation, uint32_t edgeSize);

#endif