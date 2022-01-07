#pragma once

#include <memory>

#include "SFML/Graphics/Text.hpp"

#include "Component.h"
#include "RendererPool.h"
#include "TextComponent.h"
#include "Vector.h"

namespace components::core
{
class DialogueTextComponent : public Component
{
public:
    DialogueTextComponent(ComponentOwner*, std::shared_ptr<graphics::RendererPool>,
                          const utils::Vector2f& position, const std::string& text, const graphics::FontPath&,
                          unsigned characterSize, float initialOffsetForCalculatingOffsetPosition = 0,
                          const graphics::Color& = graphics::Color::Black,
                          const utils::Vector2f& transformOffset = utils::Vector2f{0, 0},
                          bool relativeRendering = false);

    void setText(const std::string& text);
    void enable() override;
    void disable() override;

private:
    std::unordered_map<unsigned, float> numberOfCharactersToOffset;
    std::shared_ptr<TextComponent> textComponent;
    const unsigned maximumTextLength{100};
};
}
