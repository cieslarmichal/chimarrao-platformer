#pragma once

#include "gmock/gmock.h"

#include "RayCast.h"

namespace physics
{
class RayCastMock : public RayCast
{
public:
    MOCK_METHOD(RayCastResult, cast,
                (const utils::Vector2f& from, const utils::Vector2f& to, unsigned int exclusionID,
                 float lineWidth),
                (const override));
};
}