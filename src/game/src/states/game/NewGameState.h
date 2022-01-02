#pragma once

#include "ComponentOwner.h"
#include "MapsReader.h"
#include "State.h"
#include "Timer.h"
#include "UIConfig.h"
#include "UIManager.h"
#include "TileMap.h"

namespace game
{
class NewGameState : public State
{
public:
    NewGameState(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
                   std::shared_ptr<utils::FileAccess>, States&, const std::shared_ptr<TileMap>&);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;
};
}