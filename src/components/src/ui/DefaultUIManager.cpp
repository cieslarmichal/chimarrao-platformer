#include "DefaultUIManager.h"

#include "exceptions/InvalidUIConfig.h"
#include "exceptions/UIComponentNotFound.h"
#include "exceptions/UIConfigNotFound.h"

namespace components::ui
{

namespace
{
template <typename T>
static auto& tryToGetComponentByName(std::vector<T>& uiComponents, const std::string& nameToFind)
{
    auto foundComponent =
        std::find_if(uiComponents.begin(), uiComponents.end(),
                     [&nameToFind](const T& uiComponent) { return uiComponent->getName() == nameToFind; });
    if (foundComponent != uiComponents.end())
    {
        return *foundComponent;
    }

    throw exceptions::UIComponentNotFound{"Component with name: " + nameToFind + " not found"};
}

template <typename T>
void updateComponents(std::vector<T>& uiComponents, utils::DeltaTime deltaTime, const input::Input& input)
{
    for (auto& uiComponent : uiComponents)
    {
        uiComponent->update(deltaTime, input);
    }
}

template <typename T>
void activateComponents(std::vector<T>& uiComponents)
{
    for (auto& uiComponent : uiComponents)
    {
        uiComponent->activate();
    }
}

template <typename T>
void deactivateComponents(std::vector<T>& uiComponents)
{
    for (auto& uiComponent : uiComponents)
    {
        uiComponent->deactivate();
    }
}
}

DefaultUIManager::DefaultUIManager(const std::shared_ptr<graphics::RendererPool>& rendererPool)
    : uiComponentFactory{std::make_unique<UIComponentFactory>(rendererPool)}
{
}

void DefaultUIManager::createUI(std::unique_ptr<UIConfig> uiConfig)
{
    if (not uiConfig)
    {
        throw exceptions::UIConfigNotFound{"Config not found"};
    }

    if (not uiConfig->backgroundConfig)
    {
        throw exceptions::InvalidUIConfig{"Config is not valid"};
    }

    createUIComponents(std::move(uiConfig));
}

void DefaultUIManager::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    background->update(deltaTime, input);
    updateComponents(buttons, deltaTime, input);
    updateComponents(checkBoxes, deltaTime, input);
    updateComponents(labels, deltaTime, input);
    updateComponents(textFields, deltaTime, input);
}

void DefaultUIManager::activate()
{
    background->activate();
    activateComponents(buttons);
    activateComponents(checkBoxes);
    activateComponents(labels);
    activateComponents(textFields);
}

void DefaultUIManager::deactivate()
{
    background->deactivate();
    deactivateComponents(buttons);
    deactivateComponents(checkBoxes);
    deactivateComponents(labels);
    deactivateComponents(textFields);
}

void DefaultUIManager::setColor(UIComponentType componentType, const std::string& componentName,
                                graphics::Color color)
{
    switch (componentType)
    {
    case UIComponentType::Background:
    {
        background->setColor(color);
        break;
    }
    case UIComponentType::Button:
    {
        auto& button = tryToGetComponentByName(buttons, componentName);
        button->setColor(color);
        break;
    }
    case UIComponentType::CheckBox:
    {
        auto& checkBox = tryToGetComponentByName(checkBoxes, componentName);
        checkBox->setColor(color);
        break;
    }
    case UIComponentType::Label:
    {
        auto& label = tryToGetComponentByName(labels, componentName);
        label->setColor(color);
        break;
    }
    case UIComponentType::TextField:
    {
        auto& textField = tryToGetComponentByName(textFields, componentName);
        textField->setColor(color);
        break;
    }
    }
}

void DefaultUIManager::changeClickAction(UIComponentType componentType, const std::string& componentName,
                                         const std::vector<core::KeyAction>& keyActions)
{
    if (componentType == UIComponentType::Button)
    {
        auto& button = tryToGetComponentByName(buttons, componentName);
        button->setClickAction(keyActions);
    }
}

void DefaultUIManager::setText(UIComponentTypeWithLabel componentType, const std::string& componentName,
                               const std::string& text)
{
    switch (componentType)
    {
    case UIComponentTypeWithLabel::Button:
    {
        auto& button = tryToGetComponentByName(buttons, componentName);
        button->setText(text);
        break;
    }
    case UIComponentTypeWithLabel::Label:
    {
        auto& label = tryToGetComponentByName(labels, componentName);
        label->setText(text);
        break;
    }
    }
}

void DefaultUIManager::setChecked(UIComponentTypeWithCheck componentType, const std::string& componentName,
                                  bool checked)
{
    switch (componentType)
    {
    case UIComponentTypeWithCheck::CheckBox:
    {
        auto& checkBox = tryToGetComponentByName(checkBoxes, componentName);
        checkBox->setChecked(checked);
        break;
    }
    }
}

void DefaultUIManager::createUIComponents(std::unique_ptr<UIConfig> uiConfig)
{
    background = uiComponentFactory->createBackground(std::move(uiConfig->backgroundConfig));

    for (auto& buttonConfig : uiConfig->buttonsConfig)
    {
        auto button = uiComponentFactory->createButton(std::move(buttonConfig));
        buttons.emplace_back(std::move(button));
    }

    for (auto& checkBoxConfig : uiConfig->checkBoxesConfig)
    {
        auto checkBox = uiComponentFactory->createCheckBox(std::move(checkBoxConfig));
        checkBoxes.emplace_back(std::move(checkBox));
    }

    for (auto& labelConfig : uiConfig->labelsConfig)
    {
        auto label = uiComponentFactory->createLabel(std::move(labelConfig));
        labels.emplace_back(std::move(label));
    }

    for (auto& textFieldConfig : uiConfig->textFieldsConfig)
    {
        auto textField = uiComponentFactory->createTextField(std::move(textFieldConfig));
        textFields.emplace_back(std::move(textField));
    }
}

}