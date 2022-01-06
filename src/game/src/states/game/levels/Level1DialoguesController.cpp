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
      dialogueTimeToLive{2.5f}
{
}

void Level1DialoguesController::update()
{
    if (playerWithRabbitDialogue1IsOngoing)
    {
        handlePlayerWithRabbit1Dialogue();
    }
    else if (playerWithDruidDialogueIsOngoing)
    {
        handlePlayerWithDruidDialogue();
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

void Level1DialoguesController::handlePlayerWithRabbit1Dialogue()
{
    if (dialogueAliveTimer->getElapsedSeconds() >= dialogueTimeToLive)
    {
        if (const auto dialogueEntry = playerWithRabbitDialogueTrack1.getNextDialogue())
        {
            if (dialogueEntry->actor == components::core::DialogueActor::Player)
            {
                mainCharacters->rabbit->getComponent<components::core::TextComponent>()->disable();
                mainCharacters->player->getComponent<components::core::TextComponent>()->setText(
                    dialogueEntry->statement);
                mainCharacters->player->getComponent<components::core::TextComponent>()->enable();
            }

            if (dialogueEntry->actor == components::core::DialogueActor::Rabbit)
            {
                mainCharacters->player->getComponent<components::core::TextComponent>()->disable();
                mainCharacters->rabbit->getComponent<components::core::TextComponent>()->setText(
                    dialogueEntry->statement);
                mainCharacters->rabbit->getComponent<components::core::TextComponent>()->enable();
            }
        }
        else
        {
            finishPlayerWithRabbit1Dialogue();
        }

        dialogueAliveTimer->restart();
    }
}

void Level1DialoguesController::handlePlayerWithDruidDialogue()
{
    if (dialogueAliveTimer->getElapsedSeconds() >= dialogueTimeToLive)
    {
        if (const auto dialogueEntry = playerWithDruidDialogueTrack.getNextDialogue())
        {
            if (dialogueEntry->actor == components::core::DialogueActor::Player)
            {
                mainCharacters->npc->getComponent<components::core::TextComponent>()->disable();
                mainCharacters->player->getComponent<components::core::TextComponent>()->setText(
                    dialogueEntry->statement);
                mainCharacters->player->getComponent<components::core::TextComponent>()->enable();
            }

            if (dialogueEntry->actor == components::core::DialogueActor::Druid)
            {
                mainCharacters->player->getComponent<components::core::TextComponent>()->disable();
                mainCharacters->npc->getComponent<components::core::TextComponent>()->setText(
                    dialogueEntry->statement);
                mainCharacters->npc->getComponent<components::core::TextComponent>()->enable();

            }
        }
        else
        {
            finishPlayerWithDruidDialogue();
        }

        dialogueAliveTimer->restart();
    }
}

void Level1DialoguesController::finishPlayerWithRabbit1Dialogue()
{
    mainCharacters->player->getComponent<components::core::TextComponent>()->disable();
    mainCharacters->rabbit->getComponent<components::core::TextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::MovementComponent>()->unlock();
    playerWithRabbitDialogue1IsOngoing = false;
}

void Level1DialoguesController::finishPlayerWithDruidDialogue()
{
    mainCharacters->player->getComponent<components::core::TextComponent>()->disable();
    mainCharacters->npc->getComponent<components::core::TextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::MovementComponent>()->unlock();
    playerWithDruidDialogueIsOngoing = false;
}

}
