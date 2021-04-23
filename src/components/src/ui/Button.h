#pragma once

#include <memory>

#include "ButtonConfig.h"
#include "InputManager.h"
#include "RendererPool.h"
#include "Timer.h"
#include "UIComponent.h"
#include "core/ComponentOwner.h"
#include "core/KeyAction.h"

namespace components::ui
{
class Button : public UIComponent
{
public:
    Button(const std::shared_ptr<graphics::RendererPool>&, std::unique_ptr<ButtonConfig>);

    void update(utils::DeltaTime, const input::Input&) override;
    std::string getName() const override;
    void deactivate() override;
    void activate() override;
    void setColor(graphics::Color);
    void setText(const std::string&);
    void setClickAction(const std::vector<core::KeyAction>&);
    bool isActive() const override;

private:
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
    bool buttonClickActionFrozen = true;
    utils::Timer freezeClickableButtonTimer;
    const float timeAfterButtonCanBeClicked;
};
}