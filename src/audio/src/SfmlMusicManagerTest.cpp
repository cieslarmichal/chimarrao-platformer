#include "SfmlMusicStorage.h"

#include "gtest/gtest.h"

#include "ProjectPathReader.h"
#include "exceptions/MusicNotAvailable.h"

using namespace audio;
using namespace ::testing;

class SfmlMusicStorageTest : public Test
{
public:
    const std::string testDirectory{utils::ProjectPathReader::getProjectRootPath() +
                                    "src/audio/src/testResources/"};
    const std::string nonExistingMusicPath{testDirectory + "nonExistingFile"};
    const std::string existingMusicPath{testDirectory + "test_music.wav"};
    sf::Music music;

    SfmlMusicStorage storage;
};

TEST_F(SfmlMusicStorageTest, getMusicWithExistingMusicPath_shouldNotThrow)
{
    ASSERT_NO_THROW(storage.getMusic(existingMusicPath));
}

TEST_F(SfmlMusicStorageTest, getMusic_shouldRememberLoadedMusic)
{
    const auto& font1 = storage.getMusic(existingMusicPath);
    const auto& font2 = storage.getMusic(existingMusicPath);

    ASSERT_EQ(&font1, &font2);
}

TEST_F(SfmlMusicStorageTest, getMusicWithNonExistingPath_shouldThrowMusicNotAvailable)
{
    ASSERT_THROW(storage.getMusic(nonExistingMusicPath), exceptions::MusicNotAvailable);
}