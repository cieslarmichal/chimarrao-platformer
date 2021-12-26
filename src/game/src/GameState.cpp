#include "GameState.h"

#include <utility>

#include "GameStateUIConfigBuilder.h"
#include "HeadsUpDisplayUIConfigBuilder.h"
#include "ProjectPathReader.h"
#include "TimerFactory.h"
#include "ui/DefaultUIManager.h"

namespace game
{

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto soundtrackPath = projectPath + "resources/game_music_loop.wav";
}

GameState::GameState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                     std::shared_ptr<components::ui::UIManager> uiManagerInit,
                     std::unique_ptr<ComponentOwnersManager> componentOwnersManagerInit,
                     std::shared_ptr<TileMap> tileMapInit, std::shared_ptr<physics::RayCast> rayCastInit,
                     std::shared_ptr<physics::Quadtree> quadtreeInit,
                     const std::shared_ptr<components::core::SharedContext>& sharedContextInit,
                     std::shared_ptr<audio::MusicManager> musicManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      uiManager{std::move(uiManagerInit)},
      componentOwnersManager{std::move(componentOwnersManagerInit)},
      tileMap{std::move(tileMapInit)},
      rayCast{std::move(rayCastInit)},
      quadtree{std::move(quadtreeInit)},
      sharedContext{sharedContextInit},
      characterFactory{std::make_unique<CharacterFactory>(sharedContext, tileMap, rayCast, quadtree)},
      itemFactory{std::make_unique<ItemFactory>(sharedContext)},
      musicManager{std::move(musicManagerInit)}
{
    uiManager->createUI(GameStateUIConfigBuilder::createGameUIConfig(this));

    const auto playerPosition = utils::Vector2f{10.f, 10.f};
    auto player = characterFactory->createPlayer(playerPosition);

    const auto rabbitFollowerPosition = utils::Vector2f{2.f, 10.f};
    auto follower = characterFactory->createRabbitFollower(player, rabbitFollowerPosition);

    const auto npcPosition = utils::Vector2f{40.f, 20.f};
    auto npc = characterFactory->createDruidNpc(player, npcPosition);

    const auto enemy1BanditPosition = utils::Vector2f{60.f, 30.f};
    auto enemy = characterFactory->createBanditEnemy(player, enemy1BanditPosition);

    hud = std::make_unique<HeadsUpDisplay>(player, sharedContext,
                                           HeadsUpDisplayUIConfigBuilder::createUIConfig(),
                                           utils::TimerFactory::createTimer());

    const auto yerbaPosition1 = utils::Vector2f{30, 25};
    auto yerbaItem1 = itemFactory->createYerba(yerbaPosition1);

    const auto yerbaPosition2 = utils::Vector2f{50, 25};
    auto yerbaItem2 = itemFactory->createYerba(yerbaPosition2);

    for (int x = 0; x < tileMap->getSize().x; x++)
    {
        for (int y = 0; y < tileMap->getSize().y; y++)
        {
            if (tileMap->getTile(utils::Vector2i{x, y})->type)
            {
                auto obstacle = std::make_shared<components::core::ComponentOwner>(
                    utils::Vector2f{static_cast<float>(x) * 4.f, static_cast<float>(y) * 4.f},
                    "obstacle" + std::to_string(x) + std::to_string(y) + std::to_string(y), sharedContext);
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
    auto leftMapBorder = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{-1, 0},
                                                                            "left border", sharedContext);
    leftMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1, static_cast<float>(tileMap->getSize().y) * 4.f},
        components::core::CollisionLayer::Default);
    auto topMapBorder = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{0, -1},
                                                                           "top border", sharedContext);
    topMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{static_cast<float>(tileMap->getSize().x) * 4.f, 1},
        components::core::CollisionLayer::Default);
    auto rightMapBorder = std::make_shared<components::core::ComponentOwner>(
        utils::Vector2f{static_cast<float>(tileMap->getSize().x) * 4.f, 0}, "right border", sharedContext);
    rightMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1, static_cast<float>(tileMap->getSize().y) * 4.f},
        components::core::CollisionLayer::Default);
    auto bottomMapBorder = std::make_shared<components::core::ComponentOwner>(
        utils::Vector2f{0, static_cast<float>(tileMap->getSize().y) * 4.f}, "bottom border", sharedContext);
    bottomMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{static_cast<float>(tileMap->getSize().x) * 4.f, 1},
        components::core::CollisionLayer::Default);
    componentOwnersManager->add(leftMapBorder);
    componentOwnersManager->add(topMapBorder);
    componentOwnersManager->add(rightMapBorder);
    componentOwnersManager->add(bottomMapBorder);

    componentOwnersManager->add(player);
    componentOwnersManager->add(follower);
    componentOwnersManager->add(npc);
    componentOwnersManager->add(yerbaItem1);
    componentOwnersManager->add(yerbaItem2);
    componentOwnersManager->add(enemy);
    componentOwnersManager->processNewObjects();

    timer = utils::TimerFactory::createTimer();

    musicId = musicManager->acquire(soundtrackPath);
    musicManager->play(musicId);
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
    musicManager->play(musicId);
}

void GameState::deactivate()
{
    active = false;
    timer->restart();
    componentOwnersManager->deactivate();
    uiManager->deactivate();
    musicManager->stop(musicId);
}

void GameState::pause()
{
    paused = true;
    componentOwnersManager->deactivate();
    musicManager->pause(musicId);
    states.addNextState(StateType::Pause);
}

}
