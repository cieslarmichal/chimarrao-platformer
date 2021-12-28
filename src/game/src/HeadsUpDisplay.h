#pragma once

#include "ComponentOwner.h"
#include "ItemInfo.h"
#include "UIComponentFactory.h"

namespace components::ui
{
class Label;
class Image;
class UIConfig;
}

namespace game
{
class HeadsUpDisplay
{
public:
    explicit HeadsUpDisplay(std::shared_ptr<components::core::ComponentOwner> player,
                            const std::shared_ptr<components::core::SharedContext>&,
                            std::unique_ptr<components::ui::UIConfig>, std::unique_ptr<utils::Timer>);

    void update(const utils::DeltaTime&, const input::Input&);
    void activate();
    void deactivate();

private:
    void changeSelectedItem(const std::vector<components::core::ItemInfo>&);
    void useSelectedItem(const std::vector<components::core::ItemInfo>&);
    void dropSelectedItem(const std::vector<components::core::ItemInfo>&);
    void createUIComponents(std::unique_ptr<components::ui::UIConfig>);

    std::shared_ptr<components::core::ComponentOwner> player;
    std::unique_ptr<components::ui::UIComponentFactory> uiComponentFactory;
    std::unordered_map<std::string, std::unique_ptr<components::ui::Label>> labels;
    std::unordered_map<std::string, std::unique_ptr<components::ui::Image>> images;
    const std::string healthBarId;
    bool active;
    const std::vector<std::string> slotIds;
    const std::vector<std::string> slotItemIds;
    std::vector<components::core::ItemInfo> displayedItemsInfo;
    const float timeAfterSelectedItemCanBeChanged;
    std::unique_ptr<utils::Timer> changeSelectedItemTimer;
    int selectedItemIndex = 0;
    int previouslySelectedItemIndex = 0;
};
}