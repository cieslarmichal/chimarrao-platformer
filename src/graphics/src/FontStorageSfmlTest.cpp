#include "FontStorageSfml.h"

#include "gtest/gtest.h"

#include "ProjectPathReader.h"
#include "exceptions/FontNotAvailable.h"

using namespace graphics;
using namespace ::testing;

class FontStorageSfmlTest : public Test
{
public:
    const std::string testDirectory{utils::ProjectPathReader::getProjectRootPath() +
                                    "src/graphics/src/testResources/"};
    const std::string nonExistingFontPath{testDirectory + "nonExistingFile"};
    const std::string existingFontPath{testDirectory + "VeraMono.ttf"};
    sf::Font font;

    FontStorageSfml storage;
};

TEST_F(FontStorageSfmlTest, getFontWithExistingFontPath_shouldNotThrow)
{
    ASSERT_NO_THROW(storage.getFont(existingFontPath));
}

TEST_F(FontStorageSfmlTest, getFont_shouldRememberLoadedFont)
{
    const auto& font1 = storage.getFont(existingFontPath);
    const auto& font2 = storage.getFont(existingFontPath);

    ASSERT_EQ(&font1, &font2);
}

TEST_F(FontStorageSfmlTest, getFontWithNonExistingPath_shouldThrowFontNotAvailable)
{
    ASSERT_THROW(storage.getFont(nonExistingFontPath), exceptions::FontNotAvailable);
}