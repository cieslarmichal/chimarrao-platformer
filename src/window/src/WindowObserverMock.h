#pragma once

#include "gmock/gmock.h"

namespace window
{
class WindowObserverMock : public WindowObserver
{
public:
    MOCK_METHOD(void, handleWindowSizeChange, (const utils::Vector2u& windowSize), (override));
};
}
