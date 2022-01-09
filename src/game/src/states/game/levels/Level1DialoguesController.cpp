#include "Level1DialoguesController.h"

#include "DialogueTextComponent.h"
#include "KeyboardAttackComponent.h"
#include "MovementComponent.h"
#include "ProjectPathReader.h"

namespace game
{
namespace
{
const std::string dialoguesDirectory = utils::ProjectPathReader::getProjectRootPath() + "dialogues/";
const std::string playerWithRabbitDialogueFile1 = dialoguesDirectory + "player_with_rabbit1.txt";
const std::string playerWithRabbitDialogueFile2 = dialoguesDirectory + "player_with_rabbit2.txt";
const std::string playerWithDruidDialogueFile1 = dialoguesDirectory + "player_with_druid1.txt";
const std::string playerWithDruidDialogueFile2 = dialoguesDirectory + "player_with_druid2.txt";
}

Level1DialoguesController::Level1DialoguesController(Level1MainCharacters* level1MainCharacters,
                                                     std::unique_ptr<DialoguesReader> dialoguesReaderInit,
                                                     std::unique_ptr<utils::Timer> dialogueAliveTimerInit,
                                                     std::unique_ptr<utils::Timer> dialogueSkipTimerInit)
    : mainCharacters{level1MainCharacters},
      dialoguesReader{std::move(dialoguesReaderInit)},
      playerWithRabbitFirstDialogueTrack{dialoguesReader->read(playerWithRabbitDialogueFile1)},
      playerWithRabbitLastDialogueTrack{dialoguesReader->read(playerWithRabbitDialogueFile2)},
      playerWithDruidFirstDialogueTrack{dialoguesReader->read(playerWithDruidDialogueFile1)},
      playerWithDruidSecondDialogueTrack{dialoguesReader->read(playerWithDruidDialogueFile2)},
      dialogueAliveTimer{std::move(dialogueAliveTimerInit)},
      dialogueTimeToLive{2.5f},
      dialogueSkipTimer{std::move(dialogueSkipTimerInit)},
      dialogueMinimumSkipTime{0.2f}
{
}

void Level1DialoguesController::update(const input::Input& input)
{
    if (playerWithRabbitFirstDialogueIsOngoing)
    {
        handlePlayerWithRabbitFirstDialogue(input);
    }
    else if (playerWithDruidFirstDialogueIsOngoing)
    {
        handlePlayerWithDruidFirstDialogue(input);
    }
    else if (playerWithDruidSecondDialogueIsOngoing)
    {
        handlePlayerWithDruidSecondDialogue(input);
    }
    else if (playerWithRabbitLastDialogueIsOngoing)
    {
        handlePlayerWithRabbitLastDialogue(input);
    }
}

void Level1DialoguesController::startPlayerWithRabbitFirstDialogue()
{
    playerWithRabbitFirstDialogueIsOngoing = true;
    mainCharacters->player->getComponent<components::core::MovementComponent>()->lock();
    mainCharacters->player->getComponent<components::core::KeyboardAttackComponent>()->disable();
    dialogueAliveTimer->restart();
}

void Level1DialoguesController::startPlayerWithRabbitLastDialogue()
{
    playerWithRabbitLastDialogueIsOngoing = true;
    mainCharacters->player->getComponent<components::core::MovementComponent>()->lock();
    mainCharacters->player->getComponent<components::core::KeyboardAttackComponent>()->disable();
    dialogueAliveTimer->restart();
}

void Level1DialoguesController::startPlayerWithDruidFirstDialogue()
{
    playerWithDruidFirstDialogueIsOngoing = true;
    mainCharacters->player->getComponent<components::core::MovementComponent>()->lock();
    mainCharacters->player->getComponent<components::core::KeyboardAttackComponent>()->disable();
    dialogueAliveTimer->restart();
}

void Level1DialoguesController::startPlayerWithDruidSecondDialogue()
{
    playerWithDruidSecondDialogueIsOngoing = true;
    mainCharacters->player->getComponent<components::core::MovementComponent>()->lock();
    mainCharacters->player->getComponent<components::core::KeyboardAttackComponent>()->disable();
    dialogueAliveTimer->restart();
}

void Level1DialoguesController::handlePlayerWithRabbitFirstDialogue(const input::Input& input)
{
    if (dialogueAliveTimer->getElapsedSeconds() >= dialogueTimeToLive or
        (input.isKeyPressed(input::InputKey::Space) and
         dialogueSkipTimer->getElapsedSeconds() >= dialogueMinimumSkipTime))
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
        dialogueSkipTimer->restart();
    }
}

void Level1DialoguesController::handlePlayerWithRabbitLastDialogue(const input::Input& input)
{
    if (dialogueAliveTimer->getElapsedSeconds() >= dialogueTimeToLive or
        (input.isKeyPressed(input::InputKey::Space) and
         dialogueSkipTimer->getElapsedSeconds() >= dialogueMinimumSkipTime))
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
        dialogueSkipTimer->restart();
    }
}

void Level1DialoguesController::handlePlayerWithDruidFirstDialogue(const input::Input& input)
{
    if (dialogueAliveTimer->getElapsedSeconds() >= dialogueTimeToLive or
        (input.isKeyPressed(input::InputKey::Space) and
         dialogueSkipTimer->getElapsedSeconds() >= dialogueMinimumSkipTime))
    {
        if (const auto dialogueEntry = playerWithDruidFirstDialogueTrack.getNextDialogue())
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
            finishPlayerWithDruidFirstDialogue();
        }

        dialogueAliveTimer->restart();
        dialogueSkipTimer->restart();
    }
}

void Level1DialoguesController::handlePlayerWithDruidSecondDialogue(const input::Input& input)
{
    if (dialogueAliveTimer->getElapsedSeconds() >= dialogueTimeToLive or
        (input.isKeyPressed(input::InputKey::Space) and
         dialogueSkipTimer->getElapsedSeconds() >= dialogueMinimumSkipTime))
    {
        if (const auto dialogueEntry = playerWithDruidSecondDialogueTrack.getNextDialogue())
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
            finishPlayerWithDruidSecondDialogue();
        }

        dialogueAliveTimer->restart();
        dialogueSkipTimer->restart();
    }
}

void Level1DialoguesController::finishPlayerWithRabbitFirstDialogue()
{
    mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::KeyboardAttackComponent>()->enable();
    mainCharacters->rabbit->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::MovementComponent>()->unlock();
    playerWithRabbitFirstDialogueIsOngoing = false;
}

void Level1DialoguesController::finishPlayerWithRabbitLastDialogue()
{
    mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::KeyboardAttackComponent>()->enable();
    mainCharacters->rabbit->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::MovementComponent>()->unlock();
    playerWithRabbitLastDialogueIsOngoing = false;
}

void Level1DialoguesController::finishPlayerWithDruidFirstDialogue()
{
    mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::KeyboardAttackComponent>()->enable();
    mainCharacters->npc->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::MovementComponent>()->unlock();
    playerWithDruidFirstDialogueIsOngoing = false;
}

void Level1DialoguesController::finishPlayerWithDruidSecondDialogue()
{
    mainCharacters->player->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::KeyboardAttackComponent>()->enable();
    mainCharacters->npc->getComponent<components::core::DialogueTextComponent>()->disable();
    mainCharacters->player->getComponent<components::core::MovementComponent>()->unlock();
    playerWithDruidSecondDialogueIsOngoing = false;
}

bool Level1DialoguesController::hasPlayerWithRabbitLastDialogueFinished()
{
    return not playerWithRabbitLastDialogueIsOngoing;
}

bool Level1DialoguesController::hasPlayerWithDruidFirstDialogueFinished()
{
    return not playerWithDruidFirstDialogueIsOngoing;
}

}
