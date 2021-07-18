#include "HeadsUpDisplay.h"

#include <utility>

#include "HeadsUpDisplayUIConfigBuilder.h"
#include "core/HealthComponent.h"
#include "ui/Image.h"
#include "ui/Label.h"
#include "ui/UIComponentFactory.h"
#include "ui/UIConfig.h"

namespace game
{

HeadsUpDisplay::HeadsUpDisplay(std::shared_ptr<components::core::ComponentOwner> player,
                               const std::shared_ptr<graphics::RendererPool>& rendererPool,
                               std::unique_ptr<components::ui::UIConfig> uiConfig)
    : player{std::move(player)},
      uiComponentFactory{std::make_unique<components::ui::UIComponentFactory>(rendererPool)},
      healthBarId{HeadsUpDisplayUIConfigBuilder::getHealthBarId()},
      active{false}
{
    createUIComponents(std::move(uiConfig));
}

void HeadsUpDisplay::update()
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