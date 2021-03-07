#include "UIComponentFactory.h"

#include <utility>

namespace components::ui
{
UIComponentFactory::UIComponentFactory(std::shared_ptr<input::InputManager> inputManagerInit,
                                       std::shared_ptr<graphics::RendererPool> rendererPoolInit)
    : inputManager{std::move(inputManagerInit)}, rendererPool{std::move(rendererPoolInit)}
{
}

std::unique_ptr<Background>
UIComponentFactory::createBackground(std::unique_ptr<BackgroundConfig> backgroundConfig) const
{
    return std::make_unique<Background>(rendererPool, std::move(backgroundConfig));
}

std::unique_ptr<Button> UIComponentFactory::createButton(std::unique_ptr<ButtonConfig> buttonConfig) const
{
    return std::make_unique<Button>(inputManager, rendererPool, std::move(buttonConfig));
}

std::unique_ptr<CheckBox>
UIComponentFactory::createCheckBox(std::unique_ptr<CheckBoxConfig> checkBoxConfig) const
{
    return std::make_unique<CheckBox>(inputManager, rendererPool, std::move(checkBoxConfig));
}

std::unique_ptr<Label> UIComponentFactory::createLabel(std::unique_ptr<LabelConfig> labelConfig) const
{
    return std::make_unique<Label>(rendererPool, std::move(labelConfig));
}

std::unique_ptr<TextField>
UIComponentFactory::createTextField(std::unique_ptr<TextFieldConfig> textFieldConfig) const
{
    return std::make_unique<TextField>(inputManager, rendererPool, std::move(textFieldConfig));
}

}