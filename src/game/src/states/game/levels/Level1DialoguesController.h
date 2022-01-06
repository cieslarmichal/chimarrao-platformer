#pragma once

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
    void startPlayerWithRabbitDialogue();
    void startPlayerWithDruidDialogue();

private:
    Level1MainCharacters* mainCharacters;
    std::unique_ptr<DialoguesReader> dialoguesReader;
    DialogueTrack playerWithRabbitDialogueTrack1;
    DialogueTrack playerWithDruidDialogueTrack;
    bool playerWithRabbitDialogue1IsOngoing{false};
    bool playerWithDruidDialogueIsOngoing{false};
    std::unique_ptr<utils::Timer> dialogueAliveTimer;
    const float dialogueTimeToLive;
};
}
