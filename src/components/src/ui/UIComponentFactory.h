#pragma once

#include <memory>

#include "InputManager.h"
#include "RendererPool.h"
#include "UIComponent.h"

namespace components::ui
{
struct BackgroundConfig;
struct ButtonConfig;
struct CheckBoxConfig;
struct LabelConfig;
struct TextFieldConfig;

class UIComponentFactory
{
public:
    UIComponentFactory(std::shared_ptr<input::InputManager>, std::shared_ptr<graphics::RendererPool>);

    std::unique_ptr<UIComponent> createBackground(std::unique_ptr<BackgroundConfig>) const;
    std::unique_ptr<UIComponent> createButton(std::unique_ptr<ButtonConfig>) const;
    std::unique_ptr<UIComponent> createCheckBox(std::unique_ptr<CheckBoxConfig>) const;
    std::unique_ptr<UIComponent> createLabel(std::unique_ptr<LabelConfig>) const;
    std::unique_ptr<UIComponent> createTextField(std::unique_ptr<TextFieldConfig>) const;

private:
    std::shared_ptr<input::InputManager> inputManager;
    std::shared_ptr<graphics::RendererPool> rendererPool;
};
}