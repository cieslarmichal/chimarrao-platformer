#include "Animation.h"

#include "exceptions/TexturesNotInitialized.h"

namespace graphics::animation
{

Animation::Animation(std::vector<TexturePath> consecutiveTexturePathsInit, float timeBetweenTexturesInit)
    : consecutiveTexturePaths{std::move(consecutiveTexturePathsInit)},
      timeBetweenTextures{timeBetweenTexturesInit}
{
    if (consecutiveTexturePaths.empty())
    {
        throw exceptions::TexturesNotInitialized{"Textures not initialized"};
    }
    reset();
}

void Animation::update(const utils::DeltaTime& deltaTime)
{
    timeUntilNextTexture -= deltaTime.count();
    if (timeUntilNextTexture <= 0)
    {
        moveToNextTexture();
        timeUntilNextTexture = timeBetweenTextures;
    }
}

const TexturePath& Animation::getCurrentTexturePath() const
{
    return *currentTextureIter;
}

void Animation::moveToNextTexture()
{
    if (currentTextureIter == consecutiveTexturePaths.end() - 1)
    {
        currentTextureIter = consecutiveTexturePaths.begin();
    }
    else
    {
        currentTextureIter++;
    }
}
void Animation::reset()
{
    timeUntilNextTexture = timeBetweenTextures;
    currentTextureIter = consecutiveTexturePaths.begin();
}

}