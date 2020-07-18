#include "Game.h"

#include "Vector.h"
#include "WindowSfml.h"

namespace game {
Game::Game() {
    window = std::make_unique<graphics::WindowSFML>(utils::Vector2i(800, 600),"chimarrao");
    timer.start();
}

void Game::run() {
    while (window->isOpen())
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
    window->display();
}
}