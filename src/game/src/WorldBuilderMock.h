#pragma once

#include "gmock/gmock.h"

#include "WorldBuilder.h"

namespace game
{
class WorldBuilderMock : public WorldBuilder
{
public:
    MOCK_METHOD(std::vector<std::shared_ptr<components::core::ComponentOwner>>, buildWorldObjects,
                (const std::shared_ptr<TileMap>&), (override));
    MOCK_METHOD(std::shared_ptr<components::core::ComponentOwner>, getPlayer, (), (const override));
};
}