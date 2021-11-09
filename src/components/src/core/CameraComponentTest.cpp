#include "CameraComponent.h"

#include <RendererPoolMock.h>

#include "gtest/gtest.h"

#include "InputMock.h"

#include "ComponentOwner.h"

using namespace components::core;
using namespace ::testing;

namespace
{
const utils::FloatRect mapRect{0, 0, 160, 120};
const utils::Vector2f viewSize{80, 60};
const utils::Vector2f positionOnLeftMapBoundary{3, 60};
const utils::Vector2f cameraPositionWhenOwnerOnLeftBoundary{40, 60};
const utils::Vector2f positionOnTopMapBoundary{80, 7};
const utils::Vector2f cameraPositionWhenOwnerOnTopBoundary{80, 30};
const utils::Vector2f positionOnRightMapBoundary{154, 60};
const utils::Vector2f cameraPositionWhenOwnerOnRightBoundary{120, 60};
const utils::Vector2f positionOnBottomMapBoundary{80, 111};
const utils::Vector2f cameraPositionWhenOwnerOnBottomBoundary{80, 90};
const utils::Vector2f positionAtMapCenter{87, 68};
const utils::Vector2f cameraPositionWhenOwnerAtMapCenter{87, 68};
}

class CameraComponentTest : public Test
{
public:
    CameraComponentTest()
    {
        componentOwner.addComponent<CameraComponent>(rendererPool, mapRect);
        componentOwner.loadDependentComponents();
        EXPECT_CALL(*rendererPool, getViewSize()).WillOnce(ReturnRef(viewSize));
    }

    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{{0, 0}, "owner", sharedContext};
    utils::DeltaTime deltaTime{1};
    StrictMock<input::InputMock> input;
};

TEST_F(CameraComponentTest, ownerOnLeftMapBoundary_shouldNotCenterViewOnOwenr)
{
    componentOwner.transform->setPosition(positionOnLeftMapBoundary);

    EXPECT_CALL(*rendererPool, setCenter(cameraPositionWhenOwnerOnLeftBoundary));

    componentOwner.lateUpdate(deltaTime, input);
}

TEST_F(CameraComponentTest, ownerOnTopMapBoundary_shouldNotCenterViewOnOwenr)
{
    componentOwner.transform->setPosition(positionOnTopMapBoundary);

    EXPECT_CALL(*rendererPool, setCenter(cameraPositionWhenOwnerOnTopBoundary));

    componentOwner.lateUpdate(deltaTime, input);
}

TEST_F(CameraComponentTest, ownerOnRightMapBoundary_shouldNotCenterViewOnOwenr)
{
    componentOwner.transform->setPosition(positionOnRightMapBoundary);

    EXPECT_CALL(*rendererPool, setCenter(cameraPositionWhenOwnerOnRightBoundary));

    componentOwner.lateUpdate(deltaTime, input);
}

TEST_F(CameraComponentTest, ownerOnBottomMapBoundary_shouldNotCenterViewOnOwenr)
{
    componentOwner.transform->setPosition(positionOnBottomMapBoundary);

    EXPECT_CALL(*rendererPool, setCenter(cameraPositionWhenOwnerOnBottomBoundary));

    componentOwner.lateUpdate(deltaTime, input);
}

TEST_F(CameraComponentTest, ownerAtCenterMapBoundary_shouldCenterViewOnOwenr)
{
    componentOwner.transform->setPosition(positionAtMapCenter);

    EXPECT_CALL(*rendererPool, setCenter(cameraPositionWhenOwnerAtMapCenter));

    componentOwner.lateUpdate(deltaTime, input);
}