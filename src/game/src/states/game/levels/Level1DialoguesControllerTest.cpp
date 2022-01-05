#include "Level1DialoguesController.h"

#include "gtest/gtest.h"

#include "DialoguesReaderMock.h"
#include "RendererPoolMock.h"

using namespace game;
using namespace ::testing;

namespace
{
const DialogueTrack dialogueTrackBetweenPlayerAndDruid{
    {components::core::DialogueActor::Druid, "Oh, you found me"},
    {components::core::DialogueActor::Player, "Hi"},
    {components::core::DialogueActor::Player, "Do you know where can I find some food?"},
    {components::core::DialogueActor::Druid, "Have you seen those bushes?"},
    {components::core::DialogueActor::Player, "Yes, I guess so"}};
const DialogueTrack dialogueTrackBetweenPlayerAndRabbit{{components::core::DialogueActor::Player, "hello bunny"},
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
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    std::unique_ptr<StrictMock<DialoguesReaderMock>> dialoguesReaderInit{
        std::make_unique<StrictMock<DialoguesReaderMock>>()};
    StrictMock<DialoguesReaderMock>* dialoguesReader{dialoguesReaderInit.get()};
    components::core::ComponentOwner druid{position1, "Level1DialoguesControllerTest1", sharedContext};
    components::core::ComponentOwner player{position2, "Level1DialoguesControllerTest2", sharedContext};
    components::core::ComponentOwner rabbit{position3, "Level1DialoguesControllerTest3", sharedContext};

    Level1DialoguesController controller{std::move(dialoguesReaderInit)};
};

TEST_F(Level1DialoguesControllerTest, fileAccessThrow_shouldThrow)
{
    EXPECT_CALL(*fileAccess, readContent(nonExistingFilePath))
        .WillOnce(Throw(utils::exceptions::FileNotFound{""}));

    ASSERT_THROW(reader.read(nonExistingFilePath), utils::exceptions::FileNotFound);
}

TEST_F(Level1DialoguesControllerTest, readValidFile_shouldReturnDialogueTrack)
{
    EXPECT_CALL(*fileAccess, readContent(validDialogueFilePath)).WillOnce(Return(validContent));

    const auto dialogueTrack = reader.read(validDialogueFilePath);

    ASSERT_EQ(dialogueTrack, expectedDialogueTrack);
}
