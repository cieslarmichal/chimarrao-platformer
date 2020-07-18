#include "Game.h"

namespace game {
void Game::run() {
    while (true)
    {
        processInput();
        update(timer.getDurationFromLastUpdate());
        render();
    }
}

void Game::processInput() {

}

void Game::update(DeltaTime dt) {

}

void Game::render() {

}
}