#include "CustomWorldBuilder.h"

#include <utility>

namespace game
{

CustomWorldBuilder::CustomWorldBuilder(std::shared_ptr<CharacterFactory> characterFactoryInit,
                                       std::shared_ptr<ObstacleFactory> obstacleFactoryInit,
                                       std::shared_ptr<components::core::SharedContext> sharedContextInit)
    : characterFactory{std::move(characterFactoryInit)},
      obstacleFactory{std::move(obstacleFactoryInit)},
      sharedContext{std::move(sharedContextInit)}
{
}

std::vector<std::shared_ptr<components::core::ComponentOwner>>
CustomWorldBuilder::buildWorldObjects(const std::shared_ptr<TileMap>& tileMap)
{
    std::vector<std::shared_ptr<components::core::ComponentOwner>> worldObjects;

    for (int x = 0; x < tileMap->getSize().x; x++)
    {
        for (int y = 0; y < tileMap->getSize().y; y++)
        {
            const auto tileType = tileMap->getTile(utils::Vector2i{x, y})->type;
            if (tileType and *tileType == TileType::Player)
            {
                const auto position =
                    utils::Vector2f{static_cast<float>(x) * 4.f, static_cast<float>(y) * 4.f};
                player = characterFactory->createPlayer(position);
                worldObjects.push_back(player);
            }
        }
    }

    for (int x = 0; x < tileMap->getSize().x; x++)
    {
        for (int y = 0; y < tileMap->getSize().y; y++)
        {
            const auto tileType = tileMap->getTile(utils::Vector2i{x, y})->type;

            if (not tileType)
            {
                continue;
            }

            const auto position = utils::Vector2f{static_cast<float>(x) * 4.f, static_cast<float>(y) * 4.f};

            switch (*tileType)
            {
            case TileType::Brick:
            {
                auto brick = obstacleFactory->createBrick(position);
                worldObjects.push_back(brick);
                break;
            }

            case TileType::Grass:
            {
                auto grass = obstacleFactory->createGrass(position);
                worldObjects.push_back(grass);
                break;
            }

            case TileType::Tree:
            {
                auto tree = obstacleFactory->createTree(position);
                worldObjects.push_back(tree);
                break;
            }
            case TileType::Bush:
            {
                auto bush = obstacleFactory->createBush(position, player);
                worldObjects.push_back(bush);
                break;
            }
            case TileType::Campfire:
            {
                auto campfire = obstacleFactory->createCampfire(position, player);
                worldObjects.push_back(campfire);
                break;
            }
            case TileType::Chest:
            {
                auto chest = obstacleFactory->createChest(position);
                worldObjects.push_back(chest);
                break;
            }
            case TileType::Npc:
            {
                npc = characterFactory->createDruidNpc(player, position);
                worldObjects.push_back(npc);
                break;
            }
            case TileType::Rabbit:
            {
                rabbit = characterFactory->createRabbitFollower(player, position);
                worldObjects.push_back(rabbit);
                break;
            }
            case TileType::Bandit:
            {
                auto bandit = characterFactory->createBanditEnemy(player, position);
                worldObjects.push_back(bandit);
                break;
            }
            case TileType::Player:
            {
                break;
            }
            }
        }
    }

    auto leftMapBorder = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{-1, 0},
                                                                            "left border", sharedContext);
    leftMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1, static_cast<float>(tileMap->getSize().y) * 4.f},
        components::core::CollisionLayer::Default);
    worldObjects.push_back(leftMapBorder);

    auto topMapBorder = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{0, -1},
                                                                           "top border", sharedContext);
    topMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{static_cast<float>(tileMap->getSize().x) * 4.f, 1},
        components::core::CollisionLayer::Default);
    worldObjects.push_back(topMapBorder);

    auto rightMapBorder = std::make_shared<components::core::ComponentOwner>(
        utils::Vector2f{static_cast<float>(tileMap->getSize().x) * 4.f, 0}, "right border", sharedContext);
    rightMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1, static_cast<float>(tileMap->getSize().y) * 4.f},
        components::core::CollisionLayer::Default);
    worldObjects.push_back(rightMapBorder);

    auto bottomMapBorder = std::make_shared<components::core::ComponentOwner>(
        utils::Vector2f{0, static_cast<float>(tileMap->getSize().y) * 4.f}, "bottom border", sharedContext);
    bottomMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{static_cast<float>(tileMap->getSize().x) * 4.f, 1},
        components::core::CollisionLayer::Default);
    worldObjects.push_back(bottomMapBorder);

    return worldObjects;
}

std::shared_ptr<components::core::ComponentOwner> CustomWorldBuilder::getPlayer() const
{
    return player;
}

std::shared_ptr<components::core::ComponentOwner> CustomWorldBuilder::getRabbit() const
{
    return rabbit;
}

std::shared_ptr<components::core::ComponentOwner> CustomWorldBuilder::getNpc() const
{
    return npc;
}
}