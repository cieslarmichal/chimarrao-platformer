#pragma once

#include "gmock/gmock.h"

#include "TextureLoader.h"

namespace graphics
{
class TextureLoaderMock : public TextureLoader
{
public:
    MOCK_METHOD(void, load, (sf::Texture&, const TexturePath&), (const override));
};
}