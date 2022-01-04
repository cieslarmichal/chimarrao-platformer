#pragma once

#include "CharacterFactory.h"
#include "Level1Controller.h"
#include "ObstacleFactory.h"

namespace game
{
class Level1WorldBuilder
{
public:
    Level1WorldBuilder(std::shared_ptr<CharacterFactory>, std::shared_ptr<ObstacleFactory>,
                       std::shared_ptr<components::core::SharedContext>, Level1Controller*);

    std::vector<std::shared_ptr<components::core::ComponentOwner>>
    buildWorldObjects(const std::shared_ptr<TileMap>&);
    std::shared_ptr<components::core::ComponentOwner> getPlayer() const;
    std::shared_ptr<components::core::ComponentOwner> getRabbit() const;
    std::shared_ptr<components::core::ComponentOwner> getNpc() const;

private:
    std::shared_ptr<CharacterFactory> characterFactory;
    std::shared_ptr<ObstacleFactory> obstacleFactory;
    std::shared_ptr<components::core::SharedContext> sharedContext;
    Level1Controller* level1Controller;
    std::shared_ptr<components::core::ComponentOwner> player;
    std::shared_ptr<components::core::ComponentOwner> rabbit;
    std::shared_ptr<components::core::ComponentOwner> npc;
};
}
