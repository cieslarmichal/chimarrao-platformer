#pragma once

#include <bitset>
#include <cstdint>

#include "UtilsApi.h"

namespace utils
{
class Bitmask
{
public:
    UTILS_API explicit Bitmask();
    UTILS_API explicit Bitmask(uint32_t);
    UTILS_API explicit Bitmask(std::bitset<32>);

    UTILS_API void setMask(uint32_t);
    UTILS_API void setMask(std::bitset<32>);
    UTILS_API uint32_t getMask() const;
    UTILS_API bool isBitSet(unsigned position) const;
    UTILS_API void setBit(unsigned position);
    UTILS_API void resetBit(unsigned position);
    UTILS_API void clear();

private:
    bool positionIsInvalid(unsigned position) const;

    uint32_t bits;
};
}