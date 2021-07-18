#include "Button.h"

#include "core/BoxColliderComponent.h"
#include "core/ClickableComponent.h"
#include "core/GraphicsComponent.h"
#include "core/MouseOverComponent.h"
#include "core/TextComponent.h"
#include "exceptions/UIComponentConfigNotFound.h"

namespace components::ui
{

Button::Button(const std::shared_ptr<graphics::RendererPool>& rendererPool,
               std::unique_ptr<ButtonConfig> buttonConfig, std::unique_ptr<utils::Timer> timer)
    : timeAfterButtonCanBeClicked{0.25f}, freezeClickableButtonTimer{std::move(timer)}
{
    if (not buttonConfig)
    {
        throw exceptions::UIComponentConfigNotFound{"Button config not found"};
    }

    name = buttonConfig->uniqueName;
    coreComponentsOwner = std::make_unique<components::core::ComponentOwner>(buttonConfig->position, name);
    coreComponentsOwner->addGraphicsComponent(rendererPool, buttonConfig->size, buttonConfig->position,
                                              buttonConfig->buttonColor, graphics::VisibilityLayer::First,
                                              utils::Vector2f{0, 0}, true);
    coreComponentsOwner->addComponent<components::core::TextComponent>(
        rendererPool, buttonConfig->position, buttonConfig->text, buttonConfig->fontPath,
        buttonConfig->fontSize, buttonConfig->textColor, buttonConfig->textOffset, true);
    coreComponentsOwner->addComponent<components::core::BoxColliderComponent>(buttonConfig->size);

    if (buttonConfig->clickAction)
    {
        coreComponentsOwner->addComponent<components::core::ClickableComponent>(*buttonConfig->clickAction);
    }

    if (buttonConfig->mouseOverActions)
    {
        coreComponentsOwner->addComponent<components::core::MouseOverComponent>(
            buttonConfig->mouseOverActions->mouseOverAction, buttonConfig->mouseOverActions->mouseOutAction);
    }

    coreComponentsOwner->loadDependentComponents();

    if (auto clickableComponent = coreComponentsOwner->getComponent<components::core::ClickableComponent>())
    {
        clickableComponent->disable();
    }
}

void Button::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    if (buttonClickActionFrozen &&
        freezeClickableButtonTimer->getElapsedSeconds() > timeAfterButtonCanBeClicked)
    {
        buttonClickActionFrozen = false;
        if (auto clickableComponent =
                coreComponentsOwner->getComponent<components::core::ClickableComponent>())
        {
            clickableComponent->enable();
        }
    }

    coreComponentsOwner->update(deltaTime, input);
    coreComponentsOwner->lateUpdate(deltaTime, input);
}

std::string Button::getName() const
{
    return name;
}

void Button::activate()
{
    coreComponentsOwner->enable();
    restartClickActionFreezeTimer();
}

void Button::deactivate()
{
    coreComponentsOwner->disable();
    buttonClickActionFrozen = true;
}

void Button::setColor(graphics::Color color)
{
    coreComponentsOwner->getMainGraphicsComponent()->setColor(color);
}

void Button::setText(const std::string& text)
{
    coreComponentsOwner->getComponent<components::core::TextComponent>()->setText(text);
}

std::string Button::getText() const
{
    return coreComponentsOwner->getComponent<components::core::TextComponent>()->getText();
}

void Button::setClickAction(const std::vector<core::KeyAction>& keyActions)
{
    if (auto clickableComponent = coreComponentsOwner->getComponent<components::core::ClickableComponent>())
    {
        clickableComponent->setKeyActions(keyActions);
    }
}

void Button::invokeClickAction(input::InputKey inputKey)
{
    if (auto clickableComponent = coreComponentsOwner->getComponent<components::core::ClickableComponent>())
    {
        clickableComponent->invokeKeyAction(inputKey);
    }
}

bool Button::isActive() const
{
    return coreComponentsOwner->areComponentEnabled();
}

void Button::restartClickActionFreezeTimer()
{
    if (auto clickableComponent = coreComponentsOwner->getComponent<components::core::ClickableComponent>())
    {
        clickableComponent->disable();
    }
    freezeClickableButtonTimer->restart();
    buttonClickActionFrozen = true;
}

}