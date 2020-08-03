#include "PhysicsIdGenerator.h"

#include <boost/uuid/uuid_generators.hpp>

namespace physics
{
PhysicsId PhysicsIdGenerator::generateId()
{
    return boost::uuids::random_generator()();
}
}
