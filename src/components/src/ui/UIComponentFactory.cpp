#include "UIComponentFactory.h"

#include <utility>

#include "TimerFactory.h"

namespace components::ui
{

UIComponentFactory::UIComponentFactory(const std::shared_ptr<core::SharedContext>& sharedContext)
    : sharedContext{sharedContext}
{
}

std::unique_ptr<Background>
UIComponentFactory::createBackground(std::unique_ptr<BackgroundConfig> backgroundConfig) const
{
    return std::make_unique<Background>(sharedContext, std::move(backgroundConfig));
}

std::unique_ptr<Button> UIComponentFactory::createButton(std::unique_ptr<ButtonConfig> buttonConfig) const
{
    return std::make_unique<Button>(sharedContext, std::move(buttonConfig),
                                    utils::TimerFactory::createTimer());
}

std::unique_ptr<CheckBox>
UIComponentFactory::createCheckBox(std::unique_ptr<CheckBoxConfig> checkBoxConfig) const
{
    return std::make_unique<CheckBox>(sharedContext, std::move(checkBoxConfig),
                                      utils::TimerFactory::createTimer());
}

std::unique_ptr<Label> UIComponentFactory::createLabel(std::unique_ptr<LabelConfig> labelConfig) const
{
    return std::make_unique<Label>(sharedContext, std::move(labelConfig));
}

std::unique_ptr<TextField>
UIComponentFactory::createTextField(std::unique_ptr<TextFieldConfig> textFieldConfig) const
{
    return std::make_unique<TextField>(sharedContext, std::move(textFieldConfig),
                                       utils::TimerFactory::createTimer());
}

std::unique_ptr<Image> UIComponentFactory::createImage(std::unique_ptr<ImageConfig> imageConfig) const
{
    return std::make_unique<Image>(sharedContext, std::move(imageConfig));
}

}