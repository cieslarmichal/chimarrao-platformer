#pragma once

#include <memory>

#include "ImageConfig.h"
#include "RendererPool.h"
#include "UIComponent.h"
#include "core/ComponentOwner.h"
#include "core/KeyAction.h"

namespace components::ui
{
class Image : public UIComponent
{
public:
    Image(const std::shared_ptr<core::SharedContext>&, std::unique_ptr<ImageConfig>);

    void update(utils::DeltaTime, const input::Input&) override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    bool isActive() const override;
    utils::Vector2f getSize() const;
    void setSize(const utils::Vector2f&);
    void setOutline(float thickness, const sf::Color& color);
    void setTexture(const graphics::TexturePath&);

private:
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
};
}