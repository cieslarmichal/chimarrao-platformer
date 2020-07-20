#pragma once

#include <bitset>
#include <cstdint>

namespace utils
{
class BitMask
{
public:
    explicit BitMask();
    explicit BitMask(uint32_t);
    explicit BitMask(std::bitset<32>);

    void setMask(uint32_t);
    void setMask(std::bitset<32>);
    uint32_t getMask() const;
    bool isBitSet(unsigned position) const;
    void setBit(unsigned position);
    void resetBit(unsigned position);
    void clear();

private:
    bool positionIsInvalid(unsigned position) const;

    uint32_t bits;
};
}