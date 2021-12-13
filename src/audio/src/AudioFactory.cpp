#include "AudioFactory.h"

#include "DefaultAudioFactory.h"

namespace audio
{

std::unique_ptr<AudioFactory> AudioFactory::createAudioFactory()
{
    return std::make_unique<DefaultAudioFactory>();
}
}