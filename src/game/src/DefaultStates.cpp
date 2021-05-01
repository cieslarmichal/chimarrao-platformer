#include "DefaultStates.h"

namespace game
{

DefaultStates::DefaultStates(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             const std::shared_ptr<utils::FileAccess>& fileAccessInit,
                             std::shared_ptr<TileMap> tileMapInit)
    : tileMap{tileMapInit},
      stateFactory{
          std::make_unique<StateFactory>(windowInit, rendererPoolInit, fileAccessInit, *this, tileMap)}
{
}

StatesStatus DefaultStates::updateCurrentState(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (states.empty())
    {
        return StatesStatus::Exit;
    }

    const auto nextState = states.top()->update(deltaTime, input);

    if (nextState == NextState::Previous)
    {
        backToThePreviousState();
        return StatesStatus::Running;
    }
    else if (nextState == NextState::Menu)
    {
        backToTheMenuState();
        return StatesStatus::Running;
    }
    else if (nextState == NextState::Exit)
    {
        return StatesStatus::Exit;
    }

    states.top()->lateUpdate(deltaTime);
    return StatesStatus::Running;
}

void DefaultStates::deactivateCurrentState()
{
    states.top()->deactivate();
}

void DefaultStates::addNextState(StateType stateType)
{
    states.push(stateFactory->createState(stateType));
}

void DefaultStates::render()
{
    if (not states.empty())
    {
        states.top()->render();
    }
}

void DefaultStates::backToThePreviousState()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void DefaultStates::backToTheMenuState()
{
    while (not states.empty() && states.top()->getType() != StateType::Menu)
    {
        states.pop();
    }

    if (not states.empty())
    {
        states.top()->activate();
    }
}

}