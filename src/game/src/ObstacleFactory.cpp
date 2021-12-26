#include "ObstacleFactory.h"

#include "core/BoxColliderComponent.h"
#include "editor/TileType.h"
#include "core/AnimationComponent.h"

namespace game
{

ObstacleFactory::ObstacleFactory(const std::shared_ptr<components::core::SharedContext>& sharedContextInit)
    : sharedContext{sharedContextInit},
      animatorFactory{animations::AnimatorFactory::createAnimatorFactory(sharedContext->rendererPool)}
{
}

std::shared_ptr<components::core::ComponentOwner>
ObstacleFactory::createBrick(const utils::Vector2f& position)
{
    static int numberOfBricksInGame = 0;
    numberOfBricksInGame++;
    auto brick = std::make_shared<components::core::ComponentOwner>(
        position, "brick" + std::to_string(numberOfBricksInGame), sharedContext);
    brick->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4, 4}, position,
                                tileTypeToPathTexture(TileType::Brick), graphics::VisibilityLayer::Second);
    brick->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{4, 4},
                                                                components::core::CollisionLayer::Tile);
    return brick;
}

std::shared_ptr<components::core::ComponentOwner>
ObstacleFactory::createGrass(const utils::Vector2f& position)
{
    static int numberOfGrassesInGame = 0;
    numberOfGrassesInGame++;
    auto grass = std::make_shared<components::core::ComponentOwner>(
        position, "grass" + std::to_string(numberOfGrassesInGame), sharedContext);
    grass->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4, 4}, position,
                                tileTypeToPathTexture(TileType::Grass), graphics::VisibilityLayer::Second);
    grass->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{4, 4},
                                                                components::core::CollisionLayer::Tile);
    return grass;
}

std::shared_ptr<components::core::ComponentOwner> ObstacleFactory::createTree(const utils::Vector2f& position)
{
    static int numberOfTreesInGame = 0;
    numberOfTreesInGame++;
    auto tree = std::make_shared<components::core::ComponentOwner>(
        position, "tree" + std::to_string(numberOfTreesInGame), sharedContext);
    tree->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4, 4}, position,
                                tileTypeToPathTexture(TileType::Tree), graphics::VisibilityLayer::Second);
    tree->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{4, 4},
                                                                components::core::CollisionLayer::Tile);
    return tree;
}

std::shared_ptr<components::core::ComponentOwner> ObstacleFactory::createBush(const utils::Vector2f& position)
{
    static int numberOfBushesInGame = 0;
    numberOfBushesInGame++;
    auto bush = std::make_shared<components::core::ComponentOwner>(
        position, "bush" + std::to_string(numberOfBushesInGame), sharedContext);
    bush->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4, 4}, position,
                               tileTypeToPathTexture(TileType::Bush), graphics::VisibilityLayer::Second);
    bush->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{4, 4},
                                                               components::core::CollisionLayer::Tile);
    return bush;
}

std::shared_ptr<components::core::ComponentOwner>
ObstacleFactory::createCampfire(const utils::Vector2f& position)
{
    static int numberOfCampfiresInGame = 0;
    numberOfCampfiresInGame++;
    auto campfire = std::make_shared<components::core::ComponentOwner>(
        position, "campfire" + std::to_string(numberOfCampfiresInGame), sharedContext);
    auto campfireGraphicsComponent =
        campfire->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4.f, 4.f}, position,
                                  graphics::Color::White, graphics::VisibilityLayer::Second);
    const auto campfireGraphicsId = campfireGraphicsComponent->getGraphicsId();
    const std::shared_ptr<animations::Animator> campfireAnimation =
        animatorFactory->createCampfireAnimator(campfireGraphicsId);
    campfire->addComponent<components::core::AnimationComponent>(campfireAnimation);
    campfire->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{4, 4},
                                                                components::core::CollisionLayer::Tile);
    return campfire;
}

std::shared_ptr<components::core::ComponentOwner>
ObstacleFactory::createChest(const utils::Vector2f& position)
{
    static int numberOfChestsInGame = 0;
    numberOfChestsInGame++;
    auto chest = std::make_shared<components::core::ComponentOwner>(
        position, "chest" + std::to_string(numberOfChestsInGame), sharedContext);
    chest->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4, 4}, position,
                               tileTypeToPathTexture(TileType::Chest), graphics::VisibilityLayer::Second);
    chest->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{4, 4},
                                                               components::core::CollisionLayer::Tile);
    return chest;
}
}