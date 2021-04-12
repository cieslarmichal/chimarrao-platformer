#include "InputManagerFactory.h"

#include "DefaultInputManagerFactory.h"

namespace input
{

std::unique_ptr<InputManagerFactory> InputManagerFactory::createInputManagerFactory()
{
    return std::make_unique<DefaultInputManagerFactory>();
}
}