#ifndef UTILS
#define UTILS

template <typename integerType>
integerType roundUpInt(const integerType input, const uint32_t cutoff) {
    static_assert(std::is_integral<integerType>::value, "T must be an integer type!");
    const unsigned int higherValue = input + cutoff - 1;
    const unsigned int bitmask = ~cutoff;
    return (higherValue & bitmask);
}
#endif
