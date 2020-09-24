#include "Animation.h"

#include "exceptions/TexturesNotInitialized.h"

namespace animations
{

Animation::Animation(std::vector<graphics::TexturePath> consecutiveTexturePathsInit,
                     float timeBetweenTexturesInit)
    : consecutiveTexturePaths{std::move(consecutiveTexturePathsInit)},
      timeBetweenTextures{timeBetweenTexturesInit}
{
    if (consecutiveTexturePaths.empty())
    {
        throw exceptions::TexturesNotInitialized{"Textures not initialized"};
    }
    reset();
}

TextureChanged Animation::update(const utils::DeltaTime& deltaTime)
{
    timeUntilNextTexture -= deltaTime.count();
    if (timeUntilNextTexture <= 0)
    {
        moveToNextTexture();
        timeUntilNextTexture = timeBetweenTextures;
        return true;
    }
    return false;
}

const graphics::TexturePath& Animation::getCurrentTexturePath() const
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