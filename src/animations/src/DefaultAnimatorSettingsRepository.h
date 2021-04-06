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

    boost::optional<SingleFileAnimatorSettings>
    getSingleFileAnimatorSettings(const std::string& animatorName) const override;
    boost::optional<MultipleFilesAnimatorSettings>
    getMultipleFileAnimatorSettings(const std::string& animatorName) const override;

private:
    std::unique_ptr<AnimatorSettingsReader> animatorSettingsReader;
    std::unordered_map<std::string, SingleFileAnimatorSettings> singleFileAnimatorSettings;
    std::unordered_map<std::string, MultipleFilesAnimatorSettings> multipleFilesAnimatorSettings;

    static const std::string animatorSettingsFilePath;
};
}
