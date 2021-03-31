#include "AnimatorSettingsYamlReader.h"

#include <iostream>

#include "yaml-cpp/yaml.h"

#include "exceptions/AnimatorsConfigFileNotFound.h"
#include "exceptions/InvalidAnimatorsConfigFile.h"

namespace animations
{
namespace
{
const auto multipleFilesAnimatorsField{"multipleFilesAnimators"};
const auto singleFilesAnimatorsField{"singleFileAnimators"};
const auto nameField{"name"};
const auto animationsField{"animations"};
const auto animationTypeField{"type"};
const auto firstTexturePathField{"firstTexturePath"};
const auto numberOfTexturesField{"numberOfTextures"};
const auto timeBetweenTexturesInSecondsField{"timeBetweenTexturesInSeconds"};
const auto texturePathField{"texturePath"};
const auto textureWidthInPixelsField{"textureWidthInPixels"};
const auto textureHeightInPixelsField{"textureHeightInPixels"};
const auto rectToCutWidthInPixelsField{"rectToCutWidthInPixels"};
const auto rectToCutHeightInPixelsField{"rectToCutHeightInPixels"};
const auto startXPositionField{"startXPosition"};
const auto startYPositionField{"startYPosition"};
}

AnimatorsSettings AnimatorSettingsYamlReader::readAnimatorsSettings(const utils::FilePath& yamlFilePath) const
{
    try
    {
        const auto animatorsConfig = loadYamlFile(yamlFilePath);
        if (not animatorsConfig[multipleFilesAnimatorsField] and
            not animatorsConfig[singleFilesAnimatorsField])
        {
            auto errorMessage = "Missing multiple files or single file animators field";
            std::cerr << errorMessage << "\n";
            throw exceptions::InvalidAnimatorsConfigFile{errorMessage};
        }

        AnimatorsSettings animatorsSettings{};
        if (animatorsConfig[singleFilesAnimatorsField])
        {
            animatorsSettings.singleFileAnimatorsSettings =
                readSingleFileAnimatorsSettings(animatorsConfig[singleFilesAnimatorsField]);
        }

        if (animatorsConfig[multipleFilesAnimatorsField])
        {
            animatorsSettings.multipleFilesAnimatorSettings =
                readMultipleFilesAnimatorsSettings(animatorsConfig[multipleFilesAnimatorsField]);
        }

        return animatorsSettings;
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

std::vector<MultipleFilesAnimatorSettings>
AnimatorSettingsYamlReader::readMultipleFilesAnimatorsSettings(const YAML::Node& animators) const
{
    std::vector<MultipleFilesAnimatorSettings> animatorsSettings;

    for (const auto& animator : animators)
    {
        const auto animatorName = animator[nameField];
        const auto animations = animator[animationsField];

        if (not animatorName || not animations)
        {
            auto errorMessage = "Missing fields in multiple files animators";
            std::cerr << errorMessage << "\n";
            throw exceptions::InvalidAnimatorsConfigFile{errorMessage};
        }
        const auto animationsSettings = readMultipleFilesAnimationsSettings(animations);
        animatorsSettings.push_back({animatorName.as<std::string>(), animationsSettings});
    }
    return animatorsSettings;
}

std::vector<MultipleFilesAnimationSettings>
AnimatorSettingsYamlReader::readMultipleFilesAnimationsSettings(const YAML::Node& animations) const
{
    std::vector<MultipleFilesAnimationSettings> animationsSettings;

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

std::vector<SingleFileAnimatorSettings>
AnimatorSettingsYamlReader::readSingleFileAnimatorsSettings(const YAML::Node& animators) const
{
    std::vector<SingleFileAnimatorSettings> animatorsSettings;

    for (const auto& animator : animators)
    {
        const auto animatorName = animator[nameField];
        const auto animations = animator[animationsField];

        if (not animatorName || not animations)
        {
            auto errorMessage = "Missing fields in single file animators";
            std::cerr << errorMessage << "\n";
            throw exceptions::InvalidAnimatorsConfigFile{errorMessage};
        }
        const auto animationsSettings = readSingleFileAnimationsSettings(animations);
        animatorsSettings.push_back({animatorName.as<std::string>(), animationsSettings});
    }
    return animatorsSettings;
}

std::vector<SingleFileAnimationSettings>
AnimatorSettingsYamlReader::readSingleFileAnimationsSettings(const YAML::Node& animations) const
{
    std::vector<SingleFileAnimationSettings> animationsSettings;

    for (const auto& animation : animations)
    {
        const auto type = animation[animationTypeField];
        const auto texturePath = animation[texturePathField];
        const auto textureWidthInPixels = animation[textureWidthInPixelsField];
        const auto textureHeightInPixels = animation[textureHeightInPixelsField];
        const auto rectToCutWidthInPixels = animation[rectToCutWidthInPixelsField];
        const auto rectToCutHeightInPixels = animation[rectToCutHeightInPixelsField];
        const auto startXPosition = animation[startXPositionField];
        const auto startYPosition = animation[startYPositionField];
        const auto numberOfTextures = animation[numberOfTexturesField];
        const auto timeBetweenTexturesInSeconds = animation[timeBetweenTexturesInSecondsField];

        if (not type || not texturePath || not textureWidthInPixels || not textureHeightInPixels ||
            not rectToCutWidthInPixels || not rectToCutHeightInPixels || not startXPosition ||
            not startYPosition || not numberOfTextures || not timeBetweenTexturesInSeconds)
        {
            auto errorMessage = "Missing fields in animations";
            std::cerr << errorMessage << "\n";
            throw exceptions::InvalidAnimatorsConfigFile{errorMessage};
        }
        animationsSettings.push_back(
            {type.as<std::string>(), texturePath.as<std::string>(),
             utils::Vector2u{textureWidthInPixels.as<unsigned>(), textureHeightInPixels.as<unsigned>()},
             utils::IntRect{startXPosition.as<int>(), startYPosition.as<int>(),
                            rectToCutWidthInPixels.as<int>(), rectToCutHeightInPixels.as<int>()},
             numberOfTextures.as<int>(), timeBetweenTexturesInSeconds.as<float>()});
    }

    return animationsSettings;
}

}