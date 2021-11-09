#include "HeadsUpDisplayUIConfigBuilder.h"

#include "CommonUIConfigElements.h"
#include "ui/UIConfig.h"

namespace game
{
namespace
{
const auto initialTexturePath = utils::ProjectPathReader::getProjectRootPath() + "resources/yerba_item.png";
}
std::size_t HeadsUpDisplayUIConfigBuilder::numberOfSlots = 8;

std::unique_ptr<components::ui::UIConfig> HeadsUpDisplayUIConfigBuilder::createUIConfig()
{
    return std::make_unique<components::ui::UIConfig>(createBackgroundConfig(), createButtonConfigs(),
                                                      createCheckBoxConfigs(), createLabelConfigs(),
                                                      createTextFieldConfigs(), createImageConfigs());
}

std::string HeadsUpDisplayUIConfigBuilder::getHealthBarId()
{
    return "hudHealthPointsBar";
}

std::string HeadsUpDisplayUIConfigBuilder::getHealthBarFrameId()
{
    return "hudHealthPointsBarFrame";
}

std::vector<std::string> HeadsUpDisplayUIConfigBuilder::getSlotIds()
{
    std::vector<std::string> slotIds;

    for (std::size_t i = 1; i <= numberOfSlots; i++)
    {
        slotIds.push_back("slot" + std::to_string(i));
    }

    return slotIds;
}

std::vector<std::string> HeadsUpDisplayUIConfigBuilder::getSlotItemIds()
{
    std::vector<std::string> slotIds;

    for (std::size_t i = 1; i <= numberOfSlots; i++)
    {
        slotIds.push_back("slotItem" + std::to_string(i));
    }

    return slotIds;
}

std::unique_ptr<components::ui::BackgroundConfig> HeadsUpDisplayUIConfigBuilder::createBackgroundConfig()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
HeadsUpDisplayUIConfigBuilder::createButtonConfigs()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
HeadsUpDisplayUIConfigBuilder::createCheckBoxConfigs()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>> HeadsUpDisplayUIConfigBuilder::createLabelConfigs()
{
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;

    const auto healthPointsLabelPosition = utils::Vector2f{0.5, 0.5};
    auto healthPointsLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "hudHealthPointsLabel", healthPointsLabelPosition, graphics::Color::Red, "HP", 20, fontPath);
    labelsConfig.emplace_back(std::move(healthPointsLabelConfig));

    const auto itemsLabelPosition = utils::Vector2f{0.5, 3};
    auto itemsLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "hudItemsLabel", itemsLabelPosition, graphics::Color::Black, "ITEMS", 20, fontPath);
    labelsConfig.emplace_back(std::move(itemsLabelConfig));

    return labelsConfig;
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
HeadsUpDisplayUIConfigBuilder::createTextFieldConfigs()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::ImageConfig>> HeadsUpDisplayUIConfigBuilder::createImageConfigs()
{
    std::vector<std::unique_ptr<components::ui::ImageConfig>> imagesConfig;

    const auto healthPointsBarPosition = utils::Vector2f{4, 1.2};
    const auto healthPointsBarSize = utils::Vector2f{7, 1.3};
    auto healthBarImageConfig = std::make_unique<components::ui::ImageConfig>(
        "hudHealthPointsBar", healthPointsBarPosition, healthPointsBarSize, graphics::VisibilityLayer::First,
        graphics::Color::Red);
    imagesConfig.push_back(std::move(healthBarImageConfig));

    auto healthBarFrameImageConfig = std::make_unique<components::ui::ImageConfig>(
        "hudHealthPointsBarFrame", healthPointsBarPosition, healthPointsBarSize,
        graphics::VisibilityLayer::First, graphics::Color::Transparent);
    imagesConfig.push_back(std::move(healthBarFrameImageConfig));

    const auto slotSize = utils::Vector2f{2, 2};
    const auto offset = 0.3f;
    const auto firstRowStartPosition = utils::Vector2f{0.5, 6};
    std::vector<utils::Vector2f> slotsPositions;
    for (int i = 0; i < 4; i++)
    {
        const auto slotPosition = utils::Vector2f{
            firstRowStartPosition.x + slotSize.x * static_cast<float>(i) + offset * static_cast<float>(i),
            firstRowStartPosition.y};
        slotsPositions.push_back(slotPosition);
    }

    const auto secondRowStartPosition =
        utils::Vector2f{firstRowStartPosition.x, firstRowStartPosition.y + slotSize.y + offset};
    for (int i = 0; i < 4; i++)
    {
        const auto slotPosition = utils::Vector2f{
            secondRowStartPosition.x + slotSize.x * static_cast<float>(i) + offset * static_cast<float>(i),
            secondRowStartPosition.y};
        slotsPositions.push_back(slotPosition);
    }

    const auto slotIds = getSlotIds();
    for (std::size_t i = 0; i < slotsPositions.size(); i++)
    {
        auto slotConfig = std::make_unique<components::ui::ImageConfig>(
            slotIds[i], slotsPositions[i], slotSize, graphics::VisibilityLayer::Second,
            graphics::Color(152, 152, 152, 152));

        imagesConfig.push_back(std::move(slotConfig));
    }

    const auto slotItemIds = getSlotItemIds();
    for (std::size_t i = 0; i < slotsPositions.size(); i++)
    {
        auto slotConfig = std::make_unique<components::ui::ImageConfig>(
            slotItemIds[i], slotsPositions[i], slotSize, graphics::VisibilityLayer::First,
            initialTexturePath);

        imagesConfig.push_back(std::move(slotConfig));
    }

    return imagesConfig;
}

}