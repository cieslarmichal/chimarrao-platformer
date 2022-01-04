#pragma once

#include "CharacterFactory.h"
#include "ObstacleFactory.h"

namespace game
{
class CustomWorldBuilder
{
public:
    CustomWorldBuilder(std::shared_ptr<CharacterFactory>, std::shared_ptr<ObstacleFactory>,
                       std::shared_ptr<components::core::SharedContext>);

    std::vector<std::shared_ptr<components::core::ComponentOwner>>
    buildWorldObjects(const std::shared_ptr<TileMap>&);
    std::shared_ptr<components::core::ComponentOwner> getPlayer() const;
    std::shared_ptr<components::core::ComponentOwner> getRabbit() const;
    std::shared_ptr<components::core::ComponentOwner> getNpc() const;

private:
    std::shared_ptr<CharacterFactory> characterFactory;
    std::shared_ptr<ObstacleFactory> obstacleFactory;
    std::shared_ptr<components::core::SharedContext> sharedContext;
    std::shared_ptr<components::core::ComponentOwner> player;
    std::shared_ptr<components::core::ComponentOwner> rabbit;
    std::shared_ptr<components::core::ComponentOwner> npc;
};
}
