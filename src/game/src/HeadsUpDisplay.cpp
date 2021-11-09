#include "HeadsUpDisplay.h"

#include <utility>

#include "HeadsUpDisplayUIConfigBuilder.h"
#include "core/HealthComponent.h"
#include "core/ItemCollectorComponent.h"
#include "core/exceptions/DependentComponentNotFound.h"
#include "ui/Image.h"
#include "ui/UIComponentFactory.h"
#include "ui/UIConfig.h"

namespace game
{

HeadsUpDisplay::HeadsUpDisplay(std::shared_ptr<components::core::ComponentOwner> playerInit,
                               const std::shared_ptr<components::core::SharedContext>& sharedContext,
                               std::unique_ptr<components::ui::UIConfig> uiConfig)
    : player{std::move(playerInit)},
      uiComponentFactory{std::make_unique<components::ui::UIComponentFactory>(sharedContext)},
      healthBarId{HeadsUpDisplayUIConfigBuilder::getHealthBarId()},
      active{false},
      slotIds{HeadsUpDisplayUIConfigBuilder::getSlotIds()},
      slotItemIds{HeadsUpDisplayUIConfigBuilder::getSlotItemIds()}
{
    const auto healthComponent = player->getComponent<components::core::HealthComponent>();
    if (not healthComponent)
    {
        throw components::core::exceptions::DependentComponentNotFound{"HUD: health component not found"};
    }

    const auto collectorComponent = player->getComponent<components::core::ItemCollectorComponent>();
    if (not collectorComponent)
    {
        throw components::core::exceptions::DependentComponentNotFound{
            "HUD: item collector component not found"};
    }

    createUIComponents(std::move(uiConfig));
    auto& healthBarFrame = images[HeadsUpDisplayUIConfigBuilder::getHealthBarFrameId()];
    healthBarFrame->setOutline(0.15, graphics::Color::Black);

    for (const auto& slotId : slotIds)
    {
        auto& slot = images[slotId];
        slot->setOutline(0.1, graphics::Color::Black);
    }

    for (const auto& slotItemId : slotItemIds)
    {
        auto& slotItem = images[slotItemId];
        slotItem->deactivate();
    }
}

void HeadsUpDisplay::update(const utils::DeltaTime&, const input::Input&)
{
    auto healthComponent = player->getComponent<components::core::HealthComponent>();
    auto& healthBar = images[healthBarId];
    const auto currentBarSize = healthBar->getSize();
    const auto currentHealth = healthComponent->getCurrentHealth();
    const auto maximumHealth = healthComponent->getMaximumHealth();
    const auto barSizeScaledByCurrentHealth = utils::Vector2f{
        currentBarSize.x * (static_cast<float>(currentHealth) / static_cast<float>(maximumHealth)),
        currentBarSize.y};
    if (barSizeScaledByCurrentHealth != currentBarSize)
    {
        healthBar->setSize(barSizeScaledByCurrentHealth);
    }

    const auto collectorComponent = player->getComponent<components::core::ItemCollectorComponent>();
    const auto itemsInfo = collectorComponent->getItemsInfo();
    if (displayedItemsInfo == itemsInfo)
    {
        return;
    }

    for (std::size_t slotItemIndex = 0; slotItemIndex < slotItemIds.size(); slotItemIndex++)
    {
        const auto& slotItemId = slotItemIds[slotItemIndex];
        auto& slot = images[slotItemId];

        if (slotItemIndex < itemsInfo.size())
        {
            slot->activate();
            slot->setTexture(itemsInfo[slotItemIndex].texturePath);
        }
        else
        {
            slot->deactivate();
        }
    }

    displayedItemsInfo = itemsInfo;
}

void HeadsUpDisplay::activate()
{
    active = true;
}

void HeadsUpDisplay::deactivate()
{
    active = false;
}

void HeadsUpDisplay::createUIComponents(std::unique_ptr<components::ui::UIConfig> uiConfig)
{
    for (auto& labelConfig : uiConfig->labelsConfig)
    {
        auto label = uiComponentFactory->createLabel(std::move(labelConfig));
        const auto labelName = label->getName();
        labels[labelName] = std::move(label);
    }

    for (auto& imageConfig : uiConfig->imagesConfig)
    {
        auto image = uiComponentFactory->createImage(std::move(imageConfig));
        const auto imageName = image->getName();
        images[imageName] = std::move(image);
    }
}

}