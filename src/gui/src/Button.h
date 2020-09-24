#pragma once

#include <SFML/Graphics/Font.hpp>

#include "ComponentOwner.h"
#include "GraphicsComponent.h"
#include "InputObserver.h"
#include "InputManager.h"
#include "Vector.h"

namespace gui
{
class Button : public input::InputObserver, public components::ComponentOwner
{
public:
    Button(std::shared_ptr<graphics::RendererPool>, std::shared_ptr<input::InputManager>, const utils::Vector2f& size, const utils::Vector2f& position);
    ~Button();

    void handleInputStatus(const input::InputStatus&) override;

private:
    std::shared_ptr<input::InputManager> inputManager;
    std::shared_ptr<components::GraphicsComponent> graphics;
};
}