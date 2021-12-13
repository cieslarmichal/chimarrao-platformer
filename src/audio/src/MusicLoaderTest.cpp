#include "MusicLoader.h"

#include "gtest/gtest.h"

#include "ProjectPathReader.h"
#include "exceptions/CannotAccessMusicFile.h"

using namespace audio;
using namespace ::testing;

class MusicLoaderTest : public Test
{
public:
    const std::string testDirectory{utils::ProjectPathReader::getProjectRootPath() +
                                    "src/audio/src/testResources/"};
    const std::string nonExistingMusicPath{testDirectory + "nonExistingFile"};
    const std::string existingMusicPath{testDirectory + "test_music.wav"};
    sf::Music music;

    MusicLoader musicLoader;
};

TEST_F(MusicLoaderTest, givenNonExistingMusicPath_shouldThrowCannotAccess)
{
    ASSERT_THROW(musicLoader.load(music, nonExistingMusicPath), exceptions::CannotAccessMusicFile);
}

TEST_F(MusicLoaderTest, givenExistingMusicPath_shouldLoadMusicAndNotThrow)
{
    ASSERT_NO_THROW(musicLoader.load(music, existingMusicPath));
}