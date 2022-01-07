#include "DialogueTextComponent.h"

#include <utility>

#include "ComponentOwner.h"

namespace components::core
{

DialogueTextComponent::DialogueTextComponent(ComponentOwner* owner,
                                             std::shared_ptr<graphics::RendererPool> rendererPool,
                                             const utils::Vector2f& position, const std::string& text,
                                             const graphics::FontPath& fontPath, unsigned int characterSize,
                                             const graphics::Color& color,
                                             const utils::Vector2f& transformOffset, bool relativeRendering)
    : Component(owner)
{
    textComponent =
        owner->addComponent<TextComponent>(std::move(rendererPool), position, text, fontPath, characterSize,
                                           color, transformOffset, relativeRendering);
    int decreaseOffsetCounter = 0;
    const float decreaseOffsetValue = 0.5;
    float currentOffset = 3.f;

    numberOfCharactersToOffset.reserve(maximumTextLength);

    for (unsigned numberOfCharacters = 1; numberOfCharacters < maximumTextLength; numberOfCharacters++)
    {
        const auto offset = currentOffset - static_cast<float>(decreaseOffsetCounter) * decreaseOffsetValue;
        numberOfCharactersToOffset.insert({numberOfCharacters, offset});

        if (numberOfCharacters % 2 == 0)
        {
            decreaseOffsetCounter++;
        }
    }
}

void DialogueTextComponent::setText(const std::string& text)
{
    textComponent->setText(text);

    const auto numberOfCharactersInText = text.size();
    const auto offsetOnXAxis = numberOfCharactersToOffset[numberOfCharactersInText];
    const auto adjustedOffset = utils::Vector2f{offsetOnXAxis, textComponent->getOffset().y};

    textComponent->setOffset(adjustedOffset);
}

void DialogueTextComponent::enable()
{
    textComponent->enable();
}

void DialogueTextComponent::disable()
{
    textComponent->disable();
}
}