#include "ControlsStateUIConfigBuilder.h"

#include "Color.h"
#include "CommonUIConfigElements.h"
#include "ControlsState.h"
#include "ProjectPathReader.h"
#include "Vector.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto controlButtonFontSize{20};
const auto controlButtonSize = utils::Vector2f{8.f, 3.f};
const auto controlsTitlePosition = utils::Vector2f{32.f, 6.f};
const auto backToMenuButtonPosition = utils::Vector2f{34.5f, 48.f};
const auto upButtonPosition = utils::Vector2f{23.f, 16.f};
const auto downButtonPosition = utils::Vector2f{23.f, 22.f};
const auto rightButtonPosition = utils::Vector2f{23.f, 28.f};
const auto leftButtonPosition = utils::Vector2f{23.f, 34.f};
const auto spaceButtonPosition = utils::Vector2f{52.f, 16.f};
const auto shiftButtonPosition = utils::Vector2f{52.f, 22.f};
const auto ctrlButtonPosition = utils::Vector2f{52.f, 28.f};
const auto eButtonPosition = utils::Vector2f{52.f, 34.f};
const unsigned int descriptionOffset = 2;
const auto upDescriptionPosition =
    utils::Vector2f{upButtonPosition.x + controlButtonSize.x + descriptionOffset, upButtonPosition.y};
const auto downDescriptionPosition =
    utils::Vector2f{downButtonPosition.x + controlButtonSize.x + descriptionOffset, downButtonPosition.y};
const auto rightDescriptionPosition =
    utils::Vector2f{rightButtonPosition.x + controlButtonSize.x + descriptionOffset, rightButtonPosition.y};
const auto leftDescriptionPosition =
    utils::Vector2f{leftButtonPosition.x + controlButtonSize.x + descriptionOffset, leftButtonPosition.y};
const auto spaceDescriptionPosition =
    utils::Vector2f{spaceButtonPosition.x + controlButtonSize.x + descriptionOffset, spaceButtonPosition.y};
const auto shiftDescriptionPosition =
    utils::Vector2f{shiftButtonPosition.x + controlButtonSize.x + descriptionOffset, shiftButtonPosition.y};
const auto ctrlDescriptionPosition =
    utils::Vector2f{ctrlButtonPosition.x + controlButtonSize.x + descriptionOffset, ctrlButtonPosition.y};
const auto eDescriptionPosition =
    utils::Vector2f{eButtonPosition.x + controlButtonSize.x + descriptionOffset, eButtonPosition.y};
const auto iconSize = utils::Vector2f{4, 4};
const std::vector<utils::Vector2f> iconPositions{backToMenuButtonPosition};
}

std::vector<std::string> ControlsStateUIConfigBuilder::iconNames{"controlsIcon1Image"};

std::unique_ptr<components::ui::UIConfig>
ControlsStateUIConfigBuilder::createControlsUIConfig(ControlsState* controlsState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(controlsState), createButtonConfigs(controlsState),
        createCheckBoxConfigs(controlsState), createLabelConfigs(controlsState),
        createTextFieldConfigs(controlsState), createImageConfigs(controlsState));
}

std::vector<std::vector<GridButtonInfo>> ControlsStateUIConfigBuilder::getGridButtonsInfo()
{
    std::vector<std::vector<GridButtonInfo>> gridButtonsInfo{
        {GridButtonInfo{"controlsBackToMenuButton", 0, false, false}}};

    return gridButtonsInfo;
}

std::vector<std::string> ControlsStateUIConfigBuilder::getIconNames()
{
    return iconNames;
}

std::unique_ptr<components::ui::BackgroundConfig>
ControlsStateUIConfigBuilder::createBackgroundConfig(ControlsState*)
{
    return std::make_unique<components::ui::BackgroundConfig>(
        "controlsBackground", utils::Vector2f{0.f, 0.f}, utils::Vector2f{80.f, 60.f},
        graphics::VisibilityLayer::Background, menuBackgroundPath);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
ControlsStateUIConfigBuilder::createButtonConfigs(ControlsState* controlsState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;

    const auto backToMenuButtonSize = utils::Vector2f{13.f, 5.f};
    const auto backToMenuButtonOnMouseOver = [=]
    {
        controlsState->buttonsNavigator->setFocusOnButton("controlsBackToMenuButton");

        controlsState->uiManager->setColor("controlsBackToMenuButton", buttonHoverColor);
    };
    const auto backToMenuButtonOnMouseOut = [=]
    {
        controlsState->buttonsNavigator->loseFocus();
        controlsState->uiManager->setColor("controlsBackToMenuButton", buttonColor);
    };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [=] { controlsState->backToMenu(); };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsBackToMenuButton", backToMenuButtonPosition, backToMenuButtonSize, buttonColor, "Back",
        graphics::Color::Black, 37, fontPath, utils::Vector2f{2.0f, 0.0f}, backToMenuClickAction,
        backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    auto upButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsUpButton", upButtonPosition, controlButtonSize, buttonColor, "^", graphics::Color::Black,
        controlButtonFontSize, fontPath, utils::Vector2f{3.5f, 0.7f});
    buttonsConfig.emplace_back(std::move(upButtonConfig));

    auto downButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsDownButton", downButtonPosition, controlButtonSize, buttonColor, "v", graphics::Color::Black,
        controlButtonFontSize, fontPath, utils::Vector2f{3.5f, 0.1f});
    buttonsConfig.emplace_back(std::move(downButtonConfig));

    auto rightButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsRightButton", rightButtonPosition, controlButtonSize, buttonColor, "->",
        graphics::Color::Black, controlButtonFontSize, fontPath, utils::Vector2f{3.0f, 0.2f});
    buttonsConfig.emplace_back(std::move(rightButtonConfig));

    auto leftButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsLeftButton", leftButtonPosition, controlButtonSize, buttonColor, "<-",
        graphics::Color::Black, controlButtonFontSize, fontPath, utils::Vector2f{3.0f, 0.2f});
    buttonsConfig.emplace_back(std::move(leftButtonConfig));

    auto spaceButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsSpaceButton", spaceButtonPosition, controlButtonSize, buttonColor, "Space",
        graphics::Color::Black, controlButtonFontSize, fontPath, utils::Vector2f{1.0f, 0.2f});
    buttonsConfig.emplace_back(std::move(spaceButtonConfig));

    auto shiftButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsShiftButton", shiftButtonPosition, controlButtonSize, buttonColor, "Shift",
        graphics::Color::Black, controlButtonFontSize, fontPath, utils::Vector2f{1.0f, 0.2f});
    buttonsConfig.emplace_back(std::move(shiftButtonConfig));

    auto ctrlButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsCtrlButton", ctrlButtonPosition, controlButtonSize, buttonColor, "Ctrl",
        graphics::Color::Black, controlButtonFontSize, fontPath, utils::Vector2f{1.7f, 0.2f});
    buttonsConfig.emplace_back(std::move(ctrlButtonConfig));

    auto eButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsEButton", eButtonPosition, controlButtonSize, buttonColor, "E", graphics::Color::Black,
        controlButtonFontSize, fontPath, utils::Vector2f{3.5f, 0.2f});
    buttonsConfig.emplace_back(std::move(eButtonConfig));

    return buttonsConfig;
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
ControlsStateUIConfigBuilder::createCheckBoxConfigs(ControlsState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>>
ControlsStateUIConfigBuilder::createLabelConfigs(ControlsState*)
{
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;

    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "controlsTitleLabel", controlsTitlePosition, graphics::Color::Black, "Controls", 37, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    auto upDescriptionLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "controlsUpDescriptionLabel", upDescriptionPosition, graphics::Color::Black, "Jump",
        controlButtonFontSize, fontPath);
    labelsConfig.emplace_back(std::move(upDescriptionLabelConfig));

    auto downDescriptionLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "controlsDownDescriptionLabel", downDescriptionPosition, graphics::Color::Black, "Squat",
        controlButtonFontSize, fontPath);
    labelsConfig.emplace_back(std::move(downDescriptionLabelConfig));

    auto rightDescriptionLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "controlsRightDescriptionLabel", rightDescriptionPosition, graphics::Color::Black, "Move right",
        controlButtonFontSize, fontPath);
    labelsConfig.emplace_back(std::move(rightDescriptionLabelConfig));

    auto leftDescriptionLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "controlsLeftDescriptionLabel", leftDescriptionPosition, graphics::Color::Black, "Move left",
        controlButtonFontSize, fontPath);
    labelsConfig.emplace_back(std::move(leftDescriptionLabelConfig));

    auto spaceDescriptionLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "controlsSpaceDescriptionLabel", spaceDescriptionPosition, graphics::Color::Black, "Attack",
        controlButtonFontSize, fontPath);
    labelsConfig.emplace_back(std::move(spaceDescriptionLabelConfig));

    auto shiftDescriptionLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "controlsShiftDescriptionLabel", shiftDescriptionPosition, graphics::Color::Black, "Speed up",
        controlButtonFontSize, fontPath);
    labelsConfig.emplace_back(std::move(shiftDescriptionLabelConfig));

    auto ctrlDescriptionLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "controlsCtrlDescriptionLabel", ctrlDescriptionPosition, graphics::Color::Black, "Not defined",
        controlButtonFontSize, fontPath);
    labelsConfig.emplace_back(std::move(ctrlDescriptionLabelConfig));

    auto eDescriptionLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "controlsEDescriptionLabel", eDescriptionPosition, graphics::Color::Black, "Pick item",
        controlButtonFontSize, fontPath);
    labelsConfig.emplace_back(std::move(eDescriptionLabelConfig));

    return labelsConfig;
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
ControlsStateUIConfigBuilder::createTextFieldConfigs(ControlsState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::ImageConfig>>
ControlsStateUIConfigBuilder::createImageConfigs(ControlsState*)
{
    std::vector<std::unique_ptr<components::ui::ImageConfig>> imagesConfig;

    for (std::size_t iconIndex = 0; iconIndex < iconNames.size(); iconIndex++)
    {
        const auto iconPosition = utils::Vector2f{iconPositions[iconIndex].x - iconSize.x - 0.25f,
                                                  iconPositions[iconIndex].y + 0.4f};
        auto imageConfig = std::make_unique<components::ui::ImageConfig>(
            iconNames[iconIndex], iconPosition, iconSize, graphics::VisibilityLayer::First, iconPath);
        imagesConfig.push_back(std::move(imageConfig));
    }

    return imagesConfig;
}
}