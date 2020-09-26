#pragma once

#include "gmock/gmock.h"

namespace window
{
class WindowObserverMock
{
public:
    MOCK_METHOD(void, windowSizeChanged, (const utils::Vector2u& windowSize), (override));
};
}
