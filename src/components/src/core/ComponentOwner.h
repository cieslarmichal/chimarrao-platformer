#pragma once

#include <type_traits>
#include <vector>

#include "Component.h"
#include "DeltaTime.h"
#include "Input.h"
#include "TransformComponent.h"
#include "UniqueName.h"
#include "IdComponent.h"

namespace components::core
{
class ComponentOwner
{
public:
    ComponentOwner(const utils::Vector2f& position, const std::string& uniqueNameInit);

    void loadDependentComponents();
    void update(utils::DeltaTime, const input::Input&);
    void lateUpdate(utils::DeltaTime);
    void enable();
    void disable();
    std::string getName() const;
    unsigned int getId() const;
    bool areComponentEnabled() const;

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
    std::shared_ptr<IdComponent> id;

protected:
    std::vector<std::shared_ptr<Component>> components;

private:
    utils::UniqueName uniqueName;
};
}