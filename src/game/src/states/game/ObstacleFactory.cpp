#include "ObstacleFactory.h"

#include <utility>

#include "AnimationComponent.h"
#include "BoxColliderComponent.h"
#include "CommonUIConfigElements.h"
#include "LimitedSpaceActionComponent.h"
#include "TextComponent.h"
#include "TileType.h"

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

std::shared_ptr<components::core::ComponentOwner> ObstacleFactory::createSoil(const utils::Vector2f& position)
{
    static int numberOfSoilsInGame = 0;
    numberOfSoilsInGame++;
    auto soil = std::make_shared<components::core::ComponentOwner>(
        position, "soil" + std::to_string(numberOfSoilsInGame), sharedContext);
    soil->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4, 4}, position,
                               tileTypeToPathTexture(TileType::Soil), graphics::VisibilityLayer::Second);
    soil->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{4, 4},
                                                               components::core::CollisionLayer::Tile);
    return soil;
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

std::shared_ptr<components::core::ComponentOwner>
ObstacleFactory::createBush(const utils::Vector2f& position,
                            const std::shared_ptr<components::core::ComponentOwner>& player)
{
    static int numberOfBushesInGame = 0;
    numberOfBushesInGame++;
    auto bush = std::make_shared<components::core::ComponentOwner>(
        position, "bush" + std::to_string(numberOfBushesInGame), sharedContext);
    bush->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4, 4}, position,
                               tileTypeToPathTexture(TileType::Bush), graphics::VisibilityLayer::Second);
    bush->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{4, 4},
                                                               components::core::CollisionLayer::Tile);
    bush->addComponent<components::core::TextComponent>(sharedContext->rendererPool, position,
                                                        "Press E to search", fontPath, 9,
                                                        graphics::Color::Black, utils::Vector2f{-1.5, -2.f});
    bush->addComponent<components::core::LimitedSpaceActionComponent>(player.get(), []() {});
    return bush;
}

std::shared_ptr<components::core::ComponentOwner>
ObstacleFactory::createBushWithItem(const utils::Vector2f& position,
                                    const std::shared_ptr<components::core::ComponentOwner>& player,
                                    const std::shared_ptr<components::core::CollectableItemComponent>& item)
{
    static int numberOfBushesWithItemInGame = 0;
    numberOfBushesWithItemInGame++;
    auto bushWithItem = std::make_shared<components::core::ComponentOwner>(
        position, "bushWithItem" + std::to_string(numberOfBushesWithItemInGame), sharedContext);
    bushWithItem->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4, 4}, position,
                                       tileTypeToPathTexture(TileType::Bush),
                                       graphics::VisibilityLayer::Second);
    bushWithItem->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{4, 4}, components::core::CollisionLayer::Tile);
    bushWithItem->addComponent<components::core::TextComponent>(
        sharedContext->rendererPool, position, "Press E to search", fontPath, 9, graphics::Color::Black,
        utils::Vector2f{-1.5, -2.f});
    bushWithItem->addComponent<components::core::LimitedSpaceActionComponent>(
        player.get(),
        [=]() { player->getComponent<components::core::ItemCollectorComponent>()->collect(item); });
    return bushWithItem;
}

std::shared_ptr<components::core::ComponentOwner>
ObstacleFactory::createCampfire(const utils::Vector2f& position,
                                const std::shared_ptr<components::core::ComponentOwner>& player,
                                std::function<void(void)> action)
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
    campfire->addComponent<components::core::TextComponent>(
        sharedContext->rendererPool, position, "Press E to sleep", fontPath, 9, graphics::Color::Black,
        utils::Vector2f{-1.5, -2.f});
    campfire->addComponent<components::core::LimitedSpaceActionComponent>(player.get(), std::move(action));
    return campfire;
}

std::shared_ptr<components::core::ComponentOwner>
ObstacleFactory::createEmptyChest(const utils::Vector2f& position)
{
    static int numberOfEmptyChestsInGame = 0;
    numberOfEmptyChestsInGame++;
    auto emptyChest = std::make_shared<components::core::ComponentOwner>(
        position, "emptyChest" + std::to_string(numberOfEmptyChestsInGame), sharedContext);
    emptyChest->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4, 4}, position,
                                     tileTypeToPathTexture(TileType::Chest),
                                     graphics::VisibilityLayer::Second);
    emptyChest->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{4, 4},
                                                                     components::core::CollisionLayer::Tile);
    return emptyChest;
}

std::shared_ptr<components::core::ComponentOwner>
ObstacleFactory::createChestWithItem(const utils::Vector2f& position,
                                     const std::shared_ptr<components::core::ComponentOwner>& player,
                                     const std::shared_ptr<components::core::CollectableItemComponent>& item)
{
    static int numberOfChestsWithItemsInGame = 0;
    numberOfChestsWithItemsInGame++;
    auto chest = std::make_shared<components::core::ComponentOwner>(
        position, "chestWithItem" + std::to_string(numberOfChestsWithItemsInGame), sharedContext);
    chest->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{4, 4}, position,
                                tileTypeToPathTexture(TileType::Chest), graphics::VisibilityLayer::Second);
    chest->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{4, 4},
                                                                components::core::CollisionLayer::Tile);
    chest->addComponent<components::core::TextComponent>(sharedContext->rendererPool, position,
                                                         "Press E to search", fontPath, 9,
                                                         graphics::Color::Black, utils::Vector2f{-1.5, -2.f});
    chest->addComponent<components::core::LimitedSpaceActionComponent>(
        player.get(),
        [=]() { player->getComponent<components::core::ItemCollectorComponent>()->collect(item); });
    return chest;
}

}