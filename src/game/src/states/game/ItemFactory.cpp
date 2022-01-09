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
const auto appleTexturePath = projectPath + "resources/apple.png";
const auto blueberriesTexturePath = projectPath + "resources/blueberries.png";
const auto keyTexturePath = projectPath + "resources/key.png";
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
    auto colliderComponent = yerbaItem->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2, 2}, components::core::CollisionLayer::Player);
    colliderComponent->disable();
    yerbaItem->addComponent<components::core::VelocityComponent>();
    yerbaItem->addComponent<components::core::FreeFallMovementComponent>();
    yerbaItem->addComponent<components::core::CollectableItemComponent>(
        "yerba" + std::to_string(numberOfYerbaItemsInGame), components::core::ItemType::Yerba,
        std::make_shared<components::core::ItemHealEffect>(100));
    return yerbaItem;
}

std::shared_ptr<components::core::ComponentOwner>
ItemFactory::createBlueberries(const utils::Vector2f& position)
{
    static int numberOfBlueberriesItemsInGame = 0;
    numberOfBlueberriesItemsInGame++;
    auto blueberriesItem = std::make_shared<components::core::ComponentOwner>(
        position, "blueberriesItem" + std::to_string(numberOfBlueberriesItemsInGame), sharedContext);
    blueberriesItem->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{2, 2}, position,
                                          blueberriesTexturePath, graphics::VisibilityLayer::Second);
    auto colliderComponent = blueberriesItem->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2, 2}, components::core::CollisionLayer::Player);
    colliderComponent->disable();
    blueberriesItem->addComponent<components::core::VelocityComponent>();
    blueberriesItem->addComponent<components::core::FreeFallMovementComponent>();
    blueberriesItem->addComponent<components::core::CollectableItemComponent>(
        "blueberries" + std::to_string(numberOfBlueberriesItemsInGame),
        components::core::ItemType::Blueberries, std::make_shared<components::core::ItemHealEffect>(20));
    return blueberriesItem;
}

std::shared_ptr<components::core::ComponentOwner> ItemFactory::createApple(const utils::Vector2f& position)
{
    static int numberOfAppleItemsInGame = 0;
    numberOfAppleItemsInGame++;
    auto appleItem = std::make_shared<components::core::ComponentOwner>(
        position, "appleItem" + std::to_string(numberOfAppleItemsInGame), sharedContext);
    appleItem->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{2, 2}, position,
                                    appleTexturePath, graphics::VisibilityLayer::Second);
    auto colliderComponent = appleItem->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2, 2}, components::core::CollisionLayer::Player);
    colliderComponent->disable();
    appleItem->addComponent<components::core::VelocityComponent>();
    appleItem->addComponent<components::core::FreeFallMovementComponent>();
    appleItem->addComponent<components::core::CollectableItemComponent>(
        "apple" + std::to_string(numberOfAppleItemsInGame), components::core::ItemType::Apple,
        std::make_shared<components::core::ItemHealEffect>(25));
    return appleItem;
}

std::shared_ptr<components::core::ComponentOwner> ItemFactory::createKey(const utils::Vector2f& position)
{
    static int numberOfKeysItemsInGame = 0;
    numberOfKeysItemsInGame++;
    auto keyItem = std::make_shared<components::core::ComponentOwner>(
        position, "keyItem" + std::to_string(numberOfKeysItemsInGame), sharedContext);
    keyItem->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{2, 2}, position,
                                  keyTexturePath, graphics::VisibilityLayer::Second);
    auto colliderComponent = keyItem->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2, 2}, components::core::CollisionLayer::Player);
    colliderComponent->disable();
    keyItem->addComponent<components::core::VelocityComponent>();
    keyItem->addComponent<components::core::FreeFallMovementComponent>();
    keyItem->addComponent<components::core::CollectableItemComponent>(
        "key" + std::to_string(numberOfKeysItemsInGame), components::core::ItemType::Key, nullptr);
    return keyItem;
}

}