#include "SaveMapState.h"

#include <utility>

#include "GetProjectPath.h"
#include "core/ClickableComponent.h"
#include "core/GraphicsComponent.h"
#include "core/HitBoxComponent.h"
#include "core/MouseOverComponent.h"
#include "core/TextComponent.h"

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

SaveMapState::SaveMapState(const std::shared_ptr<window::Window>& windowInit,
                           const std::shared_ptr<input::InputManager>& inputManagerInit,
                           const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                           std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditorMenu{false},
      uiManager{std::make_unique<components::ui::DefaultUIManager>(inputManagerInit, rendererPoolInit,
                                                                   createSettingsUIConfig())}
{
    inputManager->registerObserver(this);

    possibleLeaveFromStateTimer.start();
}

SaveMapState::~SaveMapState()
{
    inputManager->removeObserver(this);
}

void SaveMapState::update(const utils::DeltaTime& deltaTime)
{
    if (possibleLeaveFromStateTimer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        (inputStatus->isKeyPressed(input::InputKey::Escape) || shouldBackToEditorMenu))
    {
        backToEditorMenu();
        return;
    }

    uiManager->update(deltaTime);
}

void SaveMapState::lateUpdate(const utils::DeltaTime& deltaTime)
{
}

void SaveMapState::render()
{
    rendererPool->renderAll();
}

std::string SaveMapState::getName() const
{
    return "Save map state";
}

void SaveMapState::activate()
{
    active = true;

    possibleLeaveFromStateTimer.restart();
    uiManager->activate();
}

void SaveMapState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void SaveMapState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void SaveMapState::saveMap()
{
    // TODO: implement
    shouldBackToEditorMenu = true;
}

void SaveMapState::backToEditorMenu()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

std::unique_ptr<components::ui::UIConfig> SaveMapState::createSettingsUIConfig()
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
    auto mapNameTextFieldClickInsideAction = [this] {
        uiManager->setColor(components::ui::UIComponentType::TextField, "saveMapNameTextField",
                            buttonHoverColor);
    };
    auto mapNameTextFieldClickOutsideAction = [this](std::string inputBuffer) {
        currentMapName = std::move(inputBuffer);
    };
    auto mapNameTextFieldConfig = std::make_unique<components::ui::TextFieldConfig>(
        "saveMapNameTextField", mapNamingPromptPosition, buttonSize, buttonColor, "", 20, fontPath,
        utils::Vector2f{0.75, 0.4}, mapNameTextFieldClickInsideAction, mapNameTextFieldClickOutsideAction);
    textFieldsConfig.emplace_back(std::move(mapNameTextFieldConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto cancelButtonPosition = utils::Vector2f{29, 37};
    const auto cancelButtonOnMouseOver = [&] {
      uiManager->setColor(components::ui::UIComponentType::Button, "saveMapCancelButton",
                          buttonHoverColor);
    };
    const auto cancelButtonOnMouseOut = [&] {
      uiManager->setColor(components::ui::UIComponentType::Button, "saveMapCancelButton", buttonColor);
    };
    auto cancelButtonMouseOverActions =
        components::ui::MouseOverActions{cancelButtonOnMouseOver, cancelButtonOnMouseOut};
    auto cancelClickAction = [this] { shouldBackToEditorMenu = true; };
    auto cancelButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "saveMapCancelButton", cancelButtonPosition, smallButtonSize, buttonColor, "Cancel",
        textColor, 20, fontPath, utils::Vector2f{0.5, 0.5}, cancelClickAction,
        cancelButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(cancelButtonConfig));

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto saveButtonPosition = utils::Vector2f{43, 37};
    const auto saveButtonOnMouseOver = [&] {
      uiManager->setColor(components::ui::UIComponentType::Button, "saveMapSaveButton",
                          buttonHoverColor);
    };
    const auto saveButtonOnMouseOut = [&] {
      uiManager->setColor(components::ui::UIComponentType::Button, "saveMapSaveButton", buttonColor);
    };
    auto saveButtonMouseOverActions =
        components::ui::MouseOverActions{saveButtonOnMouseOver, saveButtonOnMouseOut};
    auto saveClickAction = [this] { saveMap(); };
    auto saveButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "saveMapSaveButton", saveButtonPosition, smallButtonSize, buttonColor, "Save",
        textColor, 20, fontPath, utils::Vector2f{1.75, 0.5}, saveClickAction,
        saveButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(saveButtonConfig));

    return std::make_unique<components::ui::UIConfig>(std::move(backgroundConfig), std::move(buttonsConfig),
                                                      std::move(checkBoxesConfig), std::move(labelsConfig),
                                                      std::move(textFieldsConfig));
}

}