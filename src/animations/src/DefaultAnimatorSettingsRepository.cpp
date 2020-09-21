#include "DefaultAnimatorSettingsRepository.h"

#include "GetProjectPath.h"

namespace animations
{

const std::string DefaultAnimatorSettingsRepository::animatorSettingsFilePath{
    utils::getProjectPath("chimarrao-platformer") + "config/animators.yaml"};

DefaultAnimatorSettingsRepository::DefaultAnimatorSettingsRepository(
    std::unique_ptr<AnimatorSettingsReader> reader)
    : animatorSettingsReader{std::move(reader)}
{
    const auto animatorsSettingsAsVector =
        animatorSettingsReader->readAnimatorsSettings(animatorSettingsFilePath);
    settings.reserve(animatorsSettingsAsVector.size());

    for (const auto& animatorSettings : animatorsSettingsAsVector)
    {
        settings[animatorSettings.animatorName] = animatorSettings;
    }
}

boost::optional<AnimatorSettings>
DefaultAnimatorSettingsRepository::getAnimatorSettings(const std::string& animatorName) const
{
    const auto& animatorSettingsIter = settings.find(animatorName);
    if (animatorSettingsIter != settings.cend())
    {
        return animatorSettingsIter->second;
    }
    return boost::none;
}

}