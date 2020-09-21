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
    [[nodiscard]] AnimatorsSettings readAnimatorsSettings(const utils::FilePath& yamlFilePath) const;

private:
    YAML::Node loadYamlFile(const utils::FilePath& yamlFilePath) const;
    AnimatorsSettings readAnimatorsSettings(const YAML::Node&) const;
    AnimationsSettings readAnimationsSettings(const YAML::Node&) const;
};
}