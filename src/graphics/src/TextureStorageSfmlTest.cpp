#include "TextureStorageSfml.h"

#include "gtest/gtest.h"

#include "TextureLoaderMock.h"

#include "exceptions/CannotAccessTextureFile.h"
#include "exceptions/TextureNotAvailable.h"

using namespace graphics;
using namespace ::testing;

class TextureStorageSfmlTest : public ::testing::Test
{
public:
    TexturePath goodPath = "~/chimarrao/resources/someTexture";
    TexturePath wrongPath = "C:\\wrong\\path";

    std::unique_ptr<TextureLoaderMock> loaderInit = std::make_unique<StrictMock<TextureLoaderMock>>();
    TextureLoaderMock* loader = loaderInit.get();
    std::unique_ptr<TextureStorageSfml> storage = std::make_unique<TextureStorageSfml>(std::move(loaderInit));
};

TEST_F(TextureStorageSfmlTest, getTextureShouldNoThrow)
{
    EXPECT_CALL(*loader, load(_, goodPath)).WillOnce(Return());

    ASSERT_NO_THROW(storage->getTexture(goodPath));
}

TEST_F(TextureStorageSfmlTest, getTexture_shouldRememberLoadedTexture)
{
    EXPECT_CALL(*loader, load(_, goodPath)).WillOnce(Return());

    const auto& result1 = storage->getTexture(goodPath);
    const auto& result2 = storage->getTexture(goodPath);

    ASSERT_EQ(&result1, &result2);
}

TEST_F(TextureStorageSfmlTest, getTextureWithNonExistingPath_shouldThrowCannotAccessTextureFile)
{
    EXPECT_CALL(*loader, load(_, wrongPath)).WillOnce(Throw(exceptions::CannotAccessTextureFile{""}));

    ASSERT_THROW(storage->getTexture(wrongPath), exceptions::TextureNotAvailable);
}