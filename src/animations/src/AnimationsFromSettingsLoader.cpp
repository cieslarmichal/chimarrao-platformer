#include "AnimationsFromSettingsLoader.h"

#include "IncrementalFilePathsCreator.h"
#include "ProjectPathReader.h"
#include "StlOperators.h"
#include "exceptions/AnimationsFromSettingsNotFound.h"
#include "exceptions/InvalidAnimatorSettings.h"

namespace animations
{

void AnimationsFromSettingsLoader::loadAnimationsFromMultipleFilesAnimationsSettings(
    std::unordered_map<AnimationType, Animation>& animations,
    const std::vector<MultipleFilesAnimationSettings>& multipleFilesAnimationsSettings)
{
    if (multipleFilesAnimationsSettings.empty())
    {
        throw exceptions::AnimationsFromSettingsNotFound{"Animations not found in settings"};
    }

    animations.clear();
    animations.reserve(multipleFilesAnimationsSettings.size());

    for (const auto& animationSettings : multipleFilesAnimationsSettings)
    {
        const auto animationType = toAnimationType(animationSettings.animationType);
        const auto texturePaths = utils::IncrementalFilePathsCreator::createFilePaths(
            utils::ProjectPathReader::getProjectRootPath() + animationSettings.firstTexturePath,
            animationSettings.numberOfTextures);

        std::vector<graphics::TextureRect> textureRects;
        textureRects.reserve(texturePaths.size());
        for (const auto& texturePath : texturePaths)
        {
            textureRects.push_back(graphics::TextureRect{texturePath, std::nullopt});
        }

        const auto timeBetweenTextures = animationSettings.timeBetweenTexturesInSeconds;
        const auto loopsAllowed = animationSettings.loopsAllowed;
        const auto interruptionAllowed = animationSettings.interruptionAllowed;

        animations.insert(
            {animationType, Animation{textureRects, timeBetweenTextures, loopsAllowed, interruptionAllowed}});
    }
}
void AnimationsFromSettingsLoader::loadAnimationsFromSingleFileAnimationsSettings(
    std::unordered_map<AnimationType, Animation>& animations,
    const std::vector<SingleFileAnimationSettings>& singleFileAnimationsSettings)
{
    if (singleFileAnimationsSettings.empty())
    {
        throw exceptions::AnimationsFromSettingsNotFound{"Animations not found in settings"};
    }

    animations.clear();
    animations.reserve(singleFileAnimationsSettings.size());

    for (const auto& animationSettings : singleFileAnimationsSettings)
    {
        const auto animationType = toAnimationType(animationSettings.animationType);
        const auto timeBetweenTextures = animationSettings.timeBetweenTexturesInSeconds;

        std::vector<graphics::TextureRect> textureRects;
        for (int i = 0; i < animationSettings.numberOfTextures; i++)
        {
            const auto rect = utils::IntRect{static_cast<int>(animationSettings.textureRect.left) +
                                                 i * static_cast<int>(animationSettings.textureRect.width),
                                             static_cast<int>(animationSettings.textureRect.top),
                                             static_cast<int>(animationSettings.textureRect.width),
                                             static_cast<int>(animationSettings.textureRect.height)};

            if ((rect.left < 0 or rect.left > static_cast<int>(animationSettings.textureSize.x)) and
                (rect.width < 0 or rect.height > static_cast<int>(animationSettings.textureSize.y)))
            {
                throw exceptions::InvalidAnimatorSettings{
                    "Texture rect: " + toString(rect) +
                    " is not inside texture picture:" + toString(animationSettings.textureSize)};
            }

            textureRects.push_back(graphics::TextureRect{
                utils::ProjectPathReader::getProjectRootPath() + animationSettings.texturePath, rect});
        }

        const auto loopsAllowed = animationSettings.loopsAllowed;
        const auto interruptionAllowed = animationSettings.interruptionAllowed;

        animations.insert(
            {animationType, Animation{textureRects, timeBetweenTextures, loopsAllowed, interruptionAllowed}});
    }
}

}