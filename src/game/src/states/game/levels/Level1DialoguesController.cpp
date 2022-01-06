#include "Level1DialoguesController.h"

#include "MovementComponent.h"
#include "ProjectPathReader.h"
#include "TextComponent.h"

namespace game
{
namespace
{
const std::string dialoguesDirectory = utils::ProjectPathReader::getProjectRootPath() + "dialogues/";
const std::string playerWithRabbitDialogueFile1 = dialoguesDirectory + "player_with_rabbit1.txt";
const std::string playerWithDruidDialogueFile = dialoguesDirectory + "player_with_druid.txt";
}

Level1DialoguesController::Level1DialoguesController(Level1MainCharacters* level1MainCharacters,
                                                     std::unique_ptr<DialoguesReader> dialoguesReaderInit,
                                                     std::unique_ptr<utils::Timer> timerInit)
    : mainCharacters{level1MainCharacters},
      dialoguesReader{std::move(dialoguesReaderInit)},
      playerWithRabbitDialogueTrack1{dialoguesReader->read(playerWithRabbitDialogueFile1)},
      playerWithDruidDialogueTrack{dialoguesReader->read(playerWithDruidDialogueFile)},
      dialogueAliveTimer{std::move(timerInit)},
      dialogueTimeToLive{1.5f}
{
}

void Level1DialoguesController::update()
{
    if (playerWithRabbitDialogue1IsOngoing)
    {
        if (dialogueAliveTimer->getElapsedSeconds() >= dialogueTimeToLive)
        {
        }
//        for (const auto& dialogueEntry : playerWithRabbitDialogueTrack1)
//        {
//            if (dialogueEntry.actor == components::core::DialogueActor::Player)
//            {
//                mainCharacters->player->getComponent<components::core::TextComponent>()->setText(
//                    dialogueEntry.statement);
//            }
//
//            if (dialogueEntry.actor == components::core::DialogueActor::Rabbit)
//            {
//                mainCharacters->rabbit->getComponent<components::core::TextComponent>()->setText(
//                    dialogueEntry.statement);
//            }
//        }
    }
}

void Level1DialoguesController::startPlayerWithRabbitDialogue()
{
    playerWithRabbitDialogue1IsOngoing = true;
    mainCharacters->player->getComponent<components::core::MovementComponent>()->lock();
    dialogueAliveTimer->restart();
}

void Level1DialoguesController::startPlayerWithDruidDialogue()
{
    playerWithDruidDialogueIsOngoing = true;
    mainCharacters->player->getComponent<components::core::MovementComponent>()->lock();
    dialogueAliveTimer->restart();
}

}
