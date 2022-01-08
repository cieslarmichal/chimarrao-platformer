#include "Level1DialoguesController.h"

#include "DialogueTextComponent.h"
#include "MovementComponent.h"
#include "ProjectPathReader.h"

namespace game
{
namespace
{
const std::string dialoguesDirectory = utils::ProjectPathReader::getProjectRootPath() + "dialogues/";
const std::string playerWithRabbitDialogueFile1 = dialoguesDirectory + "player_with_rabbit1.txt";
const std::string playerWithRabbitDialogueFile2 = dialoguesDirectory + "player_with_rabbit2.txt";
const std::string playerWithDruidDialogueFile = dialoguesDirectory + "player_with_druid.txt";
}

Level1DialoguesController::Level1DialoguesController(Level1MainCharacters* level1MainCharacters,
                                                     std::unique_ptr<DialoguesReader> dialoguesReaderInit,
                                                     std::unique_ptr<utils::Timer> timerInit)
    : mainCharacters{level1MainCharacters},
      dialoguesReader{std::move(dialoguesReaderInit)},
      playerWithRabbitFirstDialogueTrack{dialoguesReader->read(playerWithRabbitDialogueFile1)},
      playerWithRabbitLastDialogueTrack{dialoguesReader->read(playerWithRabbitDialogueFile2)},
      playerWithDruidDialogueTrack{dialoguesReader->read(playerWithDruidDialogueFile)},
      dialogueAliveTimer{std::move(timerInit)},
      dialogueTimeToLive{2.5f}
{
}

void Level1DialoguesController::update()
{
    if (playerWithRabbitFirstDialogueIsOngoing)
    {
        handlePlayerWithRabbitFirstDialogue();
    }
    else if (playerWithDruidDialogueIsOngoing)
    {
        handlePlayerWithDruidDialogue();
    }
    else if (playerWithRabbitLastDialogueIsOngoing)
    {
        handlePlayerWithRabbitLastDialogue();
    }
}

void Level1DialoguesController::startPlayerWithRabbitFirstDialogue()
{
    playerWithRabbitFirstDialogueIsOngoing = true;
    mainCharacters->player->getComponent<components::core::MovementComponent>()->lock();
    dialogueAliveTimer->restart();
}

void Level1DialoguesController::startPlayerWithRabbitLastDialogue()
{
    playerWithRabbitLastDialogueIsOngoing = true;
    mainCharacters->player->getComponent<components::core::MovementComponent>()->lock();
    dialogueAliveTimer->restart();
}

void Level1DialoguesController::startPlayerWithDruidDialogue()
{
    playerWithDruidDialogueIsOngoing = true;
    mainCharacters->player->getComponent<components::core::MovementComponent>()->lock();
    dialogueAliveTimer->restart();
}

void Level1DialoguesController::handlePlayerWithRabbitFirstDialogue()
{
    if (dialogueAliveTimer->getElapsedSeconds() >= dialogueTimeToLive)
    {
        if (const auto dialogueEntry = playerWithRabbitFirstDialogueTrack.getNextDialogue())
        {
            if (dialogueEntry->actor == components::core::DialogueActor::Player)
            {
                mainCharacters->rabbit->getComponent<components::core::DialogueTextComponent>()->disable();
                mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->setText(
                    dialogueEntry->statement);
                mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->enable();
            }

            if (dialogueEntry->actor == components::core::DialogueActor::Rabbit)
            {
                mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->disable();
                mainCharacters->rabbit->getComponent<components::core::DialogueTextComponent>()->setText(
                    dialogueEntry->statement);
                mainCharacters->rabbit->getComponent<components::core::DialogueTextComponent>()->enable();
            }
        }
        else
        {
            finishPlayerWithRabbitFirstDialogue();
        }

        dialogueAliveTimer->restart();
    }
}

void Level1DialoguesController::handlePlayerWithRabbitLastDialogue()
{
    if (dialogueAliveTimer->getElapsedSeconds() >= dialogueTimeToLive)
    {
        if (const auto dialogueEntry = playerWithRabbitLastDialogueTrack.getNextDialogue())
        {
            if (dialogueEntry->actor == components::core::DialogueActor::Player)
            {
                mainCharacters->rabbit->getComponent<components::core::DialogueTextComponent>()->disable();
                mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->setText(
                    dialogueEntry->statement);
                mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->enable();
            }

            if (dialogueEntry->actor == components::core::DialogueActor::Rabbit)
            {
                mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->disable();
                mainCharacters->rabbit->getComponent<components::core::DialogueTextComponent>()->setText(
                    dialogueEntry->statement);
                mainCharacters->rabbit->getComponent<components::core::DialogueTextComponent>()->enable();
            }
        }
        else
        {
            finishPlayerWithRabbitLastDialogue();
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
                mainCharacters->npc->getComponent<components::core::DialogueTextComponent>()->disable();
                mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->setText(
                    dialogueEntry->statement);
                mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->enable();
            }

            if (dialogueEntry->actor == components::core::DialogueActor::Druid)
            {
                mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->disable();
                mainCharacters->npc->getComponent<components::core::DialogueTextComponent>()->setText(
                    dialogueEntry->statement);
                mainCharacters->npc->getComponent<components::core::DialogueTextComponent>()->enable();
            }
        }
        else
        {
            finishPlayerWithDruidDialogue();
        }

        dialogueAliveTimer->restart();
    }
}

void Level1DialoguesController::finishPlayerWithRabbitFirstDialogue()
{
    mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->rabbit->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::MovementComponent>()->unlock();
    playerWithRabbitFirstDialogueIsOngoing = false;
}

void Level1DialoguesController::finishPlayerWithRabbitLastDialogue()
{
    mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->rabbit->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::MovementComponent>()->unlock();
    playerWithRabbitLastDialogueIsOngoing = false;
}

void Level1DialoguesController::finishPlayerWithDruidDialogue()
{
    mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->npc->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::MovementComponent>()->unlock();
    playerWithDruidDialogueIsOngoing = false;
}

bool Level1DialoguesController::hasPlayerWithRabbitLastDialogueFinished()
{
    return not playerWithRabbitLastDialogueIsOngoing;
}

}
