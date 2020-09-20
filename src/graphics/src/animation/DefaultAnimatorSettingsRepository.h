#pragma once

#include "AnimatorSettingsRepository.h"
#include "AnimatorSettingsReader.h"
#include <memory>
#include <unordered_map>

namespace graphics::animation
{
class DefaultAnimatorSettingsRepository : public AnimatorSettingsRepository
{
public:
    explicit DefaultAnimatorSettingsRepository(std::unique_ptr<AnimatorSettingsReader>);

    boost::optional<AnimatorSettings> getAnimatorSettings(const std::string& animatorName) const override;

private:

    std::unique_ptr<AnimatorSettingsReader> animatorSettingsReader;
    std::unordered_map<std::string, AnimatorSettings> settings;

    static const std::string animatorSettingsFilePath;
};
}
