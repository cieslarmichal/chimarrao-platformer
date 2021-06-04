#include "CheckBox.h"

#include "core/BoxColliderComponent.h"
#include "core/ClickableComponent.h"
#include "core/GraphicsComponent.h"
#include "core/MouseOverComponent.h"
#include "core/TextComponent.h"
#include "exceptions/UIComponentConfigNotFound.h"

namespace components::ui
{

CheckBox::CheckBox(const std::shared_ptr<graphics::RendererPool>& rendererPool,
                   std::unique_ptr<CheckBoxConfig> checkBoxConfig, std::unique_ptr<utils::Timer> timer)
    : timeAfterCheckBoxCanBeClicked{0.25f}, freezeClickableCheckBoxTimer{std::move(timer)}
{
    if (not checkBoxConfig)
    {
        throw exceptions::UIComponentConfigNotFound{"CheckBox config not found"};
    }

    name = checkBoxConfig->uniqueName;
    coreComponentsOwner = std::make_unique<components::core::ComponentOwner>(checkBoxConfig->position, name);
    coreComponentsOwner->addComponent<components::core::GraphicsComponent>(
        rendererPool, checkBoxConfig->size, checkBoxConfig->position, checkBoxConfig->color,
        graphics::VisibilityLayer::First);
    const auto text = checkBoxConfig->checked ? "X" : "";
    coreComponentsOwner->addComponent<components::core::TextComponent>(
        rendererPool, checkBoxConfig->position, text, checkBoxConfig->fontPath, checkBoxConfig->fontSize,
        graphics::Color::Black, checkBoxConfig->textOffset);
    coreComponentsOwner->addComponent<components::core::BoxColliderComponent>(checkBoxConfig->size);
    coreComponentsOwner->addComponent<components::core::ClickableComponent>(checkBoxConfig->clickAction);

    if (checkBoxConfig->mouseOverActions)
    {
        coreComponentsOwner->addComponent<components::core::MouseOverComponent>(
            checkBoxConfig->mouseOverActions->mouseOverAction,
            checkBoxConfig->mouseOverActions->mouseOutAction);
    }

    coreComponentsOwner->loadDependentComponents();
    coreComponentsOwner->getComponent<components::core::ClickableComponent>()->disable();
}

void CheckBox::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    if (checkBoxClickActionFrozen &&
        freezeClickableCheckBoxTimer->getElapsedSeconds() > timeAfterCheckBoxCanBeClicked)
    {
        checkBoxClickActionFrozen = false;
        coreComponentsOwner->getComponent<components::core::ClickableComponent>()->enable();
    }

    coreComponentsOwner->update(deltaTime, input);
    coreComponentsOwner->lateUpdate(deltaTime, input);
}

std::string CheckBox::getName() const
{
    return name;
}

void CheckBox::activate()
{
    coreComponentsOwner->enable();
    coreComponentsOwner->getComponent<components::core::ClickableComponent>()->disable();
    freezeClickableCheckBoxTimer->restart();
    checkBoxClickActionFrozen = true;
}

void CheckBox::deactivate()
{
    coreComponentsOwner->disable();
    checkBoxClickActionFrozen = true;
}

void CheckBox::setColor(graphics::Color color)
{
    coreComponentsOwner->getComponent<components::core::GraphicsComponent>()->setColor(color);
}

void CheckBox::setChecked(bool checked)
{
    const auto text = checked ? "X" : "";
    coreComponentsOwner->getComponent<components::core::TextComponent>()->setText(text);
}

bool CheckBox::isActive() const
{
    return coreComponentsOwner->areComponentEnabled();
}
}