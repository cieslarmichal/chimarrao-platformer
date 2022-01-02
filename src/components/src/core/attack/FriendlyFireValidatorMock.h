#pragma once

#include "gmock/gmock.h"

#include "FriendlyFireValidator.h"

namespace components::core
{
class FriendlyFireValidatorMock : public FriendlyFireValidator
{
public:
    MOCK_METHOD(FriendlyFireValidationResult, validate, (ComponentOwner * attacker, ComponentOwner* victim),
                (const override));
};
}