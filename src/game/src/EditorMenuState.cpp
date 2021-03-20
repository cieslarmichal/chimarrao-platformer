#include "EditorMenuState.h"

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
const auto buttonSize = utils::Vector2f{25, 5};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
}

EditorMenuState::EditorMenuState(const std::shared_ptr<window::Window>& windowInit,
                                 const std::shared_ptr<input::InputManager>& inputManagerInit,
                                 const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                                 std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditor{false},
      shouldBackToMenu{false},
      uiManager{std::make_unique<components::ui::DefaultUIManager>(inputManagerInit, rendererPoolInit,
                                                                   createSettingsUIConfig())}
{
    inputManager->registerObserver(this);
    possibleLeaveFromStateTimer.start();
}

EditorMenuState::~EditorMenuState()
{
    inputManager->removeObserver(this);
}

void EditorMenuState::update(const utils::DeltaTime& deltaTime)
{
    if (possibleLeaveFromStateTimer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        shouldBackToEditor = true;
    }

    if (shouldBackToEditor)
    {
        backToEditor();
        return;
    }

    if (shouldBackToMenu)
    {
        backToMenu();
        return;
    }

    uiManager->update(deltaTime);
}

void EditorMenuState::lateUpdate(const utils::DeltaTime& deltaTime)
{
}

void EditorMenuState::render()
{
    rendererPool->renderAll();
}

std::string EditorMenuState::getName() const
{
    return "Editor menu state";
}

void EditorMenuState::activate()
{
    active = true;
    uiManager->activate();
    possibleLeaveFromStateTimer.restart();
}

void EditorMenuState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void EditorMenuState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void EditorMenuState::backToEditor()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void EditorMenuState::backToMenu()
{
    while (not states.empty() && states.top()->getName() != "Menu state")
    {
        states.pop();
    }

    if (not states.empty())
    {
        states.top()->activate();
    }
}

std::unique_ptr<components::ui::UIConfig> EditorMenuState::createSettingsUIConfig()
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> checkBoxesConfig;
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;
    std::vector<std::unique_ptr<components::ui::TextFieldConfig>> textFieldsConfig;

    const auto backgroundColor = graphics::Color{172};
    auto backgroundConfig = std::make_unique<components::ui::BackgroundConfig>(
        "editorMenuBackground", utils::Vector2f{25, 5}, utils::Vector2f{31, 50},
        graphics::VisibilityLayer::Second, backgroundColor);

    const auto editorMenuTitlePosition = utils::Vector2f{27.5, 7};
    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "editorMenuTitleLabel", editorMenuTitlePosition, graphics::Color::White, "Editor Menu", 40, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    const auto backToEditorButtonPosition = utils::Vector2f{28, 15};
    const auto backToEditorButtonOnMouseOver = [&] {
        uiManager->setColor(components::ui::UIComponentType::Button, "editorMenuBackToEditorButton",
                            buttonHoverColor);
    };
    const auto backToEditorButtonOnMouseOut = [&] {
        uiManager->setColor(components::ui::UIComponentType::Button, "editorMenuBackToEditorButton",
                            buttonColor);
    };
    auto backToEditorButtonMouseOverActions =
        components::ui::MouseOverActions{backToEditorButtonOnMouseOver, backToEditorButtonOnMouseOut};
    auto backToEditorClickAction = [this] { shouldBackToEditor = true; };
    auto backToEditorButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuBackToEditorButton", backToEditorButtonPosition, buttonSize, buttonColor,
        "Back to editor", textColor, 27, fontPath, utils::Vector2f{1.5, 0.75}, backToEditorClickAction,
        backToEditorButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToEditorButtonConfig));

    const auto loadMapButtonPosition = utils::Vector2f{28, 23};
    const auto loadMapButtonOnMouseOver = [&] {
        uiManager->setColor(components::ui::UIComponentType::Button, "editorMenuLoadMapButton",
                            buttonHoverColor);
    };
    const auto loadMapButtonOnMouseOut = [&] {
        uiManager->setColor(components::ui::UIComponentType::Button, "editorMenuLoadMapButton", buttonColor);
    };
    auto loadMapButtonMouseOverActions =
        components::ui::MouseOverActions{loadMapButtonOnMouseOver, loadMapButtonOnMouseOut};
    auto loadMapClickAction = [this] { std::cout << "load map\n"; };
    auto loadMapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuLoadMapButton", loadMapButtonPosition, buttonSize, buttonColor, "Load map", textColor, 27,
        fontPath, utils::Vector2f{7, 0.75}, loadMapClickAction, loadMapButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(loadMapButtonConfig));

    const auto newMapButtonPosition = utils::Vector2f{28, 31};
    const auto newMapButtonOnMouseOver = [&] {
        uiManager->setColor(components::ui::UIComponentType::Button, "editorMenuNewMapButton",
                            buttonHoverColor);
    };
    const auto newMapButtonOnMouseOut = [&] {
        uiManager->setColor(components::ui::UIComponentType::Button, "editorMenuNewMapButton", buttonColor);
    };
    auto newMapButtonMouseOverActions =
        components::ui::MouseOverActions{newMapButtonOnMouseOver, newMapButtonOnMouseOut};
    auto newMapClickAction = [this] { std::cout << "new map\n"; };
    auto newMapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuNewMapButton", newMapButtonPosition, buttonSize, buttonColor, "New map", textColor, 27,
        fontPath, utils::Vector2f{7, 0.75}, newMapClickAction, newMapButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(newMapButtonConfig));

    const auto saveMapButtonPosition = utils::Vector2f{28, 39};
    const auto saveMapButtonOnMouseOver = [&] {
        uiManager->setColor(components::ui::UIComponentType::Button, "editorMenuSaveMapButton",
                            buttonHoverColor);
    };
    const auto saveMapButtonOnMouseOut = [&] {
        uiManager->setColor(components::ui::UIComponentType::Button, "editorMenuSaveMapButton", buttonColor);
    };
    auto saveMapButtonMouseOverActions =
        components::ui::MouseOverActions{saveMapButtonOnMouseOver, saveMapButtonOnMouseOut};
    auto saveMapClickAction = [this] {
        states.top()->deactivate();
        states.push(std::make_unique<SaveMapState>(window, inputManager, rendererPool, states));
    };
    auto saveMapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuSaveMapButton", saveMapButtonPosition, buttonSize, buttonColor, "Save map", textColor, 27,
        fontPath, utils::Vector2f{6, 0.75}, saveMapClickAction, saveMapButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(saveMapButtonConfig));

    const auto backToMenuButtonPosition = utils::Vector2f{28, 47};
    const auto backToMenuButtonOnMouseOver = [&] {
        uiManager->setColor(components::ui::UIComponentType::Button, "editorMenuBackToMenuButton",
                            buttonHoverColor);
    };
    const auto backToMenuButtonOnMouseOut = [&] {
        uiManager->setColor(components::ui::UIComponentType::Button, "editorMenuBackToMenuButton",
                            buttonColor);
    };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [this] { shouldBackToMenu = true; };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuBackToMenuButton", backToMenuButtonPosition, buttonSize, buttonColor, "Back to menu",
        textColor, 27, fontPath, utils::Vector2f{2.75, 0.75}, backToMenuClickAction,
        backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    return std::make_unique<components::ui::UIConfig>(std::move(backgroundConfig), std::move(buttonsConfig),
                                                      std::move(checkBoxesConfig), std::move(labelsConfig),
                                                      std::move(textFieldsConfig));
}

}