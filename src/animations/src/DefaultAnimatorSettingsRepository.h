#pragma once

#include <memory>
#include <unordered_map>

#include "AnimatorSettingsReader.h"
#include "AnimatorSettingsRepository.h"

namespace animations
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
