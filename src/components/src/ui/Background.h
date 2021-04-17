#pragma once

#include <memory>

#include "BackgroundConfig.h"
#include "InputManager.h"
#include "RendererPool.h"
#include "UIComponent.h"
#include "core/ComponentOwner.h"

namespace components::ui
{
class Background : public UIComponent
{
public:
    Background(const std::shared_ptr<graphics::RendererPool>&, std::unique_ptr<BackgroundConfig>);

    void update(utils::DeltaTime, const input::Input&) override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void setColor(graphics::Color);

private:
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
};
}