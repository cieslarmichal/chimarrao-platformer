#include "AnimatorSettingsYamlReader.h"

#include <iostream>

#include "yaml-cpp/yaml.h"

#include "exceptions/AnimatorsConfigFileNotFound.h"
#include "exceptions/InvalidAnimatorsConfigFile.h"

namespace animations
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

AnimatorsSettings AnimatorSettingsYamlReader::readAnimatorsSettings(const utils::FilePath& yamlFilePath) const
{
    try
    {
        const auto animatorsConfig = loadYamlFile(yamlFilePath);
        if (not animatorsConfig[animatorsField])
        {
            auto errorMessage = "Missing animator field";
            std::cerr << errorMessage << "\n";
            throw exceptions::InvalidAnimatorsConfigFile{errorMessage};
        }
        return readAnimatorsSettings(animatorsConfig[animatorsField]);
    }
    catch (const YAML::Exception& e)
    {
        auto errorMessage = std::string{"Error while reading animators config file: "} + e.what();
        std::cerr << errorMessage << "\n";
        throw exceptions::InvalidAnimatorsConfigFile{errorMessage};
    }
}

YAML::Node AnimatorSettingsYamlReader::loadYamlFile(const utils::FilePath& yamlFilePath) const
{
    try
    {
        return YAML::LoadFile(yamlFilePath);
    }
    catch (const YAML::BadFile& badFile)
    {
        std::cerr << badFile.what() << "\n";
        throw exceptions::AnimatorsConfigFileNotFound{badFile.what()};
    }
}

AnimatorsSettings AnimatorSettingsYamlReader::readAnimatorsSettings(const YAML::Node& animators) const
{
    AnimatorsSettings animatorsSettings;

    for (const auto& animator : animators)
    {
        const auto animatorName = animator[nameField];
        const auto animations = animator[animationsField];

        if (not animatorName || not animations)
        {
            auto errorMessage = "Missing fields in animators";
            std::cerr << errorMessage << "\n";
            throw exceptions::InvalidAnimatorsConfigFile{errorMessage};
        }
        const auto animationsSettings = readAnimationsSettings(animations);
        animatorsSettings.push_back({animatorName.as<std::string>(), animationsSettings});
    }
    return animatorsSettings;
}

AnimationsSettings AnimatorSettingsYamlReader::readAnimationsSettings(const YAML::Node& animations) const
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
            auto errorMessage = "Missing fields in animations";
            std::cerr << errorMessage << "\n";
            throw exceptions::InvalidAnimatorsConfigFile{errorMessage};
        }
        animationsSettings.push_back({type.as<std::string>(), firstTexturePath.as<std::string>(),
                                      numberOfTextures.as<int>(), timeBetweenTexturesInSeconds.as<float>()});
    }

    return animationsSettings;
}

}