#pragma once

#include <unordered_map>

#include "FontStorage.h"

namespace graphics
{
class FontStorageSfml : public FontStorage
{
public:
    const sf::Font& getFont(const FontPath&) override;

private:
    void loadFont(const FontPath& path);
    bool fontInStorage(const FontPath& path);

    std::unordered_map<FontPath, std::unique_ptr<sf::Font>> fonts;
};
}