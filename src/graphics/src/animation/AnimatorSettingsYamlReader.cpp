#include "AnimatorSettingsYamlReader.h"

#include <iostream>

#include "exceptions/AnimatorsConfigFileNotFound.h"

namespace graphics::animation
{

boost::optional<AnimatorsSettings>
AnimatorSettingsYamlReader::readAnimatorsSettings(const utils::FilePath& yamlFilePath) const
{
    try
    {
        const auto animatorsConfig = loadYamlFile(yamlFilePath);
        if (const auto animators = animatorsConfig["animators"])
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
        const auto animatorName = animator["name"];
        const auto animations = animator["animations"];

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
        const auto type = animation["type"];
        const auto firstTexturePath = animation["firstTexturePath"];
        const auto numberOfTextures = animation["numberOfTextures"];

        if (not type || not firstTexturePath || not numberOfTextures)
        {
            std::cerr << "Missing fields in animations\n";
            return boost::none;
        }
        animationsSettings.push_back(
            {type.as<std::string>(), firstTexturePath.as<std::string>(), numberOfTextures.as<int>()});
    }

    return animationsSettings;
}

}