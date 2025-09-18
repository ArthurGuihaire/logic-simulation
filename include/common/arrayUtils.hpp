#pragma once
#include <vector>
#include <componentSystem.hpp>

std::pair<bool, uint32_t> getFreeMemoryRegion(std::vector<std::pair<uint32_t, uint32_t>>& freeMemoryMap, const uint32_t requestedSize);
std::pair<bool, uint32_t> getFreeMemoryRegionExact(std::vector<std::pair<uint32_t, uint32_t>>& freeMemoryMap, const uint32_t requestedSize);

uint32_t findLastCommand(std::vector<GLsizei>& counts, std::vector<const void*> indices, uint32_t commandEnd);

uint32_t findContainingCommand(std::vector<GLsizei>& counts, std::vector<const void*> indices, uint32_t firstIndex, uint32_t numIndices);

std::pair<uint32_t, uint32_t> findEdgeCommands(std::vector<GLsizei>& counts, std::vector<const void*> indices, uint32_t edgeLocation, uint32_t edgeSize);
