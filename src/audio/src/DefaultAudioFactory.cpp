#include "DefaultAudioFactory.h"

#include "SfmlMusicManager.h"
#include "SfmlMusicStorage.h"

namespace audio
{
std::unique_ptr<MusicManager> DefaultAudioFactory::createMusicManager() const
{
    return std::make_unique<SfmlMusicManager>(std::make_unique<SfmlMusicStorage>());
}
}