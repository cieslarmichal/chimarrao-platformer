#include "LayoutTile.h"

#include "TileType.h"
#include "TimerFactory.h"

namespace game
{
LayoutTile::LayoutTile(const std::shared_ptr<graphics::RendererPool>& rendererPool,
                       const utils::Vector2i& position, const utils::Vector2f& sizeInit,
                       TileType& currentTileType, TileMap& tileMap)
    : timeAfterTileCanBeClicked{0.25f}
{
    componentOwner = std::make_shared<components::core::ComponentOwner>(
        utils::Vector2f{static_cast<float>(position.x) * sizeInit.x,
                        static_cast<float>(position.y) * sizeInit.y},
        std::to_string(position.x) + std::to_string(position.y) + std::to_string(position.y));
    const auto& graphicsComponent = componentOwner->addComponent<components::core::GraphicsComponent>(
        rendererPool, sizeInit,
        utils::Vector2f{static_cast<float>(position.x) * sizeInit.x,
                        static_cast<float>(position.y) * sizeInit.y},
        tileTypeToPathTexture(TileType::Grass),
        tileMap.getTile(position)->type ? graphics::VisibilityLayer::Second :
                                          graphics::VisibilityLayer::Invisible,
        true);
    graphicsComponent->setTexture(tileTypeToPathTexture(
        tileMap.getTile(position)->type ? *tileMap.getTile(position)->type : currentTileType));
    componentOwner->addComponent<components::core::BoxColliderComponent>(sizeInit);
    const auto onRightMouseButtonClickActionLambda = [=, &tileMap, &currentTileType]
    {
        currentTileType = getNextTileType(currentTileType);
        if (!tileMap.getTile(position)->type)
        {
            graphicsComponent->setTexture(tileTypeToPathTexture(currentTileType));
        }
    };
    const auto onLeftMouseButtonClickActionLambda = [=, &tileMap, &currentTileType]
    {
        if (!tileMap.getTile(position)->type)
        {
            tileMap.getTile(position)->type = currentTileType;
            graphicsComponent->setColor(graphics::Color(255, 255, 255, 255));
            graphicsComponent->setVisibility(graphics::VisibilityLayer::Second);
            graphicsComponent->setOutline(0.2f, graphics::Color::Red);
        }
        else
        {
            tileMap.getTile(position)->type = std::nullopt;
            graphicsComponent->setTexture(tileTypeToPathTexture(currentTileType));
            graphicsComponent->setVisibility(graphics::VisibilityLayer::Second);
            graphicsComponent->setColor(graphics::Color(255, 255, 255, 64));
            graphicsComponent->setOutline(0.2f, graphics::Color::Green);
        }
    };
    componentOwner->addComponent<components::core::ClickableComponent>(
        std::vector<components::core::KeyAction>{
            {input::InputKey::MouseRight, onRightMouseButtonClickActionLambda},
            {input::InputKey::MouseLeft, onLeftMouseButtonClickActionLambda}});
    const auto onMouseOverActionLambda = [=, &tileMap, &currentTileType]
    {
        graphicsComponent->setVisibility(graphics::VisibilityLayer::First);
        if (!tileMap.getTile(position)->type)
        {
            graphicsComponent->setTexture(tileTypeToPathTexture(currentTileType));
            graphicsComponent->setColor(graphics::Color(255, 255, 255, 64));
            graphicsComponent->setOutline(0.2f, graphics::Color::Green);
        }
        else
        {
            graphicsComponent->setOutline(0.2f, graphics::Color::Red);
        }
    };
    const auto onMouseOutActionLambda = [=, &tileMap]
    {
        if (!tileMap.getTile(position)->type)
        {
            graphicsComponent->setVisibility(graphics::VisibilityLayer::Invisible);
        }
        else
        {
            graphicsComponent->setVisibility(graphics::VisibilityLayer::Second);
        }
        graphicsComponent->setOutline(0.0f, graphics::Color::Transparent);
    };
    componentOwner->addComponent<components::core::MouseOverComponent>(onMouseOverActionLambda,
                                                                       onMouseOutActionLambda);
    componentOwner->loadDependentComponents();
    componentOwner->enable();
    freezeClickableTileTimer = utils::TimerFactory::createTimer();
    activate();
}

void LayoutTile::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (auto clickableComponent = componentOwner->getComponent<components::core::ClickableComponent>())
    {
        if (not clickableComponent->isEnabled() and
            freezeClickableTileTimer->getElapsedSeconds() > timeAfterTileCanBeClicked)
        {
            clickableComponent->enable();
        }
    }
    componentOwner->update(deltaTime, input);
    componentOwner->lateUpdate(deltaTime, input);
}

void LayoutTile::activate()
{
    componentOwner->enable();
    if (auto clickableComponent = componentOwner->getComponent<components::core::ClickableComponent>())
    {
        clickableComponent->disable();
        freezeClickableTileTimer->restart();
    }
}

void LayoutTile::deactivate()
{
    componentOwner->disable();
}

void LayoutTile::pause()
{
    componentOwner->disable();
    if (auto graphicsComponent = componentOwner->getComponent<components::core::GraphicsComponent>())
    {
        graphicsComponent->enable();
    }
}

void LayoutTile::moveTile(utils::Vector2f move)
{
    componentOwner->transform->addPosition(move);
}

const utils::Vector2f& LayoutTile::getPosition() const
{
    return componentOwner->transform->getPosition();
}

bool LayoutTile::isActive() const
{
    return componentOwner->transform->isEnabled();
}
}