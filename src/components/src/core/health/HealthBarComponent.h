#pragma once

#include "Component.h"
#include "Vector.h"

namespace graphics
{
class RendererPool;
}

namespace components::core
{
class ComponentOwner;
class GraphicsComponent;
class HealthComponent;

class HealthBarComponent : public Component
{
public:
    HealthBarComponent(ComponentOwner*, const std::shared_ptr<graphics::RendererPool>&,
                       const utils::Vector2f& offset = utils::Vector2f{0, 0});

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;
    void lateUpdate(utils::DeltaTime, const input::Input& input) override;

protected:
    std::shared_ptr<GraphicsComponent> bar;
    std::shared_ptr<HealthComponent> health;
    utils::Vector2f maximumSize{2.5f, 0.4f};
};
}