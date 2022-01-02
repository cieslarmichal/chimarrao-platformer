#include "DefaultFriendlyFireValidator.h"

#include "gtest/gtest.h"

#include "AttackStrategyMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"

#include "exceptions/DependentComponentNotFound.h"

using namespace ::testing;
using namespace components::core;

class DefaultFriendlyFireValidatorTest : public Test
{
public:
    const utils::Vector2f position{20, 20};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner defaultOwner{position, "FriendlyFireValidatorTest1", sharedContext};
    ComponentOwner playerOwner{position, "FriendlyFireValidatorTest2", sharedContext,
                               ComponentOwnerType::Player};
    ComponentOwner friendOwner{position, "FriendlyFireValidatorTest3", sharedContext,
                               ComponentOwnerType::Friend};
    ComponentOwner enemyOwner{position, "FriendlyFireValidatorTest4", sharedContext,
                              ComponentOwnerType::Enemy};
    DefaultFriendlyFireValidator validator;
};

TEST_F(DefaultFriendlyFireValidatorTest, givenNotValidOwners_shouldReturnAttackNotAllowed)
{
    ASSERT_EQ(validator.validate(nullptr, nullptr), FriendlyFireValidationResult::AttackNotAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenPlayerAndDefault_shouldReturnAttackAllowed)
{
    ASSERT_EQ(validator.validate(&playerOwner, &defaultOwner), FriendlyFireValidationResult::AttackAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenPlayerAndPlayer_shouldReturnAttackNotAllowed)
{
    ASSERT_EQ(validator.validate(&playerOwner, &playerOwner), FriendlyFireValidationResult::AttackNotAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenPlayerAndFriend_shouldReturnAttackNotAllowed)
{
    ASSERT_EQ(validator.validate(&playerOwner, &friendOwner), FriendlyFireValidationResult::AttackNotAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenPlayerAndEnemy_shouldReturnAttackAllowed)
{
    ASSERT_EQ(validator.validate(&playerOwner, &enemyOwner), FriendlyFireValidationResult::AttackAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenFriendAndDefault_shouldReturnAttackAllowed)
{
    ASSERT_EQ(validator.validate(&friendOwner, &defaultOwner), FriendlyFireValidationResult::AttackAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenFriendAndPlayer_shouldReturnAttackNotAllowed)
{
    ASSERT_EQ(validator.validate(&friendOwner, &playerOwner), FriendlyFireValidationResult::AttackNotAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenFriendAndFriend_shouldReturnAttackNotAllowed)
{
    ASSERT_EQ(validator.validate(&friendOwner, &friendOwner), FriendlyFireValidationResult::AttackNotAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenFriendAndEnemy_shouldReturnAttackAllowed)
{
    ASSERT_EQ(validator.validate(&friendOwner, &enemyOwner), FriendlyFireValidationResult::AttackAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenEnemyAndDefault_shouldReturnAttackAllowed)
{
    ASSERT_EQ(validator.validate(&enemyOwner, &defaultOwner), FriendlyFireValidationResult::AttackAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenEnemyAndPlayer_shouldReturnAttackAllowed)
{
    ASSERT_EQ(validator.validate(&enemyOwner, &playerOwner), FriendlyFireValidationResult::AttackAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenEnemyAndFriend_shouldReturnAttackAllowed)
{
    ASSERT_EQ(validator.validate(&enemyOwner, &friendOwner), FriendlyFireValidationResult::AttackAllowed);
}

TEST_F(DefaultFriendlyFireValidatorTest, givenEnemyAndEnemy_shouldReturnAttackNotAllowed)
{
    ASSERT_EQ(validator.validate(&enemyOwner, &enemyOwner), FriendlyFireValidationResult::AttackNotAllowed);
}
