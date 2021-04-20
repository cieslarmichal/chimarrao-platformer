#include "TextureLoader.h"

#include "gtest/gtest.h"

#include "ProjectPathReader.h"
#include "exceptions/CannotAccessTextureFile.h"

using namespace graphics;
using namespace ::testing;

class TextureLoaderTest : public Test
{
public:
    const std::string testDirectory{utils::ProjectPathReader::getProjectRootPath() +
                                    "src/graphics/src/testResources/"};
    const std::string nonExistingTexturePath{testDirectory + "nonExistingFile"};
    const std::string existingTexturePath{testDirectory + "attack-A1.png"};
    const TextureRect nonExistingTextureRect{nonExistingTexturePath, std::nullopt};
    const TextureRect existingTextureRectWithoutRectToCutTexture{existingTexturePath, std::nullopt};
    const TextureRect existingTextureRectWithRectToCutTexture{existingTexturePath,
                                                              utils::IntRect{5, 5, 5, 5}};
    sf::Texture texture;

    TextureLoader textureLoader;
};

TEST_F(TextureLoaderTest, givenNonExistingTextureRect_shouldThrowCannotAccess)
{
    ASSERT_THROW(textureLoader.load(texture, nonExistingTextureRect), exceptions::CannotAccessTextureFile);
}

TEST_F(TextureLoaderTest, givenExistingTextureRectWithoutRectToCut_shouldLoadTextureAndNotThrow)
{
    ASSERT_NO_THROW(textureLoader.load(texture, existingTextureRectWithoutRectToCutTexture));
}

TEST_F(TextureLoaderTest, givenExistingTextureRectWithRectToCut_shouldLoadTextureAndNotThrow)
{
    ASSERT_NO_THROW(textureLoader.load(texture, existingTextureRectWithRectToCutTexture));
}