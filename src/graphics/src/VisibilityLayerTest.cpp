#include "VisibilityLayer.h"

#include "gtest/gtest.h"

#include "exceptions/VisibilityLayerNotFound.h"

using namespace graphics;
using namespace ::testing;

class VisibilityLayerTest : public Test
{
public:
    const VisibilityLayer visibilityLayer1 = VisibilityLayer::First;
    const VisibilityLayer visibilityLayer2 = VisibilityLayer::Second;
    const VisibilityLayer visibilityLayer3 = VisibilityLayer::Background;
    const std::string visibilityLayerString1 = "First";
    const std::string visibilityLayerString2 = "Second";
    const std::string visibilityLayerString3 = "Background";
    const std::string visibilityLayerDifferentSize = "FIRST";
    const std::string nonExistingVisibilityLayerString = "asdasdas";
};

TEST_F(VisibilityLayerTest, givenVisibilityLayer_shouldReturnString)
{
    ASSERT_EQ(toString(visibilityLayer1), visibilityLayerString1);
    ASSERT_EQ(toString(visibilityLayer2), visibilityLayerString2);
    ASSERT_EQ(toString(visibilityLayer3), visibilityLayerString3);
}

TEST_F(VisibilityLayerTest, givenStringNotCorrespondingToVisibilityLayer_shouldThrowVisibilityLayerNotFound)
{
    ASSERT_THROW(toVisibilityLayer(nonExistingVisibilityLayerString), exceptions::VisibilityLayerNotFound);
}

TEST_F(VisibilityLayerTest,
       givenStringCorrespondingToAnimationTypeWithDifferentCaseSize_shouldReturnAnimationType)
{
    ASSERT_EQ(toVisibilityLayer(visibilityLayerDifferentSize), visibilityLayer1);
}

TEST_F(VisibilityLayerTest, givenStringCorrespondingToAnimationType_shouldReturnAnimationType)
{
    ASSERT_EQ(toVisibilityLayer(visibilityLayerString1), visibilityLayer1);
    ASSERT_EQ(toVisibilityLayer(visibilityLayerString2), visibilityLayer2);
    ASSERT_EQ(toVisibilityLayer(visibilityLayerString3), visibilityLayer3);
}
