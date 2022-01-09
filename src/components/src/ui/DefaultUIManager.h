#pragma once

#include <vector>

#include "Background.h"
#include "Button.h"
#include "CheckBox.h"
#include "Image.h"
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
    explicit DefaultUIManager(const std::shared_ptr<core::SharedContext>&);

    void createUI(std::unique_ptr<UIConfig>) override;
    void update(utils::DeltaTime, const input::Input&) override;
    void activate() override;
    void deactivate() override;
    void setColor(const std::string& componentName, graphics::Color) override;
    void setTexture(const std::string& componentName, const std::string& texturePath) override;
    void changeClickAction(const std::string& componentName, const std::vector<core::KeyAction>&) override;
    void invokeClickAction(const std::string& componentName,
                           input::InputKey keyAssignedToClickAction) override;
    void invokeMouseOutAction(const std::string& componentName) override;
    void setText(const std::string& componentName, const std::string& text) override;
    boost::optional<std::string> getText(const std::string& componentName) const override;
    void setChecked(const std::string& componentName, bool checked) override;
    void activateComponent(const std::string& componentName) override;
    void deactivateComponent(const std::string& componentName) override;
    bool isComponentActive(const std::string& componentName) const override;
    void freezeAllButtons() override;

private:
    void createUIComponents(std::unique_ptr<UIConfig>);
    UIComponentType getComponentType(const std::string& componentName) const;

    const std::shared_ptr<core::SharedContext>& sharedContext;
    std::unique_ptr<UIComponentFactory> uiComponentFactory;
    std::unique_ptr<Background> background;
    std::vector<std::unique_ptr<Button>> buttons;
    std::vector<std::unique_ptr<Label>> labels;
    std::vector<std::unique_ptr<TextField>> textFields;
    std::vector<std::unique_ptr<CheckBox>> checkBoxes;
    std::vector<std::unique_ptr<Image>> images;
    std::unordered_map<std::string, UIComponentType> componentsRegistry;
};
}