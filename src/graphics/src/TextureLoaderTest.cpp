#include "TextureLoader.h"

#include "gtest/gtest.h"

#include "GetProjectPath.h"
#include "exceptions/CannotAccessTextureFile.h"

using namespace graphics;
using namespace ::testing;

class TextureLoaderTest : public Test
{
public:
    const std::string testDirectory{utils::getProjectPath("chimarrao-platformer") +
                                    "src/graphics/src/testResources/"};
    const std::string nonExistingTexturePath{testDirectory + "nonExistingFile"};
    const std::string existingTexturePath{testDirectory + "attack-A1.png"};
    sf::Texture texture;

    TextureLoader textureLoader;
};

TEST_F(TextureLoaderTest, givenNonExistingTexturePath_shouldThrowCannotAccess)
{
    ASSERT_THROW(textureLoader.load(texture, nonExistingTexturePath), exceptions::CannotAccessTextureFile);
}

TEST_F(TextureLoaderTest, givenExistingTexturePath_shouldLoadTextureAndNotThrow)
{
    ASSERT_NO_THROW(textureLoader.load(texture, existingTexturePath));
}