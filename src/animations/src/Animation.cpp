#include "Animation.h"

#include "exceptions/TexturesNotInitialized.h"

namespace animations
{

Animation::Animation(std::vector<graphics::TextureRect> consecutiveTextureRectsInit,
                     float timeBetweenTexturesInit, bool loopsAllowed, bool interruptionAllowed)
    : consecutiveTextureRects{std::move(consecutiveTextureRectsInit)},
      numberOfTextureRects{consecutiveTextureRects.size()},
      timeBetweenTextures{timeBetweenTexturesInit},
      timeUntilNextTexture{timeBetweenTextures},
      loopsAllowed{loopsAllowed},
      interruptionAllowed{interruptionAllowed}
{
    if (consecutiveTextureRects.empty())
    {
        throw exceptions::TexturesNotInitialized{"Textures rects not initialized"};
    }
    reset();
}

TextureRectChanged Animation::update(const utils::DeltaTime& deltaTime)
{
    timeUntilNextTexture -= deltaTime.count();
    if (timeUntilNextTexture <= 0)
    {
        moveToNextTextureRect();
        timeUntilNextTexture = timeBetweenTextures;
        return true;
    }
    return false;
}

const graphics::TextureRect& Animation::getCurrentTextureRect() const
{
    return *currentTextureRectIter;
}

bool Animation::hasAnimationFinished()
{
    return (currentTextureRectIter == consecutiveTextureRects.end() - 1) && not loopsAllowed;
}

bool Animation::areInterruptionsAllowed()
{
    return interruptionAllowed;
}

void Animation::moveToNextTextureRect()
{
    if (currentTextureRectIter == consecutiveTextureRects.end() - 1)
    {
        if (loopsAllowed)
        {
            currentTextureRectIter = consecutiveTextureRects.begin();
        }
    }
    else
    {
        currentTextureRectIter++;
    }
}

void Animation::reset()
{
    timeUntilNextTexture = timeBetweenTextures;
    currentTextureRectIter = consecutiveTextureRects.begin();
}

unsigned int Animation::getProgressInPercents()
{
    const auto currentFrameNumber =
        std::distance(consecutiveTextureRects.begin(), currentTextureRectIter) + 1;
    return 100 * currentFrameNumber / numberOfTextureRects;
}

}