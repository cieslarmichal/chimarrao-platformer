#pragma once

#include <vector>
#include "SFML/Graphics/Texture.hpp"
#include "Window.h"
#include "DeltaTime.h"
#include <memory>
#include "Entity.h"
#include "InputManager.h"

namespace game
{
class GameState
{
public:
    explicit GameState(std::shared_ptr<graphics::Window>, InputManager&);
    virtual ~GameState() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual void render(sf::RenderTarget*) = 0;
    virtual void checkIfEnded() = 0;

protected:
    InputManager& inputManager;

private:
    bool stateIsEnded;
    std::shared_ptr<graphics::Window> window;
    std::vector<sf::Texture> textures;
};
}