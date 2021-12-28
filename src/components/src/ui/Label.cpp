#include "Label.h"

#include "TextComponent.h"
#include "exceptions/UIComponentConfigNotFound.h"

namespace components::ui
{
Label::Label(const std::shared_ptr<core::SharedContext>& sharedContext,
             std::unique_ptr<LabelConfig> labelConfig)
{
    if (not labelConfig)
    {
        throw exceptions::UIComponentConfigNotFound{"Label config not found"};
    }

    name = labelConfig->uniqueName;
    coreComponentsOwner =
        std::make_unique<components::core::ComponentOwner>(labelConfig->position, name, sharedContext);
    coreComponentsOwner->addComponent<components::core::TextComponent>(
        sharedContext->rendererPool, labelConfig->position, labelConfig->text, labelConfig->fontPath,
        labelConfig->fontSize, labelConfig->color, utils::Vector2f{0, 0}, true);

    coreComponentsOwner->loadDependentComponents();
}

void Label::update(utils::DeltaTime, const input::Input&) {}

std::string Label::getName() const
{
    return name;
}

void Label::activate()
{
    coreComponentsOwner->enable();
}

void Label::deactivate()
{
    coreComponentsOwner->disable();
}

void Label::setText(const std::string& text)
{
    coreComponentsOwner->getComponent<components::core::TextComponent>()->setText(text);
}

std::string Label::getText() const
{
    return coreComponentsOwner->getComponent<components::core::TextComponent>()->getText();
}

void Label::setColor(graphics::Color color)
{
    coreComponentsOwner->getComponent<components::core::TextComponent>()->setColor(color);
}

bool Label::isActive() const
{
    return coreComponentsOwner->areComponentsEnabled();
}
}