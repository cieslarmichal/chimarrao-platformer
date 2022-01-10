#pragma once

#include "gmock/gmock.h"

#include "ComponentOwnersManager.h"

namespace components::core
{
class ComponentOwnersManagerMock : public ComponentOwnersManager
{
public:
    MOCK_METHOD(void, add, (std::shared_ptr<ComponentOwner>), (override));
    MOCK_METHOD(void, update, (const utils::DeltaTime&, const input::Input&), (override));
    MOCK_METHOD(void, processNewObjects, (), (override));
    MOCK_METHOD(void, processRemovals, (), (override));
    MOCK_METHOD(void, activate, (), (override));
    MOCK_METHOD(void, deactivate, (), (override));
};
}
