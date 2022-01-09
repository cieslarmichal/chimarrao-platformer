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
    void startPlayerWithDruidFirstDialogue();
    void startPlayerWithDruidSecondDialogue();
    bool hasPlayerWithRabbitLastDialogueFinished();
    bool hasPlayerWithDruidFirstDialogueFinished();

private:
    void handlePlayerWithRabbitFirstDialogue();
    void handlePlayerWithRabbitLastDialogue();
    void handlePlayerWithDruidFirstDialogue();
    void handlePlayerWithDruidSecondDialogue();
    void finishPlayerWithRabbitFirstDialogue();
    void finishPlayerWithRabbitLastDialogue();
    void finishPlayerWithDruidFirstDialogue();
    void finishPlayerWithDruidSecondDialogue();

    Level1MainCharacters* mainCharacters;
    std::unique_ptr<DialoguesReader> dialoguesReader;
    DialogueTrack playerWithRabbitFirstDialogueTrack;
    DialogueTrack playerWithRabbitLastDialogueTrack;
    DialogueTrack playerWithDruidFirstDialogueTrack;
    DialogueTrack playerWithDruidSecondDialogueTrack;
    bool playerWithRabbitFirstDialogueIsOngoing{false};
    bool playerWithRabbitLastDialogueIsOngoing{false};
    bool playerWithDruidFirstDialogueIsOngoing{false};
    bool playerWithDruidSecondDialogueIsOngoing{false};
    std::unique_ptr<utils::Timer> dialogueAliveTimer;
    const float dialogueTimeToLive;
};
}
