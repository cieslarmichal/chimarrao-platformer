#include "Label.h"

#include "core/TextComponent.h"
#include "exceptions/UIComponentConfigNotFound.h"

namespace components::ui
{
Label::Label(const std::shared_ptr<graphics::RendererPool>& rendererPool,
             std::unique_ptr<LabelConfig> labelConfig)
{
    if (not labelConfig)
    {
        throw exceptions::UIComponentConfigNotFound{"Label config not found"};
    }

    name = labelConfig->uniqueName;
    coreComponentsOwner = std::make_unique<components::core::ComponentOwner>(labelConfig->position, name);
    coreComponentsOwner->addComponent<components::core::TextComponent>(
        rendererPool, labelConfig->position, labelConfig->text, labelConfig->fontPath, labelConfig->fontSize,
        labelConfig->color, utils::Vector2f{0, 0}, true);

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

void Label::setColor(graphics::Color color)
{
    coreComponentsOwner->getComponent<components::core::TextComponent>()->setColor(color);
}

bool Label::isActive() const
{
    return coreComponentsOwner->areComponentEnabled();
}
}