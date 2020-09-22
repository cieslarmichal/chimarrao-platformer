#pragma once

#include <memory>

#include "Component.h"
#include "RendererPool.h"
#include "Vector.h"
#include "SFML/Graphics/Text.hpp"

namespace components
{
class TextComponent : public Component
{
public:
    TextComponent(ComponentOwner*, const utils::Vector2f& position, const std::string& text, );

    void lateUpdate(utils::DeltaTime) override;
    const graphics::GraphicsId& getGraphicsId();
    void setColor(const graphics::Color&);

private:
    std::shared_ptr<graphics::RendererPool> rendererPool;
    graphics::GraphicsId id;
    sf::Text text;
};
}
