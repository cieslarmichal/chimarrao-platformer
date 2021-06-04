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
static auto& tryToGetComponentByName(const std::vector<T>& uiComponents, const std::string& nameToFind)
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
    activateComponents(images);
}

void DefaultUIManager::deactivate()
{
    background->deactivate();
    deactivateComponents(buttons);
    deactivateComponents(checkBoxes);
    deactivateComponents(labels);
    deactivateComponents(textFields);
    deactivateComponents(images);
}

void DefaultUIManager::setColor(const std::string& componentName, graphics::Color color)
{
    switch (getComponentType(componentName))
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
    case UIComponentType::Image:
    {
        break;
    }
    }
}

void DefaultUIManager::changeClickAction(const std::string& componentName,
                                         const std::vector<core::KeyAction>& keyActions)
{
    if (getComponentType(componentName) == UIComponentType::Button)
    {
        auto& button = tryToGetComponentByName(buttons, componentName);
        button->setClickAction(keyActions);
    }
}

void DefaultUIManager::invokeClickAction(const std::string& componentName,
                                         input::InputKey keyAssignedToClickAction)
{
    if (getComponentType(componentName) == UIComponentType::Button)
    {
        auto& button = tryToGetComponentByName(buttons, componentName);
        button->invokeClickAction(keyAssignedToClickAction);
    }
    else if (getComponentType(componentName) == UIComponentType::CheckBox)
    {
        auto& checkBox = tryToGetComponentByName(checkBoxes, componentName);
        checkBox->toggle();
    }
}

void DefaultUIManager::setText(const std::string& componentName, const std::string& text)
{
    switch (getComponentType(componentName))
    {
    case UIComponentType::Button:
    {
        auto& button = tryToGetComponentByName(buttons, componentName);
        button->setText(text);
        break;
    }
    case UIComponentType::Label:
    {
        auto& label = tryToGetComponentByName(labels, componentName);
        label->setText(text);
        break;
    }
    case UIComponentType::TextField:
    {
        auto& textField = tryToGetComponentByName(textFields, componentName);
        textField->setText(text);
        break;
    }
    default:
        break;
    }
}

void DefaultUIManager::setChecked(const std::string& componentName, bool checked)
{
    if (getComponentType(componentName) == UIComponentType::CheckBox)
    {
        auto& checkBox = tryToGetComponentByName(checkBoxes, componentName);
        checkBox->setChecked(checked);
    }
}

void DefaultUIManager::activateComponent(const std::string& componentName)
{
    switch (getComponentType(componentName))
    {
    case UIComponentType::Background:
    {
        background->activate();
        break;
    }
    case UIComponentType::Button:
    {
        auto& button = tryToGetComponentByName(buttons, componentName);
        button->activate();
        break;
    }
    case UIComponentType::CheckBox:
    {
        auto& checkBox = tryToGetComponentByName(checkBoxes, componentName);
        checkBox->activate();
        break;
    }
    case UIComponentType::Label:
    {
        auto& label = tryToGetComponentByName(labels, componentName);
        label->activate();
        break;
    }
    case UIComponentType::TextField:
    {
        auto& textField = tryToGetComponentByName(textFields, componentName);
        textField->activate();
        break;
    }
    case UIComponentType::Image:
    {
        const auto& image = tryToGetComponentByName(images, componentName);
        image->activate();
        break;
    }
    }
}

void DefaultUIManager::deactivateComponent(const std::string& componentName)
{
    switch (getComponentType(componentName))
    {
    case UIComponentType::Background:
    {
        background->deactivate();
        break;
    }
    case UIComponentType::Button:
    {
        auto& button = tryToGetComponentByName(buttons, componentName);
        button->deactivate();
        break;
    }
    case UIComponentType::CheckBox:
    {
        const auto& checkBox = tryToGetComponentByName(checkBoxes, componentName);
        checkBox->deactivate();
        break;
    }
    case UIComponentType::Label:
    {
        auto& label = tryToGetComponentByName(labels, componentName);
        label->deactivate();
        break;
    }
    case UIComponentType::TextField:
    {
        auto& textField = tryToGetComponentByName(textFields, componentName);
        textField->deactivate();
        break;
    }
    case UIComponentType::Image:
    {
        const auto& image = tryToGetComponentByName(images, componentName);
        image->deactivate();
        break;
    }
    }
}

bool DefaultUIManager::isComponentActive(const std::string& componentName) const
{
    switch (getComponentType(componentName))
    {
    case UIComponentType::Background:
    {
        return background->isActive();
    }
    case UIComponentType::Button:
    {
        const auto& button = tryToGetComponentByName(buttons, componentName);
        return button->isActive();
    }
    case UIComponentType::CheckBox:
    {
        const auto& checkBox = tryToGetComponentByName(checkBoxes, componentName);
        return checkBox->isActive();
    }
    case UIComponentType::Label:
    {
        const auto& label = tryToGetComponentByName(labels, componentName);
        return label->isActive();
    }
    case UIComponentType::TextField:
    {
        const auto& textField = tryToGetComponentByName(textFields, componentName);
        return textField->isActive();
    }
    case UIComponentType::Image:
    {
        const auto& image = tryToGetComponentByName(images, componentName);
        return image->isActive();
    }
    default:
        return false;
    }
}

void DefaultUIManager::createUIComponents(std::unique_ptr<UIConfig> uiConfig)
{
    background = uiComponentFactory->createBackground(std::move(uiConfig->backgroundConfig));
    componentsRegistry.insert({background->getName(), UIComponentType::Background});

    for (auto& buttonConfig : uiConfig->buttonsConfig)
    {
        auto button = uiComponentFactory->createButton(std::move(buttonConfig));
        componentsRegistry.insert({button->getName(), UIComponentType::Button});
        buttons.emplace_back(std::move(button));
    }

    for (auto& checkBoxConfig : uiConfig->checkBoxesConfig)
    {
        auto checkBox = uiComponentFactory->createCheckBox(std::move(checkBoxConfig));
        componentsRegistry.insert({checkBox->getName(), UIComponentType::CheckBox});
        checkBoxes.emplace_back(std::move(checkBox));
    }

    for (auto& labelConfig : uiConfig->labelsConfig)
    {
        auto label = uiComponentFactory->createLabel(std::move(labelConfig));
        componentsRegistry.insert({label->getName(), UIComponentType::Label});
        labels.emplace_back(std::move(label));
    }

    for (auto& textFieldConfig : uiConfig->textFieldsConfig)
    {
        auto textField = uiComponentFactory->createTextField(std::move(textFieldConfig));
        componentsRegistry.insert({textField->getName(), UIComponentType::TextField});
        textFields.emplace_back(std::move(textField));
    }

    for (auto& imageConfig : uiConfig->imagesConfig)
    {
        auto image = uiComponentFactory->createImage(std::move(imageConfig));
        componentsRegistry.insert({image->getName(), UIComponentType::Image});
        images.emplace_back(std::move(image));
    }
}

UIComponentType DefaultUIManager::getComponentType(const std::string& componentName) const
{
    return componentsRegistry.at(componentName);
}

void DefaultUIManager::freezeAllButtons()
{
    for (auto& button : buttons)
    {
        button->restartClickActionFreezeTimer();
    }
}

}