#include <boost/uuid/uuid_generators.hpp>

#include "UniqueIdGenerator.h"

namespace utils
{
UniqueId UniqueIdGenerator::generateId()
{
    return boost::uuids::random_generator()();
}
}