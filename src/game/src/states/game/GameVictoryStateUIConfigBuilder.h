#pragma once

#include <memory>
#include <vector>

namespace components::ui
{
struct UIConfig;
struct BackgroundConfig;
struct ButtonConfig;
struct CheckBoxConfig;
struct LabelConfig;
struct TextFieldConfig;
struct ImageConfig;
}

namespace game
{
class GameVictoryState;

class GameVictoryStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createGameVictoryUIConfig(GameVictoryState*);

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(GameVictoryState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(GameVictoryState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
    createCheckBoxConfigs(GameVictoryState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(GameVictoryState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
    createTextFieldConfigs(GameVictoryState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(GameVictoryState*);

    static std::vector<std::string> iconNames;
};
}