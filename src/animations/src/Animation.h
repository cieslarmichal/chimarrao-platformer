#pragma once

#include <string>
#include <vector>

#include "DeltaTime.h"
#include "TextureRect.h"

namespace animations
{
using TextureRectChanged = bool;

class Animation
{
public:
    explicit Animation(std::vector<graphics::TextureRect> consecutiveTextureRects, float timeBetweenTextures,
                       bool loopsAllowed, bool interruptionAllowed);

    TextureRectChanged update(const utils::DeltaTime&);
    void reset();
    const graphics::TextureRect& getCurrentTextureRect() const;
    bool hasAnimationFinished();
    bool areInterruptionsAllowed();

private:
    void moveToNextTextureRect();

    std::vector<graphics::TextureRect> consecutiveTextureRects;
    std::vector<graphics::TextureRect>::iterator currentTextureRectIter;
    float timeBetweenTextures;
    float timeUntilNextTexture;
    bool loopsAllowed;
    bool interruptionAllowed;
};
}