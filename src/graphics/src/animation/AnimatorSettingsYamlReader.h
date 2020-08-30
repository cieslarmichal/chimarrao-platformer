#pragma once

#include <boost/optional.hpp>

#include "yaml-cpp/yaml.h"

#include "AnimatorSettings.h"
#include "FilePath.h"

namespace graphics::animation
{
class AnimatorSettingsYamlReader
{
public:
    static boost::optional<AnimatorsSettings> readAnimatorsSettings(const utils::FilePath& yamlFilePath);

private:
    static YAML::Node loadYamlFile(const utils::FilePath& yamlFilePath);
    static boost::optional<AnimatorsSettings> readAnimatorsSettings(const YAML::Node&);
    static boost::optional<AnimationsSettings> readAnimationsSettings(const YAML::Node&);
};
}