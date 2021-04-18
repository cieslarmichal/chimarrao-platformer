#pragma once

#include <memory>
#include <vector>

#include "DeltaTime.h"
#include "Input.h"
#include "NextState.h"
#include "RendererPool.h"
#include "States.h"
#include "Window.h"
#include "WindowObserver.h"
#include "StateType.h"

namespace sf
{
class Event;
}

namespace game
{
class State : public window::WindowObserver
{
public:
    explicit State(std::shared_ptr<window::Window>, std::shared_ptr<graphics::RendererPool>, States&);
    virtual ~State();

    virtual NextState update(const utils::DeltaTime&, const input::Input&) = 0;
    virtual void lateUpdate(const utils::DeltaTime&) = 0;
    virtual void render() = 0;
    virtual StateType getType() const = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    void handleWindowSizeChange(const utils::Vector2u& windowSize) override;

protected:
    std::shared_ptr<window::Window> window;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    States& states;
    bool active;
};
}