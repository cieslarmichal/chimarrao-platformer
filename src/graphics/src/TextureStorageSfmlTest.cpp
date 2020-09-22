#include "TextureStorageSfml.h"

#include "gtest/gtest.h"

#include "GetProjectPath.h"
#include "exceptions/TextureNotAvailable.h"

using namespace graphics;
using namespace ::testing;

class TextureStorageSfmlTest : public ::testing::Test
{
public:
    const std::string testDirectory{utils::getProjectPath("chimarrao-platformer") +
                                    "src/graphics/src/testResources/"};
    const std::string nonExistingTexturePath{testDirectory + "nonExistingFile"};
    const std::string existingTexturePath{testDirectory + "attack-A1.png"};
    sf::Texture texture;

    TextureStorageSfml storage;
};

TEST_F(TextureStorageSfmlTest, getTextureWithExistingTexturePath_shouldNoThrow)
{
    ASSERT_NO_THROW(storage.getTexture(existingTexturePath));
}

TEST_F(TextureStorageSfmlTest, getTexture_shouldRememberLoadedTexture)
{
    const auto& texture1 = storage.getTexture(existingTexturePath);
    const auto& texture2 = storage.getTexture(existingTexturePath);

    ASSERT_EQ(&texture1, &texture2);
}

TEST_F(TextureStorageSfmlTest, getTextureWithNonExistingPath_shouldThrowTextureNotAvailable)
{
    ASSERT_THROW(storage.getTexture(nonExistingTexturePath), exceptions::TextureNotAvailable);
}