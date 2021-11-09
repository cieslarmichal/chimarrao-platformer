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
    Button(const std::shared_ptr<core::SharedContext>&, std::unique_ptr<ButtonConfig>,
           std::unique_ptr<utils::Timer>);

    void update(utils::DeltaTime, const input::Input&) override;
    std::string getName() const override;
    void deactivate() override;
    void activate() override;
    void setColor(graphics::Color);
    void setText(const std::string&);
    std::string getText() const;
    void setClickAction(const std::vector<core::KeyAction>&);
    void invokeClickAction(input::InputKey);
    bool isActive() const override;
    void restartClickActionFreezeTimer();

private:
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
    bool buttonClickActionFrozen = true;
    const float timeAfterButtonCanBeClicked;
    std::unique_ptr<utils::Timer> freezeClickableButtonTimer;
};
}