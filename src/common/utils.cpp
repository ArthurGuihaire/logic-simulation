#include <utils.hpp>
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
