#pragma once

#include <type_traits>
#include <vector>

#include "Component.h"
#include "DeltaTime.h"
#include "GraphicsComponent.h"
#include "IdComponent.h"
#include "Input.h"
#include "SharedContext.h"
#include "TransformComponent.h"
#include "UniqueName.h"

namespace components::core
{
class ComponentOwner
{
public:
    ComponentOwner(const utils::Vector2f& position, const std::string& uniqueNameInit,
                   std::shared_ptr<SharedContext>);

    void loadDependentComponents();
    void update(utils::DeltaTime, const input::Input&);
    void lateUpdate(utils::DeltaTime, const input::Input& input);
    void enable();
    void disable();
    std::string getName() const;
    unsigned int getId() const;
    bool areComponentsEnabled() const;
    void remove();
    bool shouldBeRemoved() const;

    template <typename T, typename... Args>
    std::shared_ptr<T> addComponent(Args... args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        static_assert(!std::is_same<GraphicsComponent, T>::value, "T cannot be graphics");

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

    template <typename... Args>
    std::shared_ptr<GraphicsComponent> addGraphicsComponent(Args... args)
    {
        std::shared_ptr<GraphicsComponent> newGraphicsComponent =
            std::make_shared<GraphicsComponent>(this, args...);
        allGraphics.push_back(newGraphicsComponent);
        return newGraphicsComponent;
    }

    template <typename T>
    std::shared_ptr<T> getComponent() const
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        static_assert(!std::is_same<GraphicsComponent, T>::value, "T cannot be graphics");

        for (auto& existingComponent : components)
        {
            if (std::dynamic_pointer_cast<T>(existingComponent))
            {
                return std::dynamic_pointer_cast<T>(existingComponent);
            }
        }

        return nullptr;
    }

    std::shared_ptr<GraphicsComponent> getMainGraphicsComponent() const
    {
        if (not allGraphics.empty())
        {
            return allGraphics.at(0);
        }
        return nullptr;
    }

    std::shared_ptr<TransformComponent> transform;
    std::vector<std::shared_ptr<GraphicsComponent>> allGraphics;
    std::shared_ptr<IdComponent> id;
    std::shared_ptr<SharedContext> sharedContext;

protected:
    std::vector<std::shared_ptr<Component>> components;

private:
    utils::UniqueName uniqueName;
    bool toRemove{false};
};
}