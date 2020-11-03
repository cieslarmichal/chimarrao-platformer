#pragma once

#include <vector>

#include "ClickableComponent.h"
#include "State.h"
#include "TileMap.h"

namespace game
{
class EditorState : public State
{
public:
    explicit EditorState(const std::shared_ptr<window::Window>&, const std::shared_ptr<input::InputManager>&,
                         const std::shared_ptr<graphics::RendererPool>&, std::stack<std::unique_ptr<State>>&);
    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;

private:
    std::unique_ptr<components::ComponentOwner> background;
    std::vector<std::shared_ptr<components::ComponentOwner>> clickableTileMap;
    std::unique_ptr<TileMap> tileMap;
};
}