#pragma once

#include "gmock/gmock.h"

#include "MapsReader.h"

namespace game
{
class MapsReaderMock : public MapsReader
{
public:
    MOCK_METHOD(std::vector<std::string>, readMapFilePaths, (), (override));
};
}