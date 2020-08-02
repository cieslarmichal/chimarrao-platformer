#include "GraphicsIdGenerator.h"

#include <boost/uuid/uuid_generators.hpp>

namespace graphics
{

GraphicsId GraphicsIdGenerator::generateId()
{
    return boost::uuids::random_generator()();
}
}