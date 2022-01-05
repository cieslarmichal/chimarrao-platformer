#pragma once

#include "DefaultDialoguesReader.h"
#include "Level1MainCharacters.h"

namespace game
{
class Level1DialoguesController
{
public:
    Level1DialoguesController(Level1MainCharacters*, std::unique_ptr<DialoguesReader>);

    void startPlayerWithRabbitDialogue();
    void startPlayerWithDruidDialogue();

private:
    Level1MainCharacters* mainCharacters;
    std::unique_ptr<DialoguesReader> dialoguesReader;
    DialogueTrack playerWithRabbitDialogueTrack1;
    DialogueTrack playerWithDruidDialogueTrack;
};
}
