#include "FontLoader.h"

#include "gtest/gtest.h"

#include "GetProjectPath.h"
#include "exceptions/CannotAccessFontFile.h"

using namespace graphics;
using namespace ::testing;

class FontLoaderTest : public Test
{
public:
    const std::string testDirectory{utils::getProjectPath("chimarrao-platformer") +
                                    "src/graphics/src/testResources/"};
    const std::string nonExistingFontPath{testDirectory + "nonExistingFile"};
    const std::string existingFontPath{testDirectory + "VeraMono.ttf"};
    sf::Font font;

    FontLoader fontLoader;
};

TEST_F(FontLoaderTest, givenNonExistingFontPath_shouldThrowCannotAccess)
{
    ASSERT_THROW(fontLoader.load(font, nonExistingFontPath), exceptions::CannotAccessFontFile);
}

TEST_F(FontLoaderTest, givenExistingFontPath_shouldLoadFontAndNotThrow)
{
    ASSERT_NO_THROW(fontLoader.load(font, existingFontPath));
}