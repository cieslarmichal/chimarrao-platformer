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
class GameOverState;

class GameOverStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createGameOverUIConfig(GameOverState*);

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(GameOverState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(GameOverState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(GameOverState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(GameOverState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
    createTextFieldConfigs(GameOverState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(GameOverState*);

    static std::vector<std::string> iconNames;
};
}