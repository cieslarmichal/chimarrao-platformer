#include "FontStorageSfml.h"

#include <iostream>

#include "FontLoader.h"
#include "exceptions/CannotAccessFontFile.h"
#include "exceptions/FontNotAvailable.h"

namespace graphics
{

const sf::Font& FontStorageSfml::getFont(const FontPath& path)
{
    if (not fontInStorage(path))
    {
        loadFont(path);
    }
    return *fonts.at(path);
}

void FontStorageSfml::loadFont(const FontPath& path)
{
    auto font = std::make_unique<sf::Font>();
    try
    {
        FontLoader::load(*font, path);
    }
    catch (const exceptions::CannotAccessFontFile& e)
    {
        std::cerr << e.what() << std::endl;
        throw exceptions::FontNotAvailable{e.what()};
    }
    fonts[path] = std::move(font);
}

bool FontStorageSfml::fontInStorage(const FontPath& path)
{
    return fonts.count(path) == 1;
}

}