#include "PauseState.h"

#include "GetProjectPath.h"
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

PauseState::PauseState(const std::shared_ptr<window::Window>& windowInit,
                       const std::shared_ptr<input::InputManager>& inputManagerInit,
                       const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                       std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToGame{false},
      shouldBackToMenu{false},
      uiManager{std::make_unique<components::ui::DefaultUIManager>(inputManagerInit, rendererPoolInit,
                                                                   createSettingsUIConfig())}
{
    inputManager->registerObserver(this);
    timer.start();
}

PauseState::~PauseState()
{
    inputManager->removeObserver(this);
}

void PauseState::update(const utils::DeltaTime& deltaTime)
{
    if (timer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        shouldBackToGame = true;
    }

    if (shouldBackToGame)
    {
        backToGame();
        return;
    }

    if (shouldBackToMenu)
    {
        backToMenu();
        return;
    }
    uiManager->update(deltaTime);
}

void PauseState::lateUpdate(const utils::DeltaTime& deltaTime)
{
}

void PauseState::render()
{
    rendererPool->renderAll();
}

std::string PauseState::getName() const
{
    return "Pause state";
}

void PauseState::activate()
{
    active = true;
    uiManager->activate();
}

void PauseState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void PauseState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void PauseState::backToGame()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void PauseState::backToMenu()
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

std::unique_ptr<components::ui::UIConfig> PauseState::createSettingsUIConfig()
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> checkBoxesConfig;
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;
    std::vector<std::unique_ptr<components::ui::TextFieldConfig>> textFieldsConfig;


    auto backgroundConfig = std::make_unique<components::ui::BackgroundConfig>(
        "pauseBackground", utils::Vector2f{25, 10}, utils::Vector2f{31, 32},
        graphics::VisibilityLayer::Background, graphics::Color{172});

    const auto textPausePosition = utils::Vector2f{35, 13};
    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "pauseTitleLabel", textPausePosition, graphics::Color::White, "Pause", 40, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

///////////////////////////////////////////////////////////////////////////////////////////////

    const auto backToGameButtonPosition = utils::Vector2f{28, 21};

    const auto backToGameButtonOnMouseOver = [&] {
      uiManager->setColor(components::ui::UIComponentType::Button, "pauseBackToGameButton",
                          buttonHoverColor);
    };
    const auto backToGameButtonOnMouseOut = [&] {
      uiManager->setColor(components::ui::UIComponentType::Button, "pauseBackToGameButton", buttonColor);
    };
    auto backToGameButtonMouseOverActions =
        components::ui::MouseOverActions{backToGameButtonOnMouseOver, backToGameButtonOnMouseOut};
    auto backToGameClickAction = [this] { shouldBackToGame = true; };
    auto backToGameButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "pauseBackToGameButton", backToGameButtonPosition, buttonSize, buttonColor, "Back to game",
        textColor, 30, fontPath, utils::Vector2f{2, 0.5}, backToGameClickAction,
        backToGameButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToGameButtonConfig));

//////////////////////////////////////////////////////////////////////////////////////////////

    const auto backToMenuButtonPosition = utils::Vector2f{28, 30};

    const auto backToMenuButtonOnMouseOver = [&] {
      uiManager->setColor(components::ui::UIComponentType::Button, "pauseBackToMenuButton",
                          buttonHoverColor);
    };
    const auto backToMenuButtonOnMouseOut = [&] {
      uiManager->setColor(components::ui::UIComponentType::Button, "pauseBackToMenuButton", buttonColor);
    };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [this] { shouldBackToMenu = true; };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "pauseBackToMenuButton", backToMenuButtonPosition, buttonSize, buttonColor, "Back to menu",
        textColor, 30, fontPath, utils::Vector2f{2, 0.5}, backToMenuClickAction,
        backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    return std::make_unique<components::ui::UIConfig>(std::move(backgroundConfig), std::move(buttonsConfig),
                                                      std::move(checkBoxesConfig), std::move(labelsConfig),
                                                      std::move(textFieldsConfig));
}

}