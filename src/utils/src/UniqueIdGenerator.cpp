#include "UniqueIdGenerator.h"

#include <boost/uuid/uuid_generators.hpp>

namespace utils
{
UniqueId UniqueIdGenerator::generateId()
{
    return boost::uuids::random_generator()();
}
}