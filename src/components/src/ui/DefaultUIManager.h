#pragma once

#include <vector>

#include "Background.h"
#include "Button.h"
#include "CheckBox.h"
#include "Label.h"
#include "TextField.h"
#include "UIComponentFactory.h"
#include "UIConfig.h"
#include "UIManager.h"

namespace components::ui
{
class DefaultUIManager : public UIManager
{
public:
    DefaultUIManager(const std::shared_ptr<graphics::RendererPool>&);

    void createUI(std::unique_ptr<UIConfig>) override;
    void update(utils::DeltaTime, const input::Input&) override;
    void activate() override;
    void deactivate() override;
    void setColor(UIComponentType, const std::string& componentName, graphics::Color) override;
    void changeClickAction(UIComponentType, const std::string& componentName,
                           const std::vector<core::KeyAction>&) override;
    void setText(UIComponentTypeWithLabel, const std::string& componentName,
                 const std::string& text) override;
    void setChecked(UIComponentTypeWithCheck, const std::string& componentName, bool checked) override;
    void activateComponent(UIComponentType, const std::string& componentName) override;
    void deactivateComponent(UIComponentType, const std::string& componentName) override;
    bool isComponentActive(UIComponentType, const std::string& componentName) const override;

private:
    void createUIComponents(std::unique_ptr<UIConfig>);

    std::unique_ptr<UIComponentFactory> uiComponentFactory;
    std::unique_ptr<Background> background;
    std::vector<std::unique_ptr<Button>> buttons;
    std::vector<std::unique_ptr<Label>> labels;
    std::vector<std::unique_ptr<TextField>> textFields;
    std::vector<std::unique_ptr<CheckBox>> checkBoxes;
};
}