#include "Level1DialoguesController.h"

#include "gtest/gtest.h"

#include "DialoguesReaderMock.h"
#include "RendererPoolMock.h"

#include "DialogueActorComponent.h"
#include "ProjectPathReader.h"

using namespace game;
using namespace ::testing;

namespace
{
const std::string dialoguesDirectory = utils::ProjectPathReader::getProjectRootPath() + "dialogues/";
const std::string playerWithRabbitDialogueFile1 = dialoguesDirectory + "player_with_rabbit1.txt";
const std::string playerWithDruidDialogueFile = dialoguesDirectory + "player_with_druid.txt";
const DialogueTrack dialogueTrackBetweenPlayerAndDruid{
    {components::core::DialogueActor::Druid, "Oh, you found me"},
    {components::core::DialogueActor::Player, "Hi"},
    {components::core::DialogueActor::Player, "Do you know where can I find some food?"},
    {components::core::DialogueActor::Druid, "Have you seen those bushes?"},
    {components::core::DialogueActor::Player, "Yes, I guess so"}};
const DialogueTrack dialogueTrackBetweenPlayerAndRabbit{
    {components::core::DialogueActor::Player, "hello bunny"},
    {components::core::DialogueActor::Rabbit, "hello my love"},
    {components::core::DialogueActor::Player, "where are we going?"},
    {components::core::DialogueActor::Rabbit, "i dont know"}};
const utils::Vector2f position1{8, 0};
const utils::Vector2f position2{5, 0};
const utils::Vector2f position3{12, 0};
}

class Level1DialoguesControllerTest : public Test
{
public:
    Level1DialoguesControllerTest()
    {
        druid->addComponent<components::core::DialogueActorComponent>(components::core::DialogueActor::Druid);
        player->addComponent<components::core::DialogueActorComponent>(
            components::core::DialogueActor::Player);
        rabbit->addComponent<components::core::DialogueActorComponent>(
            components::core::DialogueActor::Rabbit);
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
};

TEST_F(Level1DialoguesControllerTest, createDialogueController_shouldReadDialogueTracks)
{
    EXPECT_CALL(*dialoguesReader, read(playerWithRabbitDialogueFile1)).WillOnce(Return(dialogueTrackBetweenPlayerAndRabbit));
    EXPECT_CALL(*dialoguesReader, read(playerWithDruidDialogueFile)).WillOnce(Return(dialogueTrackBetweenPlayerAndDruid));

    Level1DialoguesController controller{&mainCharacters, std::move(dialoguesReaderInit)};
}

TEST_F(Level1DialoguesControllerTest, startPlayerWithRabbitDialogue)
{
}
