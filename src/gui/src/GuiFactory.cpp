#include "GuiFactory.h"

#include "DefaultGuiFactory.h"

namespace gui
{

std::unique_ptr<GuiFactory> GuiFactory::createGuiFactory()
{
    return std::make_unique<DefaultGuiFactory>();
}
}