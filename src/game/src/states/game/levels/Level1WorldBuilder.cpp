#include "Level1WorldBuilder.h"

#include <utility>

#include "exceptions/InvalidTileMap.h"

namespace game
{

Level1WorldBuilder::Level1WorldBuilder(std::shared_ptr<CharacterFactory> characterFactoryInit,
                                       std::shared_ptr<ObstacleFactory> obstacleFactoryInit,
                                       std::shared_ptr<ItemFactory> itemFactoryInit,
                                       std::shared_ptr<components::core::SharedContext> sharedContextInit,
                                       Level1Controller* level1ControllerInit)
    : characterFactory{std::move(characterFactoryInit)},
      obstacleFactory{std::move(obstacleFactoryInit)},
      itemFactory{std::move(itemFactoryInit)},
      sharedContext{std::move(sharedContextInit)},
      level1Controller{level1ControllerInit}
{
}

std::vector<std::shared_ptr<components::core::ComponentOwner>>
Level1WorldBuilder::buildWorldObjects(const std::shared_ptr<TileMap>& tileMap)
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
                player = characterFactory->createPlayer(position,
                                                        [this]() { level1Controller->deadPlayerAction(); });
                worldObjects.push_back(player);
            }
        }
    }

    if (not player)
    {
        throw exceptions::InvalidTileMap{"player does not exist in map"};
    }

    std::vector<utils::Vector2f> campfirePositions;
    std::vector<utils::Vector2f> bushPositions;

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
            case TileType::Soil:
            {
                auto grass = obstacleFactory->createSoil(position);
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
                bushPositions.push_back(position);
                break;
            }
            case TileType::Campfire:
            {
                campfirePositions.push_back(position);
                break;
            }
            case TileType::Chest:
            {
                auto key = itemFactory->createKey({position.x + 1, position.x + 1});
                auto keyCollectableItem = key->getComponent<components::core::CollectableItemComponent>();
                auto chest = obstacleFactory->createChestWithItem(position, player, keyCollectableItem);
                worldObjects.push_back(key);
                worldObjects.push_back(chest);
                break;
            }
            case TileType::Npc:
            {
                npc = characterFactory->createDruidNpc(player, position,
                                                       [this]() { level1Controller->druidFirstAction(); });
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
                //                auto bandit = characterFactory->createBanditEnemy(player, position);
                //                worldObjects.push_back(bandit);
                break;
            }
            case TileType::Player:
            {
                break;
            }
            }
        }
    }

    if (campfirePositions.size() != 2)
    {
        throw exceptions::InvalidTileMap{"number of campfires is not equal 2"};
    }

    std::sort(campfirePositions.begin(), campfirePositions.end(),
              [](const utils::Vector2f& lhs, const utils::Vector2f& rhs) { return lhs.x < rhs.x; });

    auto firstCampfireInLevel = obstacleFactory->createCampfire(
        campfirePositions[0], player, [this]() { level1Controller->firstCampfireAction(); });
    worldObjects.push_back(firstCampfireInLevel);

    auto lastCampfireInLevel = obstacleFactory->createCampfire(
        campfirePositions[1], player, [this]() { level1Controller->lastCampfireAction(); });
    worldObjects.push_back(lastCampfireInLevel);

    if (bushPositions.size() != 3)
    {
        throw exceptions::InvalidTileMap{"number of bushes is not equal 3"};
    }

    std::sort(bushPositions.begin(), bushPositions.end(),
              [](const utils::Vector2f& lhs, const utils::Vector2f& rhs) { return lhs.x < rhs.x; });

    auto blueberries = itemFactory->createBlueberries({bushPositions[0].x + 1, bushPositions[0].y + 1});
    auto blueberriesCollectableItem = blueberries->getComponent<components::core::CollectableItemComponent>();
    auto blueberriesBush =
        obstacleFactory->createBushWithItem(bushPositions[0], player, blueberriesCollectableItem);
    worldObjects.push_back(blueberries);
    worldObjects.push_back(blueberriesBush);

    auto apple = itemFactory->createApple({bushPositions[1].x + 1, bushPositions[1].y + 1});
    auto appleCollectableItem = apple->getComponent<components::core::CollectableItemComponent>();
    auto appleBush = obstacleFactory->createBushWithItem(bushPositions[1], player, appleCollectableItem);
    worldObjects.push_back(apple);
    worldObjects.push_back(appleBush);

    auto yerba = itemFactory->createYerba({bushPositions[2].x + 1, bushPositions[2].y + 1});
    auto yerbaCollectableItem = yerba->getComponent<components::core::CollectableItemComponent>();
    auto yerbaBush = obstacleFactory->createBushWithItem(bushPositions[2], player, yerbaCollectableItem);
    worldObjects.push_back(yerba);
    worldObjects.push_back(yerbaBush);

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

std::shared_ptr<components::core::ComponentOwner> Level1WorldBuilder::getPlayer() const
{
    return player;
}

std::shared_ptr<components::core::ComponentOwner> Level1WorldBuilder::getRabbit() const
{
    return rabbit;
}

std::shared_ptr<components::core::ComponentOwner> Level1WorldBuilder::getNpc() const
{
    return npc;
}
}