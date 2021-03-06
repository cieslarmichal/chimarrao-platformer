#include "Button.h"

#include "core/ClickableComponent.h"
#include "core/GraphicsComponent.h"
#include "core/HitBoxComponent.h"
#include "core/MouseOverComponent.h"
#include "core/TextComponent.h"
#include "exceptions/UIComponentConfigNotFound.h"

namespace components::ui
{

Button::Button(const std::shared_ptr<input::InputManager>& inputManager,
               const std::shared_ptr<graphics::RendererPool>& rendererPool,
               std::unique_ptr<ButtonConfig> buttonConfig)
    : timeAfterButtonCanBeClicked{0.25f}
{
    if (not buttonConfig)
    {
        throw exceptions::UIComponentConfigNotFound{"Button config not found"};
    }

    name = buttonConfig->uniqueName->getName();
    coreComponentsOwner = std::make_unique<components::core::ComponentOwner>(buttonConfig->position);
    coreComponentsOwner->addComponent<components::core::GraphicsComponent>(
        rendererPool, buttonConfig->size, buttonConfig->position, buttonConfig->color,
        graphics::VisibilityLayer::First);
    coreComponentsOwner->addComponent<components::core::TextComponent>(
        rendererPool, buttonConfig->position, buttonConfig->text, buttonConfig->fontPath,
        buttonConfig->fontSize, buttonConfig->color, buttonConfig->textOffset);
    coreComponentsOwner->addComponent<components::core::HitBoxComponent>(buttonConfig->size);
    coreComponentsOwner->addComponent<components::core::ClickableComponent>(
        inputManager, std::move(buttonConfig->clickAction));

    if (buttonConfig->mouseOverActions)
    {
        coreComponentsOwner->addComponent<components::core::MouseOverComponent>(
            inputManager, buttonConfig->mouseOverActions->mouseOverAction,
            buttonConfig->mouseOverActions->mouseOutAction);
    }

    coreComponentsOwner->loadDependentComponents();
    coreComponentsOwner->getComponent<components::core::ClickableComponent>()->disable();

    freezeClickableButtonTimer.start();
}

void Button::update(utils::DeltaTime deltaTime)
{
    if (buttonClickActionFrozen &&
        freezeClickableButtonTimer.getElapsedSeconds() > timeAfterButtonCanBeClicked)
    {
        buttonClickActionFrozen = false;
        coreComponentsOwner->getComponent<components::core::ClickableComponent>()->enable();
    }

    coreComponentsOwner->update(deltaTime);
    coreComponentsOwner->lateUpdate(deltaTime);
}

std::string Button::getName() const
{
    return name;
}

void Button::activate()
{
    coreComponentsOwner->enable();
    coreComponentsOwner->getComponent<components::core::ClickableComponent>()->disable();
    freezeClickableButtonTimer.restart();
    buttonClickActionFrozen = true;
}

void Button::deactivate()
{
    coreComponentsOwner->disable();
    buttonClickActionFrozen = true;
}

void Button::setColor(graphics::Color color)
{
    coreComponentsOwner->getComponent<components::core::GraphicsComponent>()->setColor(color);
}

void Button::setText(const std::string& text)
{
    coreComponentsOwner->getComponent<components::core::TextComponent>()->setText(text);
}

}