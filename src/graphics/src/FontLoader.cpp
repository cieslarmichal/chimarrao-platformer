#include "FontLoader.h"

#include "exceptions/CannotAccessFontFile.h"

namespace graphics
{

void FontLoader::load(sf::Font& font, const FontPath& fontPath)
{
    if (not font.loadFromFile(fontPath))
    {
        throw exceptions::CannotAccessFontFile("Cannot load font: " + fontPath);
    }
}
}