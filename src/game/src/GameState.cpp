#include "GameState.h"

#include <utility>

#include "GameStateUIConfigBuilder.h"
#include "HeadsUpDisplayUIConfigBuilder.h"
#include "ProjectPathReader.h"
#include "TimerFactory.h"
#include "core/FreeFallMovementComponent.h"
#include "ui/DefaultUIManager.h"

namespace game
{

GameState::GameState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                     std::shared_ptr<components::ui::UIManager> uiManagerInit,
                     std::unique_ptr<ComponentOwnersManager> componentOwnersManagerInit,
                     std::shared_ptr<TileMap> tileMapInit, std::shared_ptr<physics::RayCast> rayCastInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      uiManager{std::move(uiManagerInit)},
      componentOwnersManager{std::move(componentOwnersManagerInit)},
      tileMap{std::move(tileMapInit)},
      rayCast{std::move(rayCastInit)},
      characterFactory{std::make_unique<CharacterFactory>(rendererPool, tileMap, rayCast)}
{
    uiManager->createUI(GameStateUIConfigBuilder::createGameUIConfig(this));

    const auto playerPosition = utils::Vector2f{10.f, 10.f};
    auto player = characterFactory->createPlayer(playerPosition);

    const auto rabbitFollowerPosition = utils::Vector2f{2.f, 10.f};
    auto follower = characterFactory->createRabbitFollower(player, rabbitFollowerPosition);

    const auto npcPosition = utils::Vector2f{40.f, 20.f};
    auto npc = characterFactory->createDruidNpc(player, npcPosition);

    //    const auto enemy1Name = "enemy1";
    //    const auto enemy1BanditPosition = utils::Vector2f{60.f, 30.f};
    //    auto enemy = characterFactory->createBanditEnemy(enemy1Name, player, enemy1BanditPosition);

    hud = std::make_unique<HeadsUpDisplay>(player, rendererPool,
                                           HeadsUpDisplayUIConfigBuilder::createUIConfig());

    auto yerbaItem = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{30, 25}, "yerbaItem");
    yerbaItem->addGraphicsComponent(rendererPool, utils::Vector2f{2, 2}, utils::Vector2f{30, 25},
                                    utils::ProjectPathReader::getProjectRootPath() +
                                        "resources/yerba_item.png",
                                    graphics::VisibilityLayer::Second);
    yerbaItem->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{2, 2},
                                                                    components::core::CollisionLayer::Player);
    yerbaItem->addComponent<components::core::VelocityComponent>();
    yerbaItem->addComponent<components::core::FreeFallMovementComponent>();
    componentOwnersManager->add(yerbaItem);

    for (int x = 0; x < tileMap->getSize().x; x++)
    {
        for (int y = 0; y < tileMap->getSize().y; y++)
        {
            if (tileMap->getTile(utils::Vector2i{x, y})->type)
            {
                auto obstacle = std::make_shared<components::core::ComponentOwner>(
                    utils::Vector2f{static_cast<float>(x) * 4.f, static_cast<float>(y) * 4.f},
                    "obstacle" + std::to_string(x) + std::to_string(y) + std::to_string(y));
                obstacle->addGraphicsComponent(
                    rendererPool, utils::Vector2f{4, 4},
                    utils::Vector2f{static_cast<float>(x) * 4.f, static_cast<float>(y) * 4.f},
                    tileTypeToPathTexture(*(tileMap->getTile(utils::Vector2i{x, y})->type)),
                    graphics::VisibilityLayer::Second);
                obstacle->addComponent<components::core::BoxColliderComponent>(
                    utils::Vector2f{4, 4}, components::core::CollisionLayer::Tile);
                componentOwnersManager->add(obstacle);
            }
        }
    }
    auto leftMapBorder =
        std::make_shared<components::core::ComponentOwner>(utils::Vector2f{-1, 0}, "left border");
    leftMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1, tileMap->getSize().y * 4.f}, components::core::CollisionLayer::Default);
    auto topMapBorder =
        std::make_shared<components::core::ComponentOwner>(utils::Vector2f{0, -1}, "top border");
    topMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{tileMap->getSize().x * 4.f, 1}, components::core::CollisionLayer::Default);
    auto rightMapBorder = std::make_shared<components::core::ComponentOwner>(
        utils::Vector2f{tileMap->getSize().x * 4.f, 0}, "right border");
    rightMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1, tileMap->getSize().y * 4.f}, components::core::CollisionLayer::Default);
    auto bottomMapBorder = std::make_shared<components::core::ComponentOwner>(
        utils::Vector2f{0, tileMap->getSize().y * 4.f}, "bottom border");
    bottomMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{tileMap->getSize().x * 4.f, 1}, components::core::CollisionLayer::Default);
    componentOwnersManager->add(leftMapBorder);
    componentOwnersManager->add(topMapBorder);
    componentOwnersManager->add(rightMapBorder);
    componentOwnersManager->add(bottomMapBorder);

    componentOwnersManager->add(player);
    componentOwnersManager->add(follower);
    componentOwnersManager->add(npc);
    //    componentOwnersManager->add(enemy);
    componentOwnersManager->processNewObjects();

    timer = utils::TimerFactory::createTimer();
}

NextState GameState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (timer->getElapsedSeconds() > timeAfterStateCouldBePaused &&
        input.isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (not paused)
    {
        componentOwnersManager->update(deltaTime, input);
        uiManager->update(deltaTime, input);
        hud->update(deltaTime, input);
    }

    componentOwnersManager->processRemovals();

    return NextState::Same;
}

void GameState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void GameState::render()
{
    rendererPool->renderAll();
}

StateType GameState::getType() const
{
    return StateType::Game;
}

void GameState::activate()
{
    active = true;
    paused = false;
    timer->restart();
    componentOwnersManager->activate();
    uiManager->activate();
}

void GameState::deactivate()
{
    active = false;
    timer->restart();
    componentOwnersManager->deactivate();
    uiManager->deactivate();
}

void GameState::pause()
{
    paused = true;
    componentOwnersManager->deactivate();
    states.addNextState(StateType::Pause);
}

}
