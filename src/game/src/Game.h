#pragma once

#include "Timer.h"

namespace game
{
class Game
{
private:
    Timer timer;

    void processInput();

    void update(DeltaTime dt);

    void render();

public:
    void run();
};
}