#pragma once

#include "gmock/gmock.h"

#include "TileMap.h"

namespace game
{
class TileMapMock : public TileMap
{
public:
    MOCK_METHOD(void, saveToFile, (), (override));
    MOCK_METHOD(void, loadFromFile, (const std::string&), (override));
    MOCK_METHOD(void, setTileMapInfo, (const TileMapInfo&), (override));
    MOCK_METHOD(void, setName, (const std::string&), (override));
    MOCK_METHOD(const std::string&, getName, (), (const override));
    MOCK_METHOD(void, setTile, (utils::Vector2i position, TileType value), (override));
    MOCK_METHOD(std::shared_ptr<TileInfo>&, getTile, (utils::Vector2i position), (override));
    MOCK_METHOD(utils::Vector2i, getSize, (), (const override));
    MOCK_METHOD(void, extend, (), (override));
    MOCK_METHOD(bool, isCustomMap, (), (const override));
};
}