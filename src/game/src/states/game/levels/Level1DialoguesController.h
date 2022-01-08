#pragma once

#include <mutex>

#include "DefaultDialoguesReader.h"
#include "Level1MainCharacters.h"
#include "Timer.h"

namespace game
{
class Level1DialoguesController
{
public:
    Level1DialoguesController(Level1MainCharacters*, std::unique_ptr<DialoguesReader>,
                              std::unique_ptr<utils::Timer>);

    void update();
    void startPlayerWithRabbitFirstDialogue();
    void startPlayerWithRabbitLastDialogue();
    void startPlayerWithDruidDialogue();
    bool hasPlayerWithRabbitLastDialogueFinished();

private:
    void handlePlayerWithRabbitFirstDialogue();
    void handlePlayerWithRabbitLastDialogue();
    void handlePlayerWithDruidDialogue();
    void finishPlayerWithRabbitFirstDialogue();
    void finishPlayerWithRabbitLastDialogue();
    void finishPlayerWithDruidDialogue();

    Level1MainCharacters* mainCharacters;
    std::unique_ptr<DialoguesReader> dialoguesReader;
    DialogueTrack playerWithRabbitFirstDialogueTrack;
    DialogueTrack playerWithRabbitLastDialogueTrack;
    DialogueTrack playerWithDruidDialogueTrack;
    bool playerWithRabbitFirstDialogueIsOngoing{false};
    bool playerWithRabbitLastDialogueIsOngoing{false};
    bool playerWithDruidDialogueIsOngoing{false};
    std::unique_ptr<utils::Timer> dialogueAliveTimer;
    const float dialogueTimeToLive;
};
}
