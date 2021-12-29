#pragma once

#include <memory>

#include "BackgroundConfig.h"
#include "ComponentOwner.h"
#include "InputManager.h"
#include "RendererPool.h"
#include "UIComponent.h"

namespace components::ui
{
class Background : public UIComponent
{
public:
    Background(const std::shared_ptr<core::SharedContext>&, std::unique_ptr<BackgroundConfig>);

    void update(utils::DeltaTime, const input::Input&) override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void setColor(graphics::Color);
    bool isActive() const override;

private:
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
};
}