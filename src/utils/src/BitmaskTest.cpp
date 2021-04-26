#include "Bitmask.h"

#include "gtest/gtest.h"

using namespace utils;
using namespace ::testing;

class BitmaskTest : public Test
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

TEST_F(BitmaskTest, createdDefaultMask_shouldBeEmpty)
{
    Bitmask bitMask{};

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, emptyMask);
}

TEST_F(BitmaskTest, createdMaskWithInitializedByBitSet_shouldContainThisValue)
{
    Bitmask bitMask{bitSet1};

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, mask1);
}

TEST_F(BitmaskTest, createdMaskWithInitializedByNumber_shouldContainThisValue)
{
    Bitmask bitMask{mask2};

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, mask2);
}

TEST_F(BitmaskTest, shouldSetMaskByNumber)
{
    Bitmask bitMask{};

    bitMask.setMask(mask3);

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, mask3);
}

TEST_F(BitmaskTest, shouldSetMaskByBitSet)
{
    Bitmask bitMask{};

    bitMask.setMask(bitSet2);

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, mask2);
}

TEST_F(BitmaskTest, shouldClearMask)
{
    Bitmask bitMask{bitSet1};

    bitMask.clear();

    const auto currentBitMask = bitMask.getMask();
    ASSERT_EQ(currentBitMask, emptyMask);
}

TEST_F(BitmaskTest, givenPositionOutOfScope_shouldReturnFalse)
{
    Bitmask bitMask{bitSet1};

    const auto isBitSet = bitMask.isBitSet(outOfScopePosition);

    ASSERT_FALSE(isBitSet);
}

TEST_F(BitmaskTest, initialSetOfBit_shouldReturnTrue)
{
    Bitmask bitMask{bitSet1};

    const auto isBitSet = bitMask.isBitSet(0);

    ASSERT_TRUE(isBitSet);
}

TEST_F(BitmaskTest, initialResetOfBit_shouldReturnTrue)
{
    Bitmask bitMask{};

    const auto isBitSet = bitMask.isBitSet(1);

    ASSERT_FALSE(isBitSet);
}

TEST_F(BitmaskTest, givenPositionOutOfScope_shouldNotSetBitOnThatPosition)
{
    Bitmask bitMask{};

    bitMask.setBit(outOfScopePosition);

    const auto isBitSet = bitMask.isBitSet(outOfScopePosition);
    ASSERT_FALSE(isBitSet);
}

TEST_F(BitmaskTest, givenPosition_shouldSetBitOnThatPosition)
{
    Bitmask bitMask{};

    bitMask.setBit(7);

    const auto isBitSet = bitMask.isBitSet(7);
    ASSERT_TRUE(isBitSet);
}

TEST_F(BitmaskTest, givenPosition_shouldResetBitOnThatPosition)
{
    Bitmask bitMask{};
    bitMask.setBit(24);

    bitMask.resetBit(24);

    const auto isBitSet = bitMask.isBitSet(24);
    ASSERT_FALSE(isBitSet);
}

TEST_F(BitmaskTest, givenPositionOutOfScope_shouldNotResetBitOnThatPosition)
{
    Bitmask bitMask{};

    bitMask.resetBit(outOfScopePosition);

    const auto isBitSet = bitMask.isBitSet(outOfScopePosition);
    ASSERT_FALSE(isBitSet);
}