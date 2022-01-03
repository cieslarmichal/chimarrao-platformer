#include "MenuState.h"

#include <utility>

#include "MenuStateUIConfigBuilder.h"
#include "TimerFactory.h"
#include "navigators/PaginatedButtonsNavigator.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
}

MenuState::MenuState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                     std::shared_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      uiManager{std::move(uiManagerInit)},
      buttonsNavigator{std::make_unique<GridButtonsNavigator>(
          uiManager, MenuStateUIConfigBuilder::getGridButtonsInfo(), MenuStateUIConfigBuilder::getIconNames(),
          buttonColor, buttonHoverColor, utils::TimerFactory::createTimer(),
          utils::TimerFactory::createTimer())},
      shouldExit{false}
{
    uiManager->createUI(MenuStateUIConfigBuilder::createMenuUIConfig(this));
    buttonsNavigator->initialize();
}

NextState MenuState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (const auto nextState = buttonsNavigator->update(deltaTime, input); nextState == NextState::Previous)
    {
        return NextState::Exit;
    }

    uiManager->update(deltaTime, input);

    if (shouldExit)
    {
        return NextState::Exit;
    }

    return NextState::Same;
}

void MenuState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void MenuState::render()
{
    rendererPool->renderAll();
}

StateType MenuState::getType() const
{
    return StateType::Menu;
}

void MenuState::activate()
{
    active = true;
    uiManager->activate();
    buttonsNavigator->activate();
}

void MenuState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void MenuState::handleWindowSizeChange(const utils::Vector2u& windowSize)
{
    State::handleWindowSizeChange(windowSize);
    uiManager->freezeAllButtons();
}

void MenuState::newGame()
{
    states.deactivateCurrentState();
    states.addNextState(StateType::StoryGame);
}

void MenuState::chooseMap()
{
    states.deactivateCurrentState();
    states.addNextState(StateType::ChooseMap);
}

void MenuState::runMapEditor()
{
    states.deactivateCurrentState();
    states.addNextState(StateType::Editor);
}

void MenuState::runControls()
{
    states.deactivateCurrentState();
    states.addNextState(StateType::Controls);
}

void MenuState::runSettings()
{
    states.deactivateCurrentState();
    states.addNextState(StateType::Settings);
}

void MenuState::exit()
{
    shouldExit = true;
}

}