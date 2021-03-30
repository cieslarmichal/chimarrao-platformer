#include "Animation.h"

#include "exceptions/TexturesNotInitialized.h"

namespace animations
{

Animation::Animation(std::vector<graphics::TextureRect> consecutiveTextureRectsInit,
                     float timeBetweenTexturesInit)
    : consecutiveTextureRects{std::move(consecutiveTextureRectsInit)},
      timeBetweenTextures{timeBetweenTexturesInit}
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

void Animation::moveToNextTextureRect()
{
    if (currentTextureRectIter == consecutiveTextureRects.end() - 1)
    {
        currentTextureRectIter = consecutiveTextureRects.begin();
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

}