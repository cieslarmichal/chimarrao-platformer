#include "ControlsStateUIConfigBuilder.h"

#include "Color.h"
#include "ControlsState.h"
#include "GetProjectPath.h"
#include "Vector.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

namespace game
{

namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto controlButtonFontSize{20};
const auto controlButtonSize = utils::Vector2f{8, 3};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
const auto backgroundPath =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/menu_background.jpg";
const auto controlsTitlePosition = utils::Vector2f{32, 6};
const auto backToMenuButtonPosition = utils::Vector2f{34.5, 48};
const auto upButtonPosition = utils::Vector2f{23, 16};
const auto downButtonPosition = utils::Vector2f{23, 22};
const auto rightButtonPosition = utils::Vector2f{23, 28};
const auto leftButtonPosition = utils::Vector2f{23, 34};
const auto spaceButtonPosition = utils::Vector2f{52, 16};
const auto shiftButtonPosition = utils::Vector2f{52, 22};
const auto ctrlButtonPosition = utils::Vector2f{52, 28};
const auto eButtonPosition = utils::Vector2f{52, 34};
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
}

std::unique_ptr<components::ui::UIConfig>
ControlsStateUIConfigBuilder::createControlsUIConfig(ControlsState* controlsState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> checkBoxesConfig;
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;
    std::vector<std::unique_ptr<components::ui::TextFieldConfig>> textFieldsConfig;

    auto backgroundConfig = std::make_unique<components::ui::BackgroundConfig>(
        "controlsBackground", utils::Vector2f{0, 0}, utils::Vector2f{80, 60},
        graphics::VisibilityLayer::Background, backgroundPath);

    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "controlsTitleLabel", controlsTitlePosition, graphics::Color::Black, "Controls", 37, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    const auto backToMenuButtonSize = utils::Vector2f{13, 5};
    const auto backToMenuButtonOnMouseOver = [=] {
      controlsState->uiManager->setColor(components::ui::UIComponentType::Button, "controlsBackToMenuButton",
                          buttonHoverColor);
    };
    const auto backToMenuButtonOnMouseOut = [=] {
      controlsState->uiManager->setColor(components::ui::UIComponentType::Button, "controlsBackToMenuButton", buttonColor);
    };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [=] { controlsState->shouldBackToMenu = true; };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsBackToMenuButton", backToMenuButtonPosition, backToMenuButtonSize, buttonColor, "Back",
        graphics::Color::Black, 37, fontPath, utils::Vector2f{2, 0}, backToMenuClickAction,
        backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    auto upButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsUpButton", upButtonPosition, controlButtonSize, buttonColor, "^", graphics::Color::Black,
        controlButtonFontSize, fontPath, utils::Vector2f{3.5, 0.7});
    buttonsConfig.emplace_back(std::move(upButtonConfig));

    auto downButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsDownButton", downButtonPosition, controlButtonSize, buttonColor, "v", graphics::Color::Black,
        controlButtonFontSize, fontPath, utils::Vector2f{3.5, 0.1});
    buttonsConfig.emplace_back(std::move(downButtonConfig));

    auto rightButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsRightButton", rightButtonPosition, controlButtonSize, buttonColor, "->",
        graphics::Color::Black, controlButtonFontSize, fontPath, utils::Vector2f{3, 0.2});
    buttonsConfig.emplace_back(std::move(rightButtonConfig));

    auto leftButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsLeftButton", leftButtonPosition, controlButtonSize, buttonColor, "<-",
        graphics::Color::Black, controlButtonFontSize, fontPath, utils::Vector2f{3, 0.2});
    buttonsConfig.emplace_back(std::move(leftButtonConfig));

    auto spaceButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsSpaceButton", spaceButtonPosition, controlButtonSize, buttonColor, "Space",
        graphics::Color::Black, controlButtonFontSize, fontPath, utils::Vector2f{1.0, 0.2});
    buttonsConfig.emplace_back(std::move(spaceButtonConfig));

    auto shiftButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsShiftButton", shiftButtonPosition, controlButtonSize, buttonColor, "Shift",
        graphics::Color::Black, controlButtonFontSize, fontPath, utils::Vector2f{1.0, 0.2});
    buttonsConfig.emplace_back(std::move(shiftButtonConfig));

    auto ctrlButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsCtrlButton", ctrlButtonPosition, controlButtonSize, buttonColor, "Ctrl",
        graphics::Color::Black, controlButtonFontSize, fontPath, utils::Vector2f{1.7, 0.2});
    buttonsConfig.emplace_back(std::move(ctrlButtonConfig));

    auto eButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "controlsEButton", eButtonPosition, controlButtonSize, buttonColor, "E", graphics::Color::Black,
        controlButtonFontSize, fontPath, utils::Vector2f{3.5, 0.2});
    buttonsConfig.emplace_back(std::move(eButtonConfig));

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

    return std::make_unique<components::ui::UIConfig>(std::move(backgroundConfig), std::move(buttonsConfig),
                                                      std::move(checkBoxesConfig), std::move(labelsConfig),
                                                      std::move(textFieldsConfig));
}
}