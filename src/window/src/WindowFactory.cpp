#include "WindowFactory.h"

#include "DefaultWindowFactory.h"

namespace window
{

std::unique_ptr<WindowFactory> WindowFactory::createGuiFactory()
{
    return std::make_unique<DefaultWindowFactory>();
}
}