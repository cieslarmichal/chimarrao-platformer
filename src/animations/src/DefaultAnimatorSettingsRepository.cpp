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
    const auto animatorsSettings = animatorSettingsReader->readAnimatorsSettings(animatorSettingsFilePath);
    singleFileAnimatorSettings.reserve(animatorsSettings.singleFileAnimatorsSettings.size());
    multipleFilesAnimatorSettings.reserve(animatorsSettings.multipleFilesAnimatorSettings.size());

    for (const auto& animatorSettings : animatorsSettings.multipleFilesAnimatorSettings)
    {
        multipleFilesAnimatorSettings[animatorSettings.animatorName] = animatorSettings;
    }

    for (const auto& animatorSettings : animatorsSettings.singleFileAnimatorsSettings)
    {
        singleFileAnimatorSettings[animatorSettings.animatorName] = animatorSettings;
    }
}

boost::optional<SingleFileAnimatorSettings>
DefaultAnimatorSettingsRepository::getSingleFileAnimatorSettings(const std::string& animatorName) const
{
    const auto& animatorSettingsIter = singleFileAnimatorSettings.find(animatorName);
    if (animatorSettingsIter != singleFileAnimatorSettings.cend())
    {
        return animatorSettingsIter->second;
    }
    return boost::none;
}

boost::optional<MultipleFilesAnimatorSettings>
DefaultAnimatorSettingsRepository::getMultipleFileAnimatorSettings(const std::string& animatorName) const
{
    const auto& animatorSettingsIter = multipleFilesAnimatorSettings.find(animatorName);
    if (animatorSettingsIter != multipleFilesAnimatorSettings.cend())
    {
        return animatorSettingsIter->second;
    }
    return boost::none;
}

}