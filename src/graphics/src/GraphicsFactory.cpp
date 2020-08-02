#include "GraphicsFactory.h"
#include "DefaultGraphicsFactory.h"

namespace graphics
{

std::unique_ptr<GraphicsFactory> GraphicsFactory::createGraphicsFactory()
{
    return std::make_unique<DefaultGraphicsFactory>();
}
}