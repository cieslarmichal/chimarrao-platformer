#include "TextField.h"

#include "core/ClickableComponent.h"
#include "core/GraphicsComponent.h"
#include "core/HitBoxComponent.h"
#include "core/MouseOverComponent.h"
#include "core/TextComponent.h"
#include "exceptions/UIComponentConfigNotFound.h"

namespace components::ui
{

TextField::TextField(const std::shared_ptr<graphics::RendererPool>& rendererPool,
                     std::unique_ptr<TextFieldConfig> textFieldConfig)
    : inputBufferMaximumSize{15}, timeAfterNextLetterCanBeDeleted{0.08f}
{
    if (not textFieldConfig)
    {
        throw exceptions::UIComponentConfigNotFound{"TextField config not found"};
    }

    name = textFieldConfig->uniqueName;
    coreComponentsOwner = std::make_unique<components::core::ComponentOwner>(textFieldConfig->position, name);
    coreComponentsOwner->addComponent<components::core::GraphicsComponent>(
        rendererPool, textFieldConfig->size, textFieldConfig->position, textFieldConfig->color,
        graphics::VisibilityLayer::First);
    coreComponentsOwner->addComponent<components::core::TextComponent>(
        rendererPool, textFieldConfig->size, "", textFieldConfig->fontPath, textFieldConfig->fontSize,
        graphics::Color::White, textFieldConfig->textOffset);
    coreComponentsOwner->addComponent<components::core::HitBoxComponent>(textFieldConfig->size);

    clickInsideFieldAction = textFieldConfig->clickInFieldAction;
    clickOutsideFieldAction = textFieldConfig->clickOutsideFieldAction;

    auto textFieldClickedAction = [&]
    {
        textFieldClicked = true;
        clickInsideFieldAction();
    };

    coreComponentsOwner->addComponent<components::core::ClickableComponent>(
        std::move(textFieldClickedAction));

    coreComponentsOwner->loadDependentComponents();
    deleteCharactersTimer.start();
}

void TextField::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    if (input.isKeyPressed(input::InputKey::MouseLeft))
    {
        if (auto textFieldHitBox = coreComponentsOwner->getComponent<components::core::HitBoxComponent>())
        {
            if (not textFieldHitBox->intersects(input.getMousePosition()))
            {
                textFieldClicked = false;
                clickOutsideFieldAction(inputBuffer);
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
            if (deleteCharactersTimer.getElapsedSeconds() > timeAfterNextLetterCanBeDeleted)
            {
                if (not inputBuffer.empty())
                {
                    utils::StringHelper::cutOffString(inputBuffer, inputBuffer.size() - 1,
                                                      inputBuffer.size() - 1);
                    setText(inputBuffer);
                }

                deleteCharactersTimer.restart();
            }
        }
    }

    coreComponentsOwner->update(deltaTime, input);
    coreComponentsOwner->lateUpdate(deltaTime);
}

std::string TextField::getName() const
{
    return name;
}

void TextField::activate()
{
    textFieldClicked = false;
    coreComponentsOwner->enable();
    deleteCharactersTimer.restart();
}

void TextField::deactivate()
{
    textFieldClicked = false;
    coreComponentsOwner->disable();
}

void TextField::setColor(graphics::Color color)
{
    coreComponentsOwner->getComponent<components::core::GraphicsComponent>()->setColor(color);
}

void TextField::setText(const std::string& text)
{
    coreComponentsOwner->getComponent<components::core::TextComponent>()->setText(text);
}

}