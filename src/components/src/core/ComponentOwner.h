#pragma once

#include <type_traits>
#include <vector>

#include "Component.h"
#include "DeltaTime.h"
#include "TransformComponent.h"

namespace components::core
{
class ComponentOwner
{
public:
    ComponentOwner(const utils::Vector2f& position);

    void loadDependentComponents();
    void start();
    void update(utils::DeltaTime);
    void lateUpdate(utils::DeltaTime);
    void enable();
    void disable();

    template <typename T, typename... Args>
    std::shared_ptr<T> addComponent(Args... args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        for (auto& existingComponent : components)
        {
            if (std::dynamic_pointer_cast<T>(existingComponent))
            {
                return std::dynamic_pointer_cast<T>(existingComponent);
            }
        }

        std::shared_ptr<T> newComponent = std::make_shared<T>(this, args...);
        components.push_back(newComponent);

        return newComponent;
    }

    template <typename T>
    std::shared_ptr<T> getComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        for (auto& existingComponent : components)
        {
            if (std::dynamic_pointer_cast<T>(existingComponent))
            {
                return std::dynamic_pointer_cast<T>(existingComponent);
            }
        }

        return nullptr;
    }

    std::shared_ptr<TransformComponent> transform;

protected:
    std::vector<std::shared_ptr<Component>> components;
};
}