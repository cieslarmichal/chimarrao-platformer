#include "BitMask.h"

#include <bitset>

#include "gtest/gtest.h"

using namespace utils;
using namespace ::testing;

class BitMaskTest : public Test
{
public:
    const uint32_t emptyMask{0};
    const uint32_t mask1{1};
    const uint32_t mask2{25};
    const uint32_t mask3{3};
    const std::bitset<32> bitSet1{"0001"};
    const std::bitset<32> bitSet2{"11001"};
    const unsigned outOfScopePosition{32};
};

TEST_F(BitMaskTest, createdDefaultMask_shouldBeEmpty)
{
    BitMask bitMask{};

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, emptyMask);
}

TEST_F(BitMaskTest, createdMaskWithInitializedByBitSet_shouldContainThisValue)
{
    BitMask bitMask{bitSet1};

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, mask1);
}

TEST_F(BitMaskTest, createdMaskWithInitializedByNumber_shouldContainThisValue)
{
    BitMask bitMask{mask2};

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, mask2);
}

TEST_F(BitMaskTest, shouldSetMaskByNumber)
{
    BitMask bitMask{};

    bitMask.setMask(mask3);

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, mask3);
}

TEST_F(BitMaskTest, shouldSetMaskByBitSet)
{
    BitMask bitMask{};

    bitMask.setMask(bitSet2);

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, mask2);
}

TEST_F(BitMaskTest, shouldClearMask)
{
    BitMask bitMask{bitSet1};

    bitMask.clear();

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, emptyMask);
}

TEST_F(BitMaskTest, givenPositionOutOfScope_shouldReturnFalse)
{
    BitMask bitMask{bitSet1};

    const auto isBitSet = bitMask.isBitSet(outOfScopePosition);

    ASSERT_FALSE(isBitSet);
}

TEST_F(BitMaskTest, initialSetOfBit_shouldReturnTrue)
{
    BitMask bitMask{bitSet1};

    const auto isBitSet = bitMask.isBitSet(0);

    ASSERT_TRUE(isBitSet);
}

TEST_F(BitMaskTest, initialResetOfBit_shouldReturnTrue)
{
    BitMask bitMask{};

    const auto isBitSet = bitMask.isBitSet(1);

    ASSERT_FALSE(isBitSet);
}

TEST_F(BitMaskTest, givenPositionOutOfScope_shouldNotSetBitOnThatPosition)
{
    BitMask bitMask{};

    bitMask.setBit(outOfScopePosition);

    const auto isBitSet = bitMask.isBitSet(outOfScopePosition);
    ASSERT_FALSE(isBitSet);
}

TEST_F(BitMaskTest, givenPosition_shouldSetBitOnThatPosition)
{
    BitMask bitMask{};

    bitMask.setBit(7);

    const auto isBitSet = bitMask.isBitSet(7);
    ASSERT_TRUE(isBitSet);
}

TEST_F(BitMaskTest, givenPosition_shouldResetBitOnThatPosition)
{
    BitMask bitMask{};
    bitMask.setBit(24);

    bitMask.resetBit(24);

    const auto isBitSet = bitMask.isBitSet(24);
    ASSERT_FALSE(isBitSet);
}

TEST_F(BitMaskTest, givenPositionOutOfScope_shouldNotResetBitOnThatPosition)
{
    BitMask bitMask{};

    bitMask.resetBit(outOfScopePosition);

    const auto isBitSet = bitMask.isBitSet(outOfScopePosition);
    ASSERT_FALSE(isBitSet);
}