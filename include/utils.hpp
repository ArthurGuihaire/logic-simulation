#ifndef UTILS
#define UTILS
#include <vector>
#include <inttypes.h>
std::pair<bool, uint32_t> getFreeMemoryRegion(std::vector<std::pair<uint32_t, uint32_t>>&, uint32_t requestedSize);
#endif