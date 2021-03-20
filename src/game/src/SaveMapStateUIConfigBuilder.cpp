#include "SaveMapStateUIConfigBuilder.h"

#include "GetProjectPath.h"
#include "SaveMapState.h"
#include "ui/DefaultUIManager.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
const auto textColor = graphics::Color(200, 200, 200);
const auto smallButtonSize = utils::Vector2f{8, 3.5};
const auto buttonSize = utils::Vector2f{20, 3.5};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
const auto mapNamingTextDescriptionPosition = utils::Vector2f{24, 21};
const auto mapNamingPromptPosition =
    utils::Vector2f{mapNamingTextDescriptionPosition.x + 12.0f, mapNamingTextDescriptionPosition.y - 0.25f};
const auto changeResolutionButtonSize = utils::Vector2f{2.5, 2};
}

std::unique_ptr<components::ui::UIConfig>
SaveMapStateUIConfigBuilder::createSaveMapUIConfig(SaveMapState* saveMapState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> checkBoxesConfig;
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;
    std::vector<std::unique_ptr<components::ui::TextFieldConfig>> textFieldsConfig;

    auto backgroundConfig = std::make_unique<components::ui::BackgroundConfig>(
        "saveMapBackground", utils::Vector2f{20, 8}, utils::Vector2f{40, 37},
        graphics::VisibilityLayer::Second, graphics::Color{172});

    const auto titlePosition = utils::Vector2f{30, 11};
    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "saveMapTitleLabel", titlePosition, graphics::Color::White, "Save map", 40, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    auto mapNameLabelConfig =
        std::make_unique<components::ui::LabelConfig>("saveMapMapNameLabel", mapNamingTextDescriptionPosition,
                                                      graphics::Color::White, "Map name:", 20, fontPath);
    labelsConfig.emplace_back(std::move(mapNameLabelConfig));

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    auto mapNameTextFieldClickInsideAction = [=] {
        saveMapState->uiManager->setColor(components::ui::UIComponentType::TextField, "saveMapNameTextField",
                                          buttonHoverColor);
    };
    auto mapNameTextFieldClickOutsideAction = [=](std::string inputBuffer) {
        saveMapState->currentMapName = std::move(inputBuffer);
    };
    auto mapNameTextFieldConfig = std::make_unique<components::ui::TextFieldConfig>(
        "saveMapNameTextField", mapNamingPromptPosition, buttonSize, buttonColor, "", 20, fontPath,
        utils::Vector2f{0.75, 0.4}, mapNameTextFieldClickInsideAction, mapNameTextFieldClickOutsideAction);
    textFieldsConfig.emplace_back(std::move(mapNameTextFieldConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto cancelButtonPosition = utils::Vector2f{29, 37};
    const auto cancelButtonOnMouseOver = [=] {
        saveMapState->uiManager->setColor(components::ui::UIComponentType::Button, "saveMapCancelButton",
                                          buttonHoverColor);
    };
    const auto cancelButtonOnMouseOut = [=] {
        saveMapState->uiManager->setColor(components::ui::UIComponentType::Button, "saveMapCancelButton",
                                          buttonColor);
    };
    auto cancelButtonMouseOverActions =
        components::ui::MouseOverActions{cancelButtonOnMouseOver, cancelButtonOnMouseOut};
    auto cancelClickAction = [=] { saveMapState->shouldBackToEditorMenu = true; };
    auto cancelButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "saveMapCancelButton", cancelButtonPosition, smallButtonSize, buttonColor, "Cancel", textColor, 20,
        fontPath, utils::Vector2f{0.5, 0.5}, cancelClickAction, cancelButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(cancelButtonConfig));

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto saveButtonPosition = utils::Vector2f{43, 37};
    const auto saveButtonOnMouseOver = [=] {
        saveMapState->uiManager->setColor(components::ui::UIComponentType::Button, "saveMapSaveButton",
                                          buttonHoverColor);
    };
    const auto saveButtonOnMouseOut = [=] {
        saveMapState->uiManager->setColor(components::ui::UIComponentType::Button, "saveMapSaveButton",
                                          buttonColor);
    };
    auto saveButtonMouseOverActions =
        components::ui::MouseOverActions{saveButtonOnMouseOver, saveButtonOnMouseOut};
    auto saveClickAction = [=] { saveMapState->saveMap(); };
    auto saveButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "saveMapSaveButton", saveButtonPosition, smallButtonSize, buttonColor, "Save", textColor, 20,
        fontPath, utils::Vector2f{1.75, 0.5}, saveClickAction, saveButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(saveButtonConfig));

    return std::make_unique<components::ui::UIConfig>(std::move(backgroundConfig), std::move(buttonsConfig),
                                                      std::move(checkBoxesConfig), std::move(labelsConfig),
                                                      std::move(textFieldsConfig));
}
}