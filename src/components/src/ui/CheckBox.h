#pragma once

#include <memory>

#include "CheckBoxConfig.h"
#include "ComponentOwner.h"
#include "InputManager.h"
#include "RendererPool.h"
#include "Timer.h"
#include "UIComponent.h"

namespace components::ui
{
class CheckBox : public UIComponent
{
public:
    CheckBox(const std::shared_ptr<core::SharedContext>&, std::unique_ptr<CheckBoxConfig>,
             std::unique_ptr<utils::Timer>);

    void update(utils::DeltaTime, const input::Input&) override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void setColor(graphics::Color);
    void setChecked(bool isChecked);
    void toggle();
    void invokeMouseOutAction();
    bool isActive() const override;

private:
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
    bool checkBoxClickActionFrozen = true;
    const float timeAfterCheckBoxCanBeClicked;
    std::unique_ptr<utils::Timer> freezeClickableCheckBoxTimer;
    bool checked{false};
};
}