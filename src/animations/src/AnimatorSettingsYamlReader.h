#pragma once

#include "AnimatorSettingsReader.h"

namespace YAML
{
class Node;
}

namespace animations
{
class AnimatorSettingsYamlReader : public AnimatorSettingsReader
{
public:
    AnimatorsSettings readAnimatorsSettings(const utils::FilePath& yamlFilePath) const;

private:
    YAML::Node loadYamlFile(const utils::FilePath& yamlFilePath) const;
    std::vector<MultipleFilesAnimatorSettings> readMultipleFilesAnimatorsSettings(const YAML::Node&) const;
    std::vector<MultipleFilesAnimationSettings> readMultipleFilesAnimationsSettings(const YAML::Node&) const;
    std::vector<SingleFileAnimatorSettings> readSingleFileAnimatorsSettings(const YAML::Node&) const;
    std::vector<SingleFileAnimationSettings> readSingleFileAnimationsSettings(const YAML::Node&) const;
};
}