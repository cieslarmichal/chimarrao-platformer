#include "Bitmask.h"

namespace utils
{

Bitmask::Bitmask() : bits{0} {}

Bitmask::Bitmask(uint32_t maskInit) : bits{maskInit} {}

Bitmask::Bitmask(std::bitset<32> bitSetInit) : bits{static_cast<uint32_t>(bitSetInit.to_ulong())} {}

void Bitmask::setMask(uint32_t newMask)
{
    bits = newMask;
}

void Bitmask::setMask(std::bitset<32> newMask)
{
    bits = static_cast<uint32_t>(newMask.to_ulong());
}

uint32_t Bitmask::getMask() const
{
    return bits;
}

bool Bitmask::isBitSet(unsigned position) const
{
    if (positionIsInvalid(position))
    {
        return false;
    }
    return (bits & (1u << position)) != 0;
}

void Bitmask::setBit(unsigned position)
{
    if (positionIsInvalid(position))
    {
        return;
    }
    bits |= 1u << position;
}

void Bitmask::resetBit(unsigned position)
{
    if (positionIsInvalid(position))
    {
        return;
    }
    bits &= ~(1u << position);
}

void Bitmask::clear()
{
    bits = 0u;
}

bool Bitmask::positionIsInvalid(unsigned int position) const
{
    return position >= 32;
}

}