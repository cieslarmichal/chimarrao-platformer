#include "ItemFactory.h"

#include "BoxColliderComponent.h"
#include "CollectableItemComponent.h"
#include "FreeFallMovementComponent.h"
#include "ItemHealEffect.h"
#include "ProjectPathReader.h"
#include "TimerFactory.h"

namespace game
{
namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto yerbaTexturePath = projectPath + "resources/yerba_item.png";
const auto appleTexturePath = projectPath + "resources/Fantasy World/Food Icons/PNG/Transparent/apple.png";
const auto meatTexturePath = projectPath + "resources/Fantasy World/Food Icons/PNG/Transparent/meat.png";
}

ItemFactory::ItemFactory(const std::shared_ptr<components::core::SharedContext>& sharedContext)
    : sharedContext{sharedContext}
{
}

std::shared_ptr<components::core::ComponentOwner> ItemFactory::createYerba(const utils::Vector2f& position)
{
    static int numberOfYerbaItemsInGame = 0;
    numberOfYerbaItemsInGame++;
    auto yerbaItem = std::make_shared<components::core::ComponentOwner>(
        position, "yerbaItem" + std::to_string(numberOfYerbaItemsInGame), sharedContext);
    yerbaItem->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{2, 2}, position,
                                    yerbaTexturePath, graphics::VisibilityLayer::Second);
    yerbaItem->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{2, 2},
                                                                    components::core::CollisionLayer::Player);
    yerbaItem->addComponent<components::core::VelocityComponent>();
    yerbaItem->addComponent<components::core::FreeFallMovementComponent>();
    yerbaItem->addComponent<components::core::CollectableItemComponent>(
        "yerba" + std::to_string(numberOfYerbaItemsInGame), components::core::ItemType::Yerba,
        std::make_shared<components::core::ItemHealEffect>(10));
    return yerbaItem;
}

std::shared_ptr<components::core::ComponentOwner> ItemFactory::createApple(const utils::Vector2f& position)
{
    static int numberOfAppleItemsInGame = 0;
    numberOfAppleItemsInGame++;
    auto appleItem = std::make_shared<components::core::ComponentOwner>(
        position, "appleItem" + std::to_string(numberOfAppleItemsInGame), sharedContext);
    appleItem->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{2, 2}, position,
                                    appleTexturePath, graphics::VisibilityLayer::Second);
    appleItem->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{2, 2},
                                                                    components::core::CollisionLayer::Player);
    appleItem->addComponent<components::core::VelocityComponent>();
    appleItem->addComponent<components::core::FreeFallMovementComponent>();
    appleItem->addComponent<components::core::CollectableItemComponent>(
        "apple" + std::to_string(numberOfAppleItemsInGame), components::core::ItemType::Apple,
        std::make_shared<components::core::ItemHealEffect>(5));
    return appleItem;
}

std::shared_ptr<components::core::ComponentOwner> ItemFactory::createMeat(const utils::Vector2f& position)
{
    static int numberOfMeatItemsInGame = 0;
    numberOfMeatItemsInGame++;
    auto meatItem = std::make_shared<components::core::ComponentOwner>(
        position, "meatItem" + std::to_string(numberOfMeatItemsInGame), sharedContext);
    meatItem->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{2, 2}, position,
                                   meatTexturePath, graphics::VisibilityLayer::Second);
    meatItem->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{2, 2},
                                                                   components::core::CollisionLayer::Player);
    meatItem->addComponent<components::core::VelocityComponent>();
    meatItem->addComponent<components::core::FreeFallMovementComponent>();
    meatItem->addComponent<components::core::CollectableItemComponent>(
        "meat" + std::to_string(numberOfMeatItemsInGame), components::core::ItemType::Meat,
        std::make_shared<components::core::ItemHealEffect>(5));
    return meatItem;
}
}