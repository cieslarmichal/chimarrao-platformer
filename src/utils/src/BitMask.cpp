#include "BitMask.h"

namespace utils
{

BitMask::BitMask() : bits{0} {}

BitMask::BitMask(uint32_t maskInit) : bits{maskInit} {}

BitMask::BitMask(std::bitset<32> bitSetInit) : bits{static_cast<uint32_t>(bitSetInit.to_ulong())} {}

void BitMask::setMask(uint32_t newMask)
{
    bits = newMask;
}

void BitMask::setMask(std::bitset<32> newMask)
{
    bits = static_cast<uint32_t>(newMask.to_ulong());
}

uint32_t BitMask::getMask() const
{
    return bits;
}

bool BitMask::isBitSet(unsigned position) const
{
    if (positionIsInvalid(position))
    {
        return false;
    }
    return (bits & (1u << position)) != 0;
}

void BitMask::setBit(unsigned position)
{
    if (positionIsInvalid(position))
    {
        return;
    }
    bits |= 1u << position;
}

void BitMask::resetBit(unsigned position)
{
    if (positionIsInvalid(position))
    {
        return;
    }
    bits &= ~(1u << position);
}

void BitMask::clear()
{
    bits = 0u;
}

bool BitMask::positionIsInvalid(unsigned int position) const
{
    return position >= 32;
}

bool operator==(const BitMask& lhs, const BitMask& rhs)
{
    return lhs.getMask() == rhs.getMask();
}

}