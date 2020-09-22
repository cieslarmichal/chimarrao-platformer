#pragma once

#include "gmock/gmock.h"

#include "FontStorage.h"

namespace graphics
{
class FontStorageMock : public FontStorage
{
public:
    MOCK_METHOD(const sf::Font&, getFont, (const FontPath&));
};
}