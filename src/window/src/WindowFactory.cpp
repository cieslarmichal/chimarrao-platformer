#include "WindowFactory.h"

#include "DefaultWindowFactory.h"

namespace window
{

std::unique_ptr<WindowFactory> WindowFactory::createWindowFactory()
{
    return std::make_unique<DefaultWindowFactory>();
}
}