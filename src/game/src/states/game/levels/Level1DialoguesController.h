#pragma once

#include "DefaultDialoguesReader.h"
#include "Level1MainCharacters.h"

namespace game
{
class Level1DialoguesController
{
public:
    Level1DialoguesController(Level1MainCharacters*);

    void startPlayerWithRabbitDialogue();
    void startPlayerWithDruidDialogue();

private:
    Level1MainCharacters* mainCharacters;
};
}
