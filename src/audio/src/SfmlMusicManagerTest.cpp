#include "SfmlMusicManager.h"

#include "gtest/gtest.h"

#include "MusicStorageMock.h"

#include "MusicIdGenerator.h"
#include "ProjectPathReader.h"
#include "exceptions/MusicNotFound.h"

using namespace audio;
using namespace ::testing;

class SfmlMusicManagerTest : public Test
{
public:
    const std::string testDirectory{utils::ProjectPathReader::getProjectRootPath() +
                                    "src/audio/src/testResources/"};
    const std::string nonExistingMusicPath{testDirectory + "nonExistingFile"};
    const std::string existingMusicPath{testDirectory + "test_music.wav"};
    sf::Music music;
    const MusicId nonExistingId = MusicIdGenerator::generateId();

    std::unique_ptr<StrictMock<MusicStorageMock>> musicStorageInit =
        std::make_unique<StrictMock<MusicStorageMock>>();
    StrictMock<MusicStorageMock>* musicStorage = musicStorageInit.get();
    SfmlMusicManager musicManager{std::move(musicStorageInit)};
};

TEST_F(SfmlMusicManagerTest, aquireMusic_shoulReturnIdAndNotThrow)
{
    ASSERT_NO_THROW(musicManager.acquire(existingMusicPath));
}

TEST_F(SfmlMusicManagerTest, release_givenNonExistingMusicId_shouldThrowMusicNotFound)
{
    ASSERT_THROW(musicManager.release(nonExistingId), exceptions::MusicNotFound);
}

TEST_F(SfmlMusicManagerTest, release_givenExistingMusicId_shouldNoThrow)
{
    EXPECT_CALL(*musicStorage, getMusic(existingMusicPath)).WillRepeatedly(ReturnRef(music));
    const auto musicId = musicManager.acquire(existingMusicPath);

    ASSERT_NO_THROW(musicManager.release(musicId));
}

TEST_F(SfmlMusicManagerTest, play_givenNonExistingMusicId_shouldThrowMusicNotFound)
{
    ASSERT_THROW(musicManager.play(nonExistingId), exceptions::MusicNotFound);
}

TEST_F(SfmlMusicManagerTest, play_givenExistingMusicId_shouldNoThrow)
{
    EXPECT_CALL(*musicStorage, getMusic(existingMusicPath)).WillRepeatedly(ReturnRef(music));
    const auto musicId = musicManager.acquire(existingMusicPath);

    ASSERT_NO_THROW(musicManager.play(musicId));
}

TEST_F(SfmlMusicManagerTest, pause_givenNonExistingMusicId_shouldThrowMusicNotFound)
{
    ASSERT_THROW(musicManager.pause(nonExistingId), exceptions::MusicNotFound);
}

TEST_F(SfmlMusicManagerTest, pause_givenExistingMusicId_shouldNoThrow)
{
    EXPECT_CALL(*musicStorage, getMusic(existingMusicPath)).WillRepeatedly(ReturnRef(music));
    const auto musicId = musicManager.acquire(existingMusicPath);

    ASSERT_NO_THROW(musicManager.pause(musicId));
}

TEST_F(SfmlMusicManagerTest, stop_givenNonExistingMusicId_shouldThrowMusicNotFound)
{
    ASSERT_THROW(musicManager.stop(nonExistingId), exceptions::MusicNotFound);
}

TEST_F(SfmlMusicManagerTest, stop_givenExistingMusicId_shouldNoThrow)
{
    EXPECT_CALL(*musicStorage, getMusic(existingMusicPath)).WillRepeatedly(ReturnRef(music));
    const auto musicId = musicManager.acquire(existingMusicPath);

    ASSERT_NO_THROW(musicManager.stop(musicId));
}
