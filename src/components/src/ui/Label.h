#pragma once

#include <memory>

#include "LabelConfig.h"
#include "RendererPool.h"
#include "Timer.h"
#include "UIComponent.h"
#include "core/ComponentOwner.h"

namespace components::ui
{
class Label : public UIComponent
{
public:
    Label(const std::shared_ptr<graphics::RendererPool>&, std::unique_ptr<LabelConfig>);

    void update(utils::DeltaTime, const input::Input&) override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void setText(const std::string&);
    std::string getText() const;
    void setColor(graphics::Color);
    bool isActive() const override;

private:
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
};
}