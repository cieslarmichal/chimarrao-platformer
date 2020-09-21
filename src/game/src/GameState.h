#pragma once

#include <memory>
#include <vector>

#include "DeltaTime.h"
#include "InputManager.h"
#include "RendererPool.h"
#include "Window.h"

namespace game
{
class GameState
{
public:
    explicit GameState(std::shared_ptr<gui::Window>, std::shared_ptr<input::InputManager>,
                       std::shared_ptr<graphics::RendererPool>);
    virtual ~GameState() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual void lateUpdate(const utils::DeltaTime&) = 0;
    virtual void render() = 0;
    virtual void checkIfEnded() = 0;

protected:
    std::shared_ptr<input::InputManager> inputManager;
    std::shared_ptr<gui::Window> window;
    std::shared_ptr<graphics::RendererPool> rendererPool;

private:
    bool stateIsEnded;
};
}