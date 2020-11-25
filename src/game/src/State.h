#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "DeltaTime.h"
#include "InputManager.h"
#include "RendererPool.h"
#include "Window.h"
#include "WindowObserver.h"

namespace sf
{
class Event;
}

namespace game
{
class State : public window::WindowObserver
{
public:
    explicit State(std::shared_ptr<window::Window>, std::shared_ptr<input::InputManager>,
                   std::shared_ptr<graphics::RendererPool>, std::stack<std::unique_ptr<State>>&);
    virtual ~State();

    virtual void update(const utils::DeltaTime&) = 0;
    virtual void lateUpdate(const utils::DeltaTime&) = 0;
    virtual void render() = 0;
    virtual std::string getName() const = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    void handleWindowSizeChange(const utils::Vector2u& windowSize) override;

protected:
    std::shared_ptr<window::Window> window;
    std::shared_ptr<input::InputManager> inputManager;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    std::stack<std::unique_ptr<State>>& states;
    bool active;
};
}