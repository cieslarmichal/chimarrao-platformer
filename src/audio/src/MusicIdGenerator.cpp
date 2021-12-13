#include "MusicIdGenerator.h"

#include <boost/uuid/uuid_generators.hpp>

namespace audio
{

MusicId MusicIdGenerator::generateId()
{
    return boost::uuids::random_generator()();
}
}