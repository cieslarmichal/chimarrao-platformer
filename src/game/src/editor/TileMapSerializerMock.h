#pragma once

#include "gmock/gmock.h"

#include "TileMapSerializer.h"

namespace game
{
class TileMapSerializerMock : public TileMapSerializer
{
public:
    MOCK_METHOD(std::string, serialize, (const TileMapInfo&), (const override));
    MOCK_METHOD(TileMapInfo, deserialize, (const std::string&), (const override));
};
}