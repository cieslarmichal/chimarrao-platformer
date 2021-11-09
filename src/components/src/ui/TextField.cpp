#include "TextField.h"

#include "core/BoxColliderComponent.h"
#include "core/ClickableComponent.h"
#include "core/GraphicsComponent.h"
#include "core/MouseOverComponent.h"
#include "core/TextComponent.h"
#include "exceptions/UIComponentConfigNotFound.h"

namespace components::ui
{

TextField::TextField(const std::shared_ptr<core::SharedContext>& sharedContext,
                     std::unique_ptr<TextFieldConfig> textFieldConfig, std::unique_ptr<utils::Timer> timer)
    : inputBufferMaximumSize{15},
      timeAfterNextLetterCanBeDeleted{0.08f},
      deleteCharactersTimer{std::move(timer)}
{
    if (not textFieldConfig)
    {
        throw exceptions::UIComponentConfigNotFound{"TextField config not found"};
    }

    name = textFieldConfig->uniqueName;
    coreComponentsOwner =
        std::make_unique<components::core::ComponentOwner>(textFieldConfig->position, name, sharedContext);
    coreComponentsOwner->addGraphicsComponent(sharedContext->rendererPool, textFieldConfig->size,
                                              textFieldConfig->position, textFieldConfig->color,
                                              graphics::VisibilityLayer::First, utils::Vector2f{0, 0}, true);
    coreComponentsOwner->addComponent<components::core::TextComponent>(
        sharedContext->rendererPool, textFieldConfig->size, "", textFieldConfig->fontPath,
        textFieldConfig->fontSize, graphics::Color::White, textFieldConfig->textOffset, true);
    coreComponentsOwner->addComponent<components::core::BoxColliderComponent>(textFieldConfig->size);

    clickInsideFieldAction = textFieldConfig->clickInFieldAction;
    mouseOutFieldAction = textFieldConfig->mouseOverActions->mouseOutAction;

    auto textFieldClickedAction = [&]
    {
        textFieldClicked = true;
        clickInsideFieldAction();
    };

    coreComponentsOwner->addComponent<components::core::ClickableComponent>(
        std::move(textFieldClickedAction));

    auto mouseOutAction = [&]
    {
        textFieldClicked = false;
        mouseOutFieldAction();
    };

    if (textFieldConfig->mouseOverActions)
    {
        coreComponentsOwner->addComponent<components::core::MouseOverComponent>(
            textFieldConfig->mouseOverActions->mouseOverAction, mouseOutAction);
    }

    coreComponentsOwner->loadDependentComponents();
}

void TextField::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    if (input.isKeyPressed(input::InputKey::MouseLeft))
    {
        if (auto textFieldHitBox =
                coreComponentsOwner->getComponent<components::core::BoxColliderComponent>())
        {
            if (not textFieldHitBox->intersects(input.getMousePosition()))
            {
                textFieldClicked = false;
                mouseOutFieldAction();
            }
        }
    }

    if (textFieldClicked)
    {
        for (const auto& alphanumericButtonKey : input::alphaNumericalButtons)
        {
            if (input.isKeyReleased(alphanumericButtonKey))
            {
                if (inputBuffer.size() < inputBufferMaximumSize)
                {
                    inputBuffer += utils::StringHelper::getLowerCases(toString(alphanumericButtonKey));
                    setText(inputBuffer);
                }
            }
        }

        if (input.isKeyPressed(input::InputKey::Backspace))
        {
            if (deleteCharactersTimer->getElapsedSeconds() > timeAfterNextLetterCanBeDeleted)
            {
                if (not inputBuffer.empty())
                {
                    utils::StringHelper::cutOffString(inputBuffer, inputBuffer.size() - 1,
                                                      inputBuffer.size() - 1);
                    setText(inputBuffer);
                }

                deleteCharactersTimer->restart();
            }
        }
    }

    coreComponentsOwner->update(deltaTime, input);
    coreComponentsOwner->lateUpdate(deltaTime, input);
}

std::string TextField::getName() const
{
    return name;
}

void TextField::activate()
{
    textFieldClicked = false;
    coreComponentsOwner->enable();
    deleteCharactersTimer->restart();
}

void TextField::deactivate()
{
    textFieldClicked = false;
    coreComponentsOwner->disable();
}

void TextField::setColor(graphics::Color color)
{
    coreComponentsOwner->getMainGraphicsComponent()->setColor(color);
}

void TextField::setText(const std::string& text)
{
    coreComponentsOwner->getComponent<components::core::TextComponent>()->setText(text);
    inputBuffer = text;
}

std::string TextField::getText() const
{
    return inputBuffer;
}

bool TextField::isActive() const
{
    return coreComponentsOwner->areComponentsEnabled();
}

void TextField::select()
{
    if (auto clickableComponent = coreComponentsOwner->getComponent<components::core::ClickableComponent>())
    {
        clickableComponent->invokeKeyAction(input::InputKey::MouseLeft);
    }
}

void TextField::invokeMouseOutAction()
{
    textFieldClicked = false;
}

}