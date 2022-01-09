#include "Level1DialoguesController.h"

#include "gtest/gtest.h"

#include "DialoguesReaderMock.h"
#include "RendererPoolMock.h"
#include "TimerMock.h"

#include "DialogueActorComponent.h"
#include "DialogueTextComponent.h"
#include "KeyboardHorizontalMovementComponent.h"
#include "MovementComponent.h"
#include "ProjectPathReader.h"

using namespace game;
using namespace ::testing;

namespace
{
const std::string dialoguesDirectory = utils::ProjectPathReader::getProjectRootPath() + "dialogues/";
const std::string playerWithRabbitDialogueFile1 = dialoguesDirectory + "player_with_rabbit1.txt";
const std::string playerWithRabbitDialogueFile2 = dialoguesDirectory + "player_with_rabbit2.txt";
const std::string playerWithDruidDialogueFile1 = dialoguesDirectory + "player_with_druid1.txt";
const std::string playerWithDruidDialogueFile2 = dialoguesDirectory + "player_with_druid2.txt";
const std::vector<DialogueEntry> dialogueEntriesBetweenPlayerAndDruid1{
    {components::core::DialogueActor::Druid, "Oh, you found me"},
    {components::core::DialogueActor::Player, "Hi"},
    {components::core::DialogueActor::Player, "Do you know where can I find some food?"},
    {components::core::DialogueActor::Druid, "Have you seen those bushes?"},
    {components::core::DialogueActor::Player, "Yes, I guess so"}};
const std::vector<DialogueEntry> dialogueEntriesBetweenPlayerAndDruid2{
    {components::core::DialogueActor::Druid, "zzz"},
    {components::core::DialogueActor::Player, "xxx"},
    {components::core::DialogueActor::Player, "xxx"},
    {components::core::DialogueActor::Druid, "yyy"},
    {components::core::DialogueActor::Player, "aaa"}};
const std::vector<DialogueEntry> dialogueEntriesBetweenPlayerAndRabbit1{
    {components::core::DialogueActor::Player, "hello bunny"},
    {components::core::DialogueActor::Rabbit, "hello my love"},
    {components::core::DialogueActor::Player, "where are we going?"},
    {components::core::DialogueActor::Rabbit, "i dont know"}};
const std::vector<DialogueEntry> dialogueEntriesBetweenPlayerAndRabbit2{
    {components::core::DialogueActor::Rabbit, "1"},
    {components::core::DialogueActor::Player, "2"},
    {components::core::DialogueActor::Player, "3"},
    {components::core::DialogueActor::Rabbit, "4"}};
const utils::Vector2f position1{8, 0};
const utils::Vector2f position2{5, 0};
const utils::Vector2f position3{12, 0};
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto dummyFontSize = 4;
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
}

class Level1DialoguesControllerTest_Base : public Test
{
public:
    Level1DialoguesControllerTest_Base()
    {
        EXPECT_CALL(*dialoguesReader, read(playerWithRabbitDialogueFile1))
            .WillOnce(Return(dialogueEntriesBetweenPlayerAndRabbit1));
        EXPECT_CALL(*dialoguesReader, read(playerWithRabbitDialogueFile2))
            .WillOnce(Return(dialogueEntriesBetweenPlayerAndRabbit2));
        EXPECT_CALL(*dialoguesReader, read(playerWithDruidDialogueFile1))
            .WillOnce(Return(dialogueEntriesBetweenPlayerAndDruid1));
        EXPECT_CALL(*dialoguesReader, read(playerWithDruidDialogueFile2))
            .WillOnce(Return(dialogueEntriesBetweenPlayerAndDruid2));
    }

    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    std::unique_ptr<StrictMock<DialoguesReaderMock>> dialoguesReaderInit{
        std::make_unique<StrictMock<DialoguesReaderMock>>()};
    StrictMock<DialoguesReaderMock>* dialoguesReader{dialoguesReaderInit.get()};
    std::shared_ptr<components::core::ComponentOwner> player =
        std::make_shared<components::core::ComponentOwner>(position1, "Level1DialoguesControllerTest1",
                                                           sharedContext);
    std::shared_ptr<components::core::ComponentOwner> rabbit =
        std::make_shared<components::core::ComponentOwner>(position2, "Level1DialoguesControllerTest2",
                                                           sharedContext);
    std::shared_ptr<components::core::ComponentOwner> druid =
        std::make_shared<components::core::ComponentOwner>(position3, "Level1DialoguesControllerTest3",
                                                           sharedContext);
    Level1MainCharacters mainCharacters{player, rabbit, druid};
    std::unique_ptr<StrictMock<utils::TimerMock>> timerInit{std::make_unique<StrictMock<utils::TimerMock>>()};
    StrictMock<utils::TimerMock>* timer{timerInit.get()};
};

class Level1DialoguesControllerTest : public Level1DialoguesControllerTest_Base
{
public:
    Level1DialoguesControllerTest()
    {
        player->addComponent<components::core::KeyboardHorizontalMovementComponent>();
        auto playerText = player->addComponent<components::core::DialogueTextComponent>(
            rendererPool, position1, "xxx", fontPath, dummyFontSize);
        playerTextId = playerText->getGraphicsId();
        auto rabbitText = rabbit->addComponent<components::core::DialogueTextComponent>(
            rendererPool, position2, "yyy", fontPath, dummyFontSize);
        rabbitTextId = rabbitText->getGraphicsId();
        auto druidText = druid->addComponent<components::core::DialogueTextComponent>(
            rendererPool, position3, "zzz", fontPath, dummyFontSize);
        druidTextId = druidText->getGraphicsId();
    }

    Level1DialoguesController controller{&mainCharacters, std::move(dialoguesReaderInit),
                                         std::move(timerInit)};
    graphics::GraphicsId playerTextId;
    graphics::GraphicsId rabbitTextId;
    graphics::GraphicsId druidTextId;
};

TEST_F(Level1DialoguesControllerTest,
       startPlayerWithRabbitFirstDialogue_shouldLockPlayerMovementAndRestartTimer)
{
    EXPECT_CALL(*timer, restart());

    controller.startPlayerWithRabbitFirstDialogue();

    ASSERT_TRUE(player->getComponent<components::core::MovementComponent>()->isLocked());
}

TEST_F(Level1DialoguesControllerTest,
       startPlayerWithRabbitLastDialogue_shouldLockPlayerMovementAndRestartTimer)
{
    EXPECT_CALL(*timer, restart());

    controller.startPlayerWithRabbitFirstDialogue();

    ASSERT_TRUE(player->getComponent<components::core::MovementComponent>()->isLocked());
}

TEST_F(Level1DialoguesControllerTest, startPlayerWithDruidDialogue_shouldLockPlayerMovementAndRestartTimer)
{
    EXPECT_CALL(*timer, restart());

    controller.startPlayerWithDruidFirstDialogue();

    ASSERT_TRUE(player->getComponent<components::core::MovementComponent>()->isLocked());
}

class Level1DialoguesControllerTest_PlayerWithRabbitFirstDialogue : public Level1DialoguesControllerTest
{
public:
};

TEST_F(Level1DialoguesControllerTest_PlayerWithRabbitFirstDialogue,
       firstDialogueUpdate_shouldSetPlayerTextAndDisableRabbitText)
{
    EXPECT_CALL(*timer, restart()).Times(2);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillOnce(Return(5));
    EXPECT_CALL(*rendererPool, setText(playerTextId, "hello bunny"));
    controller.startPlayerWithRabbitFirstDialogue();

    controller.update();

    ASSERT_FALSE(rabbit->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_TRUE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithRabbitFirstDialogue,
       secondDialogueUpdate_shouldSetRabbitTextAndDisablePlayerText)
{
    EXPECT_CALL(*timer, restart()).Times(3);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    EXPECT_CALL(*rendererPool, setText(playerTextId, "hello bunny"));
    EXPECT_CALL(*rendererPool, setText(rabbitTextId, "hello my love"));
    controller.startPlayerWithRabbitFirstDialogue();

    controller.update();
    controller.update();

    ASSERT_FALSE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_TRUE(rabbit->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithRabbitFirstDialogue,
       after4Dialogues_shouldUnlockPlayerAndDisableTexts)
{
    EXPECT_CALL(*timer, restart()).Times(6);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    controller.startPlayerWithRabbitFirstDialogue();

    controller.update();
    controller.update();
    controller.update();
    controller.update();
    controller.update();

    ASSERT_FALSE(player->getComponent<components::core::MovementComponent>()->isLocked());
    ASSERT_FALSE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_FALSE(rabbit->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

class Level1DialoguesControllerTest_PlayerWithRabbitLastDialogue : public Level1DialoguesControllerTest
{
public:
};

TEST_F(Level1DialoguesControllerTest_PlayerWithRabbitLastDialogue,
       firstDialogueUpdate_shouldSetPlayerTextAndDisableRabbitText)
{
    EXPECT_CALL(*timer, restart()).Times(2);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillOnce(Return(5));
    EXPECT_CALL(*rendererPool, setText(rabbitTextId, "1"));
    controller.startPlayerWithRabbitLastDialogue();

    controller.update();

    ASSERT_FALSE(controller.hasPlayerWithRabbitLastDialogueFinished());
    ASSERT_TRUE(rabbit->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_FALSE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithRabbitLastDialogue,
       secondDialogueUpdate_shouldSetRabbitTextAndDisablePlayerText)
{
    EXPECT_CALL(*timer, restart()).Times(3);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    EXPECT_CALL(*rendererPool, setText(rabbitTextId, "1"));
    EXPECT_CALL(*rendererPool, setText(playerTextId, "2"));
    controller.startPlayerWithRabbitLastDialogue();

    controller.update();
    controller.update();

    ASSERT_FALSE(controller.hasPlayerWithRabbitLastDialogueFinished());
    ASSERT_TRUE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_FALSE(rabbit->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithRabbitLastDialogue,
       after4Dialogues_shouldUnlockPlayerAndDisableTexts)
{
    EXPECT_CALL(*timer, restart()).Times(6);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    controller.startPlayerWithRabbitLastDialogue();

    controller.update();
    controller.update();
    controller.update();
    controller.update();
    controller.update();

    ASSERT_TRUE(controller.hasPlayerWithRabbitLastDialogueFinished());
    ASSERT_FALSE(player->getComponent<components::core::MovementComponent>()->isLocked());
    ASSERT_FALSE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_FALSE(rabbit->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

class Level1DialoguesControllerTest_PlayerWithDruidFirstDialogue : public Level1DialoguesControllerTest
{
public:
};

TEST_F(Level1DialoguesControllerTest_PlayerWithDruidFirstDialogue,
       firstUpdate_shouldSetDruidTextAndDisablePlayerText)
{
    EXPECT_CALL(*timer, restart()).Times(2);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillOnce(Return(5));
    EXPECT_CALL(*rendererPool, setText(druidTextId, "Oh, you found me"));
    controller.startPlayerWithDruidFirstDialogue();

    controller.update();

    ASSERT_FALSE(controller.hasPlayerWithDruidFirstDialogueFinished());
    ASSERT_FALSE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_TRUE(druid->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithDruidFirstDialogue,
       secondDialogueUpdate_shouldSetPlayerTextAndDisableDruidText)
{
    EXPECT_CALL(*timer, restart()).Times(3);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    EXPECT_CALL(*rendererPool, setText(druidTextId, "Oh, you found me"));
    EXPECT_CALL(*rendererPool, setText(playerTextId, "Hi"));
    controller.startPlayerWithDruidFirstDialogue();

    controller.update();
    controller.update();

    ASSERT_FALSE(controller.hasPlayerWithDruidFirstDialogueFinished());
    ASSERT_FALSE(druid->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_TRUE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithDruidFirstDialogue,
       after5Dialogues_shouldUnlockPlayerAndDisableTexts)
{
    EXPECT_CALL(*timer, restart()).Times(7);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    controller.startPlayerWithDruidFirstDialogue();

    controller.update();
    controller.update();
    controller.update();
    controller.update();
    controller.update();
    controller.update();

    ASSERT_TRUE(controller.hasPlayerWithDruidFirstDialogueFinished());
    ASSERT_FALSE(player->getComponent<components::core::MovementComponent>()->isLocked());
    ASSERT_FALSE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_FALSE(druid->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

class Level1DialoguesControllerTest_PlayerWithDruidSecondDialogue : public Level1DialoguesControllerTest
{
public:
};

TEST_F(Level1DialoguesControllerTest_PlayerWithDruidSecondDialogue,
       firstUpdate_shouldSetDruidTextAndDisablePlayerText)
{
    EXPECT_CALL(*timer, restart()).Times(2);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillOnce(Return(5));
    EXPECT_CALL(*rendererPool, setText(druidTextId, "zzz"));
    controller.startPlayerWithDruidSecondDialogue();

    controller.update();

    ASSERT_FALSE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_TRUE(druid->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithDruidSecondDialogue,
       secondDialogueUpdate_shouldSetPlayerTextAndDisableDruidText)
{
    EXPECT_CALL(*timer, restart()).Times(3);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    EXPECT_CALL(*rendererPool, setText(druidTextId, "zzz"));
    EXPECT_CALL(*rendererPool, setText(playerTextId, "xxx"));
    controller.startPlayerWithDruidSecondDialogue();

    controller.update();
    controller.update();

    ASSERT_FALSE(druid->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_TRUE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithDruidSecondDialogue,
       after5Dialogues_shouldUnlockPlayerAndDisableTexts)
{
    EXPECT_CALL(*timer, restart()).Times(7);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    controller.startPlayerWithDruidSecondDialogue();

    controller.update();
    controller.update();
    controller.update();
    controller.update();
    controller.update();
    controller.update();

    ASSERT_FALSE(player->getComponent<components::core::MovementComponent>()->isLocked());
    ASSERT_FALSE(player->getComponent<components::core::DialogueTextComponent>()->isEnabled());
    ASSERT_FALSE(druid->getComponent<components::core::DialogueTextComponent>()->isEnabled());
}