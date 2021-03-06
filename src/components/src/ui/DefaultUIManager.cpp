#include "DefaultUIManager.h"

namespace components::ui
{

DefaultUIManager::DefaultUIManager(const std::shared_ptr<input::InputManager>& inputManager,
                                   const std::shared_ptr<graphics::RendererPool>& rendererPool,
                                   std::unique_ptr<UIConfig> uiConfig)
{
}

void DefaultUIManager::setColor(UIComponentType, const std::string& componentName, graphics::Color) {}

void DefaultUIManager::setText(UIComponentTypeWithText, const std::string& componentName,
                               const std::string& text)
{
}

void DefaultUIManager::setChecked(UIComponentTypeWithCheck, const std::string& componentName, bool checked) {}

}