#pragma once

#include <vector>
#include "Window.h"
#include "RendererPool.h"
#include "DeltaTime.h"
#include <memory>
#include "InputManager.h"

namespace game
{
class GameState
{
public:
    explicit GameState(std::shared_ptr<graphics::Window>, InputManager&, std::shared_ptr<graphics::RendererPool>);
    virtual ~GameState() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual void render() = 0;
    virtual void checkIfEnded() = 0;

protected:
    InputManager& inputManager;
    std::shared_ptr<graphics::Window> window;
    std::shared_ptr<graphics::RendererPool> rendererPool;

private:
    bool stateIsEnded;
};
}