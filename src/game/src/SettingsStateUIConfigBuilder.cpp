#include "SettingsStateUIConfigBuilder.h"

#include "CommonUIConfigElements.h"
#include "ProjectPathReader.h"
#include "ui/DefaultUIManager.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto buttonSize = utils::Vector2f{25.f, 5.f};
const auto sectionTextFontSize{37};
const auto displayModeFontSize{12};
const auto displayModeButtonSize = utils::Vector2f{8.f, 3.f};
const auto settingsTitlePosition = utils::Vector2f{32.f, 6.f};
const auto displayModeSectionPosition = utils::Vector2f{38.f, 18.f};
const auto windowModeButtonPosition = utils::Vector2f{
    displayModeSectionPosition.x + 15.8f + displayModeButtonSize.x, displayModeSectionPosition.y + 0.5f};
const auto fullscreenModeButtonPosition =
    utils::Vector2f{windowModeButtonPosition.x + displayModeButtonSize.x + 0.5f, windowModeButtonPosition.y};
const auto resolutionSectionPosition = utils::Vector2f{38.f, 24.f};
const auto resolutionTextFieldPosition = utils::Vector2f{
    resolutionSectionPosition.x + 18.5f + displayModeButtonSize.x, resolutionSectionPosition.y + 0.8f};
const auto changeResolutionButtonSize = utils::Vector2f{2.5f, 2.f};
const auto resolutionDecreaseButtonPosition =
    utils::Vector2f{resolutionTextFieldPosition.x - 2.8f, resolutionSectionPosition.y + 1.15f};
const auto resolutionIncreaseButtonPosition =
    utils::Vector2f{resolutionTextFieldPosition.x + 11.2f, resolutionSectionPosition.y + 1.15f};
const auto vsyncSectionPosition = utils::Vector2f{38.f, 30.f};
const auto vsyncButtonPosition =
    utils::Vector2f{vsyncSectionPosition.x + 31.0f, vsyncSectionPosition.y + 0.5f};
const auto vsyncButtonSize = utils::Vector2f{3.f, 3.f};
const auto frameLimitSectionPosition = utils::Vector2f{38.f, 36.f};
const auto frameLimitTextFieldPosition = utils::Vector2f{
    frameLimitSectionPosition.x + 22.5f + displayModeButtonSize.x, frameLimitSectionPosition.y + 0.8f};
const auto frameLimitDecreaseButtonPosition =
    utils::Vector2f{frameLimitTextFieldPosition.x - 4.0f, frameLimitSectionPosition.y + 1.15f};
const auto frameLimitIncreaseButtonPosition =
    utils::Vector2f{frameLimitTextFieldPosition.x + 4.5f, frameLimitSectionPosition.y + 1.15f};
const auto backToMenuButtonPosition = utils::Vector2f{34.5f, 48.f};
const auto applyChangesButtonPosition = utils::Vector2f{55.f, 48.f};
const auto iconSize = utils::Vector2f{4, 4};
const std::vector<utils::Vector2f> sectionPositions{displayModeSectionPosition, resolutionSectionPosition,
                                                    vsyncSectionPosition,       frameLimitSectionPosition,
                                                    backToMenuButtonPosition,   applyChangesButtonPosition};
}

std::unique_ptr<components::ui::UIConfig>
SettingsStateUIConfigBuilder::createSettingsUIConfig(SettingsState* settingsState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(settingsState), createButtonConfigs(settingsState),
        createCheckBoxConfigs(settingsState), createLabelConfigs(settingsState),
        createTextFieldConfigs(settingsState), createImageConfigs(settingsState));
}

std::unique_ptr<components::ui::BackgroundConfig>
SettingsStateUIConfigBuilder::createBackgroundConfig(SettingsState*)
{
    return std::make_unique<components::ui::BackgroundConfig>(
        "settingsBackground", utils::Vector2f{0.f, 0.f}, utils::Vector2f{80.f, 60.f},
        graphics::VisibilityLayer::Background, menuBackgroundPath);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
SettingsStateUIConfigBuilder::createButtonConfigs(SettingsState* settingsState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;

    const auto backToMenuButtonSize = utils::Vector2f{13.f, 5.f};
    const auto backToMenuButtonOnMouseOver = [=]
    { settingsState->uiManager->setColor("settingsBackToMenuButton", buttonHoverColor); };
    const auto backToMenuButtonOnMouseOut = [=]
    { settingsState->uiManager->setColor("settingsBackToMenuButton", buttonColor); };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [=] { settingsState->shouldBackToMenu = true; };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsBackToMenuButton", backToMenuButtonPosition, backToMenuButtonSize, buttonColor, "Back",
        graphics::Color::Black, sectionTextFontSize, fontPath, utils::Vector2f{2.f, 0.f},
        backToMenuClickAction, backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    const auto applyChangesButtonSize = utils::Vector2f{13.f, 5.f};
    const auto applyChangesButtonOnMouseOver = [=]
    { settingsState->uiManager->setColor("settingsApplyChangesButton", buttonHoverColor); };
    const auto applyChangesButtonOnMouseOut = [=]
    { settingsState->uiManager->setColor("settingsApplyChangesButton", buttonColor); };
    auto applyChangesButtonMouseOverActions =
        components::ui::MouseOverActions{applyChangesButtonOnMouseOver, applyChangesButtonOnMouseOut};
    auto applyChangesClickAction = [=] { settingsState->applyWindowSettingsChanges(); };
    auto applyChangesButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsApplyChangesButton", applyChangesButtonPosition, applyChangesButtonSize, buttonColor,
        "Apply", graphics::Color::Black, sectionTextFontSize, fontPath, utils::Vector2f{1.f, 0.f},
        applyChangesClickAction, applyChangesButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(applyChangesButtonConfig));

    auto windowModeClickAction = [=] { settingsState->setWindowMode(); };
    auto windowModeButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsWindowModeButton", windowModeButtonPosition, displayModeButtonSize, buttonColor, "Window",
        graphics::Color::Black, displayModeFontSize, fontPath, utils::Vector2f{2.f, 0.9f},
        windowModeClickAction);
    buttonsConfig.emplace_back(std::move(windowModeButtonConfig));

    auto fullscreenModeClickAction = [=] { settingsState->setFullscreenMode(); };
    auto fullscreenModeButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsFullscreenModeButton", fullscreenModeButtonPosition, displayModeButtonSize, buttonColor,
        "Fullscreen", graphics::Color::Black, displayModeFontSize, fontPath, utils::Vector2f{0.5f, 0.9f},
        fullscreenModeClickAction);
    buttonsConfig.emplace_back(std::move(fullscreenModeButtonConfig));

    const auto resolutionDecreaseButtonOnMouseOver = [=]
    { settingsState->uiManager->setColor("settingsResolutionDecreaseButton", buttonHoverColor); };
    const auto resolutionDecreaseButtonOnMouseOut = [=]
    { settingsState->uiManager->setColor("settingsResolutionDecreaseButton", buttonColor); };
    auto resolutionDecreaseButtonMouseOverActions = components::ui::MouseOverActions{
        resolutionDecreaseButtonOnMouseOver, resolutionDecreaseButtonOnMouseOut};
    auto resolutionDecreaseButtonClickAction = [=] { settingsState->decreaseResolution(); };
    auto resolutionDecreaseButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsResolutionDecreaseButton", resolutionDecreaseButtonPosition, changeResolutionButtonSize,
        buttonColor, "<", graphics::Color::Black, 20, fontPath, utils::Vector2f{0.6f, -0.3f},
        resolutionDecreaseButtonClickAction, resolutionDecreaseButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(resolutionDecreaseButtonConfig));

    const auto resolutionIncreaseButtonOnMouseOver = [=]
    { settingsState->uiManager->setColor("settingsResolutionIncreaseButton", buttonHoverColor); };
    const auto resolutionIncreaseButtonOnMouseOut = [=]
    { settingsState->uiManager->setColor("settingsResolutionIncreaseButton", buttonColor); };
    auto resolutionIncreaseButtonMouseOverActions = components::ui::MouseOverActions{
        resolutionIncreaseButtonOnMouseOver, resolutionIncreaseButtonOnMouseOut};
    auto resolutionIncreaseButtonClickAction = [=] { settingsState->increaseResolution(); };
    auto resolutionIncreaseButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsResolutionIncreaseButton", resolutionIncreaseButtonPosition, changeResolutionButtonSize,
        buttonColor, ">", graphics::Color::Black, 20, fontPath, utils::Vector2f{0.6f, -0.3f},
        resolutionIncreaseButtonClickAction, resolutionIncreaseButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(resolutionIncreaseButtonConfig));

    const auto frameLimitDecreaseButtonOnMouseOver = [=]
    { settingsState->uiManager->setColor("settingsFrameLimitDecreaseButton", buttonHoverColor); };
    const auto frameLimitDecreaseButtonOnMouseOut = [=]
    { settingsState->uiManager->setColor("settingsFrameLimitDecreaseButton", buttonColor); };
    auto frameLimitDecreaseButtonMouseOverActions = components::ui::MouseOverActions{
        frameLimitDecreaseButtonOnMouseOver, frameLimitDecreaseButtonOnMouseOut};
    auto frameLimitDecreaseButtonClickAction = [=] { settingsState->decreaseFrameLimit(); };
    auto frameLimitDecreaseButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsFrameLimitDecreaseButton", frameLimitDecreaseButtonPosition, changeResolutionButtonSize,
        buttonColor, "<", graphics::Color::Black, 20, fontPath, utils::Vector2f{0.6f, -0.3f},
        frameLimitDecreaseButtonClickAction, frameLimitDecreaseButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(frameLimitDecreaseButtonConfig));

    const auto frameLimitIncreaseButtonOnMouseOver = [=]
    { settingsState->uiManager->setColor("settingsFrameLimitIncreaseButton", buttonHoverColor); };
    const auto frameLimitIncreaseButtonOnMouseOut = [=]
    { settingsState->uiManager->setColor("settingsFrameLimitIncreaseButton", buttonColor); };
    auto frameLimitIncreaseButtonMouseOverActions = components::ui::MouseOverActions{
        frameLimitIncreaseButtonOnMouseOver, frameLimitIncreaseButtonOnMouseOut};
    auto frameLimitIncreaseButtonClickAction = [=] { settingsState->increaseFrameLimit(); };
    auto frameLimitIncreaseButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsFrameLimitIncreaseButton", frameLimitIncreaseButtonPosition, changeResolutionButtonSize,
        buttonColor, ">", graphics::Color::Black, 20, fontPath, utils::Vector2f{0.6f, -0.3f},
        frameLimitIncreaseButtonClickAction, frameLimitIncreaseButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(frameLimitIncreaseButtonConfig));

    return buttonsConfig;
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
SettingsStateUIConfigBuilder::createCheckBoxConfigs(SettingsState* settingsState)
{
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> checkBoxesConfig;

    const auto vsyncCheckBoxOnMouseOver = [=]
    { settingsState->uiManager->setColor("settingsVsyncCheckBox", buttonHoverColor); };
    const auto vsyncCheckBoxOnMouseOut = [=]
    { settingsState->uiManager->setColor("settingsVsyncCheckBox", buttonColor); };
    auto vsyncCheckBoxMouseOverActions =
        components::ui::MouseOverActions{vsyncCheckBoxOnMouseOver, vsyncCheckBoxOnMouseOut};
    auto vsyncCheckBoxClickAction = [=] { settingsState->switchVsync(); };
    auto vsyncCheckBoxConfig = std::make_unique<components::ui::CheckBoxConfig>(
        "settingsVsyncCheckBox", vsyncButtonPosition, vsyncButtonSize, buttonColor, false, 25, fontPath,
        utils::Vector2f{0.8f, 0.0f}, vsyncCheckBoxClickAction, vsyncCheckBoxMouseOverActions);
    checkBoxesConfig.emplace_back(std::move(vsyncCheckBoxConfig));

    return checkBoxesConfig;
}

std::vector<std::unique_ptr<components::ui::LabelConfig>>
SettingsStateUIConfigBuilder::createLabelConfigs(SettingsState*)
{
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;

    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "settingsTitleLabel", settingsTitlePosition, graphics::Color::Black, "Settings", 37, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    auto displayModeLabelConfig =
        std::make_unique<components::ui::LabelConfig>("settingsDisplayModeLabel", displayModeSectionPosition,
                                                      graphics::Color::Black, "Display mode:", 30, fontPath);
    labelsConfig.emplace_back(std::move(displayModeLabelConfig));

    auto resolutionLabelConfig =
        std::make_unique<components::ui::LabelConfig>("settingsResolutionLabel", resolutionSectionPosition,
                                                      graphics::Color::Black, "Resolution:", 30, fontPath);
    labelsConfig.emplace_back(std::move(resolutionLabelConfig));

    auto resolutionValueLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "settingsResolutionValueLabel", resolutionTextFieldPosition, graphics::Color::Black, "", 20,
        fontPath);
    labelsConfig.emplace_back(std::move(resolutionValueLabelConfig));

    auto vsyncLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "settingsVsyncLabel", vsyncSectionPosition, graphics::Color::Black, "Vsync:", 30, fontPath);
    labelsConfig.emplace_back(std::move(vsyncLabelConfig));

    auto frameLimitLabelConfig =
        std::make_unique<components::ui::LabelConfig>("settingsFrameLimitLabel", frameLimitSectionPosition,
                                                      graphics::Color::Black, "Frame limit:", 30, fontPath);
    labelsConfig.emplace_back(std::move(frameLimitLabelConfig));

    auto frameLimitValueLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "settingsFrameLimitValueLabel", frameLimitTextFieldPosition, graphics::Color::Black, "", 20,
        fontPath);
    labelsConfig.emplace_back(std::move(frameLimitValueLabelConfig));

    return labelsConfig;
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
SettingsStateUIConfigBuilder::createTextFieldConfigs(SettingsState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::ImageConfig>>
SettingsStateUIConfigBuilder::createImageConfigs(SettingsState* settingsState)
{
    std::vector<std::unique_ptr<components::ui::ImageConfig>> imagesConfig;

    for (std::size_t iconIndex = 0; iconIndex < settingsState->iconNames.size(); iconIndex++)
    {
        const auto iconPosition = utils::Vector2f{sectionPositions[iconIndex].x - buttonSize.x - 1,
                                                  sectionPositions[iconIndex].y + 1};

        auto imageConfig = std::make_unique<components::ui::ImageConfig>(
            settingsState->iconNames[iconIndex], iconPosition, iconSize, graphics::VisibilityLayer::First,
            iconPath);
        imagesConfig.push_back(std::move(imageConfig));
    }

    return imagesConfig;
}
}