#pragma once

#include <memory>

#include "CheckBoxConfig.h"
#include "InputManager.h"
#include "RendererPool.h"
#include "Timer.h"
#include "UIComponent.h"
#include "core/ComponentOwner.h"

namespace components::ui
{
class CheckBox : public UIComponent
{
public:
    CheckBox(const std::shared_ptr<input::InputManager>&, const std::shared_ptr<graphics::RendererPool>&,
             std::unique_ptr<CheckBoxConfig>);

    void update(utils::DeltaTime) override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void setChecked(bool checked);

private:
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
    bool checkBoxClickActionFrozen = true;
    utils::Timer freezeClickableCheckBoxTimer;
    const float timeAfterCheckBoxCanBeClicked;
};
}