#include "Level1DialoguesController.h"

#include "gtest/gtest.h"

#include "DialoguesReaderMock.h"
#include "RendererPoolMock.h"
#include "TimerMock.h"

#include "DialogueActorComponent.h"
#include "KeyboardHorizontalMovementComponent.h"
#include "MovementComponent.h"
#include "ProjectPathReader.h"
#include "TextComponent.h"

using namespace game;
using namespace ::testing;

namespace
{
const std::string dialoguesDirectory = utils::ProjectPathReader::getProjectRootPath() + "dialogues/";
const std::string playerWithRabbitDialogueFile1 = dialoguesDirectory + "player_with_rabbit1.txt";
const std::string playerWithDruidDialogueFile = dialoguesDirectory + "player_with_druid.txt";
const std::vector<DialogueEntry> dialogueEntriesBetweenPlayerAndDruid{
    {components::core::DialogueActor::Druid, "Oh, you found me"},
    {components::core::DialogueActor::Player, "Hi"},
    {components::core::DialogueActor::Player, "Do you know where can I find some food?"},
    {components::core::DialogueActor::Druid, "Have you seen those bushes?"},
    {components::core::DialogueActor::Player, "Yes, I guess so"}};
const std::vector<DialogueEntry> dialogueEntriesBetweenPlayerAndRabbit{
    {components::core::DialogueActor::Player, "hello bunny"},
    {components::core::DialogueActor::Rabbit, "hello my love"},
    {components::core::DialogueActor::Player, "where are we going?"},
    {components::core::DialogueActor::Rabbit, "i dont know"}};
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
            .WillOnce(Return(dialogueEntriesBetweenPlayerAndRabbit));
        EXPECT_CALL(*dialoguesReader, read(playerWithDruidDialogueFile))
            .WillOnce(Return(dialogueEntriesBetweenPlayerAndDruid));
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
        auto playerText = player->addComponent<components::core::TextComponent>(
            rendererPool, position1, "xxx", fontPath, dummyFontSize);
        playerTextId = playerText->getGraphicsId();
        auto rabbitText = rabbit->addComponent<components::core::TextComponent>(
            rendererPool, position2, "yyy", fontPath, dummyFontSize);
        rabbitTextId = rabbitText->getGraphicsId();
        auto druidText = druid->addComponent<components::core::TextComponent>(rendererPool, position3, "zzz",
                                                                              fontPath, dummyFontSize);
        druidTextId = druidText->getGraphicsId();
    }

    Level1DialoguesController controller{&mainCharacters, std::move(dialoguesReaderInit),
                                         std::move(timerInit)};
    graphics::GraphicsId playerTextId;
    graphics::GraphicsId rabbitTextId;
    graphics::GraphicsId druidTextId;
};

TEST_F(Level1DialoguesControllerTest, startPlayerWithRabbitDialogue_shouldLockPlayerMovementAndRestartTimer)
{
    EXPECT_CALL(*timer, restart());

    controller.startPlayerWithRabbitDialogue();

    ASSERT_TRUE(player->getComponent<components::core::MovementComponent>()->isLocked());
}

TEST_F(Level1DialoguesControllerTest, startPlayerWithDruidDialogue_shouldLockPlayerMovementAndRestartTimer)
{
    EXPECT_CALL(*timer, restart());

    controller.startPlayerWithDruidDialogue();

    ASSERT_TRUE(player->getComponent<components::core::MovementComponent>()->isLocked());
}

class Level1DialoguesControllerTest_PlayerWithRabbitDialogue : public Level1DialoguesControllerTest
{
public:
};

TEST_F(Level1DialoguesControllerTest_PlayerWithRabbitDialogue,
       firstDialogueUpdate_shouldSetPlayerTextAndDisableRabbitText)
{
    EXPECT_CALL(*timer, restart()).Times(2);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillOnce(Return(5));
    EXPECT_CALL(*rendererPool, setText(playerTextId, "hello bunny"));
    controller.startPlayerWithRabbitDialogue();

    controller.update();

    ASSERT_FALSE(rabbit->getComponent<components::core::TextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithRabbitDialogue,
       secondDialogueUpdate_shouldSetRabbitTextAndDisablePlayerText)
{
    EXPECT_CALL(*timer, restart()).Times(3);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    EXPECT_CALL(*rendererPool, setText(playerTextId, "hello bunny"));
    EXPECT_CALL(*rendererPool, setText(rabbitTextId, "hello my love"));
    controller.startPlayerWithRabbitDialogue();

    controller.update();
    controller.update();

    ASSERT_FALSE(player->getComponent<components::core::TextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithRabbitDialogue,
       after4Dialogues_shouldUnlockPlayerAndDisableTexts)
{
    EXPECT_CALL(*timer, restart()).Times(6);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    controller.startPlayerWithRabbitDialogue();

    controller.update();
    controller.update();
    controller.update();
    controller.update();
    controller.update();

    ASSERT_FALSE(player->getComponent<components::core::MovementComponent>()->isLocked());
    ASSERT_FALSE(player->getComponent<components::core::TextComponent>()->isEnabled());
    ASSERT_FALSE(rabbit->getComponent<components::core::TextComponent>()->isEnabled());
}

class Level1DialoguesControllerTest_PlayerWithDruidDialogue : public Level1DialoguesControllerTest
{
public:
};

TEST_F(Level1DialoguesControllerTest_PlayerWithDruidDialogue,
       firstUpdate_shouldSetDruidTextAndDisablePlayerText)
{
    EXPECT_CALL(*timer, restart()).Times(2);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillOnce(Return(5));
    EXPECT_CALL(*rendererPool, setText(druidTextId, "Oh, you found me"));
    controller.startPlayerWithDruidDialogue();

    controller.update();

    ASSERT_FALSE(player->getComponent<components::core::TextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithDruidDialogue,
       secondDialogueUpdate_shouldSetPlayerTextAndDisableDruidText)
{
    EXPECT_CALL(*timer, restart()).Times(3);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    EXPECT_CALL(*rendererPool, setText(druidTextId, "Oh, you found me"));
    EXPECT_CALL(*rendererPool, setText(playerTextId, "Hi"));
    controller.startPlayerWithDruidDialogue();

    controller.update();
    controller.update();

    ASSERT_FALSE(druid->getComponent<components::core::TextComponent>()->isEnabled());
}

TEST_F(Level1DialoguesControllerTest_PlayerWithDruidDialogue,
       after5Dialogues_shouldUnlockPlayerAndDisableTexts)
{
    EXPECT_CALL(*timer, restart()).Times(7);
    EXPECT_CALL(*timer, getElapsedSeconds()).WillRepeatedly(Return(5));
    controller.startPlayerWithDruidDialogue();

    controller.update();
    controller.update();
    controller.update();
    controller.update();
    controller.update();
    controller.update();

    ASSERT_FALSE(player->getComponent<components::core::MovementComponent>()->isLocked());
    ASSERT_FALSE(player->getComponent<components::core::TextComponent>()->isEnabled());
    ASSERT_FALSE(druid->getComponent<components::core::TextComponent>()->isEnabled());
}