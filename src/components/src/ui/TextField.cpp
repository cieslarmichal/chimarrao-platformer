#include "TextField.h"

#include "core/ClickableComponent.h"
#include "core/GraphicsComponent.h"
#include "core/HitBoxComponent.h"
#include "core/MouseOverComponent.h"
#include "core/TextComponent.h"
#include "exceptions/UIComponentConfigNotFound.h"

namespace components::ui
{

TextField::TextField(const std::shared_ptr<input::InputManager>& inputManager,
                     const std::shared_ptr<graphics::RendererPool>& rendererPool,
                     std::unique_ptr<TextFieldConfig> textFieldConfig)
    : inputManager{inputManager},
      inputStatus{nullptr},
      inputBufferMaximumSize{15},
      timeAfterNextLetterCanBeDeleted{0.08f}
{
    inputManager->registerObserver(this);

    if (not textFieldConfig)
    {
        throw exceptions::UIComponentConfigNotFound{"TextField config not found"};
    }

    name = textFieldConfig->uniqueName->getName();
    coreComponentsOwner = std::make_unique<components::core::ComponentOwner>(textFieldConfig->position);
    coreComponentsOwner->addComponent<components::core::GraphicsComponent>(
        rendererPool, textFieldConfig->size, textFieldConfig->position, textFieldConfig->color,
        graphics::VisibilityLayer::First);
    coreComponentsOwner->addComponent<components::core::TextComponent>(
        rendererPool, textFieldConfig->size, "", textFieldConfig->fontPath, textFieldConfig->fontSize,
        graphics::Color::White, textFieldConfig->textOffset);
    coreComponentsOwner->addComponent<components::core::HitBoxComponent>(textFieldConfig->size);

    auto textFieldClickedAction = [&] {
        textFieldClicked = true;
        textFieldConfig->clickInFieldAction();
    };

    clickOutsideFieldAction = textFieldConfig->clickOutsideFieldAction;

    coreComponentsOwner->addComponent<components::core::ClickableComponent>(
        inputManager, std::move(textFieldClickedAction));

    coreComponentsOwner->loadDependentComponents();
    deleteCharactersTimer.start();
}

TextField::~TextField()
{
    inputManager->removeObserver(this);
}

void TextField::update(utils::DeltaTime deltaTime)
{
    if (inputStatus->isKeyPressed(input::InputKey::MouseLeft))
    {
        if (auto textFieldHitBox = coreComponentsOwner->getComponent<components::core::HitBoxComponent>())
        {
            if (not textFieldHitBox->intersects(inputStatus->getMousePosition()))
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
            if (inputStatus->isKeyReleased(alphanumericButtonKey))
            {
                if (inputBuffer.size() < inputBufferMaximumSize)
                {
                    inputBuffer += utils::getLowerCases(toString(alphanumericButtonKey));
                    setText(inputBuffer);
                }
            }
        }

        if (inputStatus->isKeyPressed(input::InputKey::Backspace))
        {
            if (deleteCharactersTimer.getElapsedSeconds() > timeAfterNextLetterCanBeDeleted)
            {
                if (not inputBuffer.empty())
                {
                    utils::cutOffString(inputBuffer, inputBuffer.size() - 1, inputBuffer.size() - 1);
                    setText(inputBuffer);
                }

                deleteCharactersTimer.restart();
            }
        }
    }

    coreComponentsOwner->update(deltaTime);
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

void TextField::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
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