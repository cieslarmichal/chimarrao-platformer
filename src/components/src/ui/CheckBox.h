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
    CheckBox(const std::shared_ptr<graphics::RendererPool>&, std::unique_ptr<CheckBoxConfig>);

    void update(utils::DeltaTime, const input::Input&) override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void setColor(graphics::Color);
    void setChecked(bool checked);
    bool isActive() const override;

private:
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
    bool checkBoxClickActionFrozen = true;
    utils::Timer freezeClickableCheckBoxTimer;
    const float timeAfterCheckBoxCanBeClicked;
};
}