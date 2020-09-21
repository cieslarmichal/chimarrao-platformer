#include "DefaultGuiFactory.h"

#include "WindowSfml.h"

namespace gui
{

std::unique_ptr<Window> DefaultGuiFactory::createWindow(const utils::Vector2u& windowSize,
                                                        const std::string& title) const
{
    return std::make_unique<WindowSfml>(windowSize, title);
}
}