#include "AnimatorSettingsYamlReader.h"

#include <iostream>

#include "exceptions/AnimatorsConfigFileNotFound.h"

namespace graphics::animation
{
namespace
{
const auto animatorsField{"animators"};
const auto nameField{"name"};
const auto animationsField{"animations"};
const auto animationTypeField{"type"};
const auto firstTexturePathField{"firstTexturePath"};
const auto numberOfTexturesField{"numberOfTextures"};
const auto timeBetweenTexturesInSecondsField{"timeBetweenTexturesInSeconds"};
}

boost::optional<AnimatorsSettings>
AnimatorSettingsYamlReader::readAnimatorsSettings(const utils::FilePath& yamlFilePath) const
{
    try
    {
        const auto animatorsConfig = loadYamlFile(yamlFilePath);
        if (const auto animators = animatorsConfig[animatorsField])
        {
            return readAnimatorsSettings(animators);
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Error while reading animators config file: " << e.what() << "\n";
        return boost::none;
    }

    return boost::none;
}

YAML::Node AnimatorSettingsYamlReader::loadYamlFile(const utils::FilePath& yamlFilePath) const
{
    try
    {
        return YAML::LoadFile(yamlFilePath);
    }
    catch (const YAML::BadFile& badFile)
    {
        throw exceptions::AnimatorsConfigFileNotFound{badFile.what()};
    }
}

boost::optional<AnimatorsSettings>
AnimatorSettingsYamlReader::readAnimatorsSettings(const YAML::Node& animators) const
{
    AnimatorsSettings animatorsSettings;

    for (const auto& animator : animators)
    {
        const auto animatorName = animator[nameField];
        const auto animations = animator[animationsField];

        if (not animatorName || not animations)
        {
            std::cerr << "Missing fields in animators\n";
            return boost::none;
        }
        if (const auto animationsSettings = readAnimationsSettings(animations))
        {
            animatorsSettings.push_back({animatorName.as<std::string>(), *animationsSettings});
        }
        else
        {
            return boost::none;
        }
    }
    return animatorsSettings;
}

boost::optional<AnimationsSettings>
AnimatorSettingsYamlReader::readAnimationsSettings(const YAML::Node& animations) const
{
    AnimationsSettings animationsSettings;

    for (const auto& animation : animations)
    {
        const auto type = animation[animationTypeField];
        const auto firstTexturePath = animation[firstTexturePathField];
        const auto numberOfTextures = animation[numberOfTexturesField];
        const auto timeBetweenTexturesInSeconds = animation[timeBetweenTexturesInSecondsField];

        if (not type || not firstTexturePath || not numberOfTextures || not timeBetweenTexturesInSeconds)
        {
            std::cerr << "Missing fields in animations\n";
            return boost::none;
        }
        animationsSettings.push_back({type.as<std::string>(), firstTexturePath.as<std::string>(),
                                      numberOfTextures.as<int>(), timeBetweenTexturesInSeconds.as<float>()});
    }

    return animationsSettings;
}

}