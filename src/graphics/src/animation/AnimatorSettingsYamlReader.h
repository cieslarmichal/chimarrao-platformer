#pragma once

#include "yaml-cpp/yaml.h"

#include "AnimatorSettingsReader.h"

namespace graphics::animation
{
class AnimatorSettingsYamlReader : public AnimatorSettingsReader
{
public:
    boost::optional<AnimatorsSettings>
    readAnimatorsSettings(const utils::FilePath& yamlFilePath) const override;

private:
    YAML::Node loadYamlFile(const utils::FilePath& yamlFilePath) const;
    boost::optional<AnimatorsSettings> readAnimatorsSettings(const YAML::Node&) const;
    boost::optional<AnimationsSettings> readAnimationsSettings(const YAML::Node&) const;
};
}