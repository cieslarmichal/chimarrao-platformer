#pragma once

#include <memory>

#include "Background.h"
#include "Button.h"
#include "CheckBox.h"
#include "Image.h"
#include "InputManager.h"
#include "Label.h"
#include "RendererPool.h"
#include "TextField.h"
#include "UIComponent.h"

namespace components::ui
{
class UIComponentFactory
{
public:
    UIComponentFactory(std::shared_ptr<graphics::RendererPool>);

    std::unique_ptr<Background> createBackground(std::unique_ptr<BackgroundConfig>) const;
    std::unique_ptr<Button> createButton(std::unique_ptr<ButtonConfig>) const;
    std::unique_ptr<CheckBox> createCheckBox(std::unique_ptr<CheckBoxConfig>) const;
    std::unique_ptr<Label> createLabel(std::unique_ptr<LabelConfig>) const;
    std::unique_ptr<TextField> createTextField(std::unique_ptr<TextFieldConfig>) const;
    std::unique_ptr<Image> createImage(std::unique_ptr<ImageConfig>) const;

private:
    std::shared_ptr<graphics::RendererPool> rendererPool;
};
}