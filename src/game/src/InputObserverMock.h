#pragma once

#include "gmock/gmock.h"

#include "InputObserver.h"

namespace game
{
class InputObserverMock : public InputObserver
{
public:
    MOCK_METHOD1(handleInputStatus, void(const InputStatus&));
};
}