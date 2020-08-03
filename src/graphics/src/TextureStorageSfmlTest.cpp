#include "TextureStorageSfml.h"

#include "gtest/gtest.h"

#include "TextureLoaderMock.h"

using namespace graphics;
using namespace ::testing;

namespace
{

}

class TextureStorageSfmlTest : public ::testing::Test
{
public:
    TexturePath goodPath = "~/chimarrao/resources/someTexture";
    TexturePath wrongPath = "C:\\wrong\\path";

    std::unique_ptr<TextureLoaderMock> loader = std::make_unique<NiceMock<TextureLoaderMock>>();
    TextureLoaderMock* loaderPtr = loader.get();

    sf::Texture texture{};

    std::unique_ptr<TextureStorageSfml> storage = std::make_unique<TextureStorageSfml>(std::move(loader));
};

TEST_F(TextureStorageSfmlTest, getTextureShouldReturnTextureOnGoodPath)
{

    EXPECT_CALL(*loaderPtr, load(_, goodPath)).WillOnce(DoAll(SetArgReferee<0>(texture), Return()));
    auto result = storage->getTexture(goodPath);
    ASSERT_TRUE(result);
}

TEST_F(TextureStorageSfmlTest, getTextureShouldRememberLoadedTexture)
{
    EXPECT_CALL(*loaderPtr, load(_, goodPath)).WillOnce(DoAll(SetArgReferee<0>(texture), Return()));
    auto result1 = storage->getTexture(goodPath);
    // no call on loader
    auto result2 = storage->getTexture(goodPath);
    ASSERT_EQ(&(result1.get()), &(result2.get()));
}

TEST_F(TextureStorageSfmlTest, getTextureShouldReturnBoostNoneOnWrongPath)
{
    EXPECT_CALL(*loaderPtr, load(_, wrongPath)).WillOnce(Throw(TextureLoader::CannotAccess{"test"}));
    auto result = storage->getTexture(wrongPath);
    ASSERT_FALSE(result);
}