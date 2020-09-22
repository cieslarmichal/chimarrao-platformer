#pragma once

#include <memory>

#include "SFML/Graphics/Text.hpp"

#include "Component.h"
#include "RendererPool.h"
#include "Vector.h"

namespace components
{
class TextComponent : public Component
{
public:
    TextComponent(ComponentOwner*, std::shared_ptr<graphics::RendererPool>, const utils::Vector2f& position,
                  const std::string& text, const graphics::FontPath&, unsigned characterSize,
                  const graphics::Color&, const utils::Vector2f& transformOffset = utils::Vector2f{0, 0});

    void lateUpdate(utils::DeltaTime) override;
    const graphics::GraphicsId& getGraphicsId();
    void setColor(const graphics::Color&);

private:
    std::shared_ptr<graphics::RendererPool> rendererPool;
    graphics::GraphicsId id;
    utils::Vector2f transformOffset;
};
}
