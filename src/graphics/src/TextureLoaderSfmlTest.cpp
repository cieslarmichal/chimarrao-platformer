#include "TextureLoaderSfml.h"

#include "exceptions/CannotAccess.h"
#include "gtest/gtest.h"
#include "GetProjectPath.h"

using namespace graphics;
using namespace ::testing;

class TextureLoaderSfmlTest : public Test
{
public:
    const std::string testDirectory{utils::getProjectPath("chimarrao") + "src/graphics/src/testTexture/"};
    const std::string nonExistingTexturePath{testDirectory + "nonExistingFile"};
    const std::string existingTexturePath{testDirectory + "attack-A1.png"};
    sf::Texture texture;

    TextureLoaderSfml textureLoader;
};

TEST_F(TextureLoaderSfmlTest, givenNonExistingTexturePath_shouldThrowCannotAccess)
{
    ASSERT_THROW(textureLoader.load(texture, nonExistingTexturePath), exceptions::CannotAccess);
}

TEST_F(TextureLoaderSfmlTest, givenExistingTexturePath_shouldLoadTextureAndNotThrow)
{
    ASSERT_NO_THROW(textureLoader.load(texture, existingTexturePath));
}