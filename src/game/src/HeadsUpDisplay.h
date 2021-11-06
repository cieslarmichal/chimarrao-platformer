#pragma once

#include "core/ComponentOwner.h"
#include "core/ItemInfo.h"
#include "ui/UIComponentFactory.h"

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
                            const std::shared_ptr<graphics::RendererPool>& rendererPool,
                            std::unique_ptr<components::ui::UIConfig>);

    void update(const utils::DeltaTime&, const input::Input&);
    void activate();
    void deactivate();

private:
    void createUIComponents(std::unique_ptr<components::ui::UIConfig>);

    std::shared_ptr<components::core::ComponentOwner> player;
    std::unique_ptr<components::ui::UIComponentFactory> uiComponentFactory;
    std::unordered_map<std::string, std::unique_ptr<components::ui::Label>> labels;
    std::unordered_map<std::string, std::unique_ptr<components::ui::Image>> images;
    const std::string healthBarId;
    bool active;
    const std::vector<std::string> slotIds;
    std::vector<components::core::ItemInfo> displayedItemsInfo;
};
}