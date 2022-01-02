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
class GameState;

class GameStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createGameUIConfig(GameState*);

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(GameState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(GameState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(GameState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(GameState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>> createTextFieldConfigs(GameState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(GameState*);
};
}