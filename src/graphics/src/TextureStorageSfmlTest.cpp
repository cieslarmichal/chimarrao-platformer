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
    const TextureRect nonExistingTextureRect{nonExistingTexturePath, std::nullopt};
    const TextureRect existingTextureRectWithoutRectToCutTexture{existingTexturePath, std::nullopt};
    const TextureRect existingTextureRectWithRectToCutTexture{existingTexturePath,
                                                              utils::IntRect{5, 5, 5, 5}};
    sf::Texture texture;

    TextureStorageSfml storage;
};

TEST_F(TextureStorageSfmlTest, getTextureWithNonExistingTextureRect_shouldThrowTextureNotAvailable)
{
    ASSERT_THROW(storage.getTexture(nonExistingTextureRect), exceptions::TextureNotAvailable);
}

TEST_F(TextureStorageSfmlTest, getTextureWithExistingTextureRect_shouldNoThrow)
{
    ASSERT_NO_THROW(storage.getTexture(existingTextureRectWithoutRectToCutTexture));
    ASSERT_NO_THROW(storage.getTexture(existingTextureRectWithRectToCutTexture));
}

TEST_F(TextureStorageSfmlTest, getTexture_shouldRememberLoadedTexture)
{
    const auto& texture1 = storage.getTexture(existingTextureRectWithRectToCutTexture);
    const auto& texture2 = storage.getTexture(existingTextureRectWithRectToCutTexture);

    ASSERT_EQ(&texture1, &texture2);
}

TEST_F(TextureStorageSfmlTest,
       getTextureTwiceWithTextureRectWithSameTexturePathButDifferentRectToCut_shouldNotReturnSameTexture)
{
    const auto& texture1 = storage.getTexture(existingTextureRectWithoutRectToCutTexture);
    const auto& texture2 = storage.getTexture(existingTextureRectWithRectToCutTexture);

    ASSERT_NE(&texture1, &texture2);
}
