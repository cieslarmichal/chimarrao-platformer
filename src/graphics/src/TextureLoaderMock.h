#pragma once

#include "gmock/gmock.h"

#include "TextureLoader.h"

namespace graphics
{
class TextureLoaderMock : public TextureLoader
{
public:
    virtual ~TextureLoaderMock() = default;

    MOCK_METHOD(void, load, (sf::Texture&, const TexturePath&));
};
}