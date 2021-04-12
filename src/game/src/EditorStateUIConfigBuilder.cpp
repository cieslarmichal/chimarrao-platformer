#include "EditorStateUIConfigBuilder.h"

#include "EditorState.h"
#include "GetProjectPath.h"
#include "SaveMapState.h"
#include "Vector.h"
#include "ui/UIConfig.h"

namespace game
{
namespace
{
const float rendererPoolSizeX = 80;
const float rendererPoolSizeY = 60;

const auto pathToBackground =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/background_glacial_mountains.png";
const auto pathToBrickTileTexture =
    utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png";
const auto tilesTextureVector =
    std::vector<std::string>{utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png",
                             utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/2.png"};
}

std::unique_ptr<components::ui::UIConfig>
EditorStateUIConfigBuilder::createEditorUIConfig(EditorState* editorState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> checkBoxesConfig;
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;
    std::vector<std::unique_ptr<components::ui::TextFieldConfig>> textFieldsConfig;

    const auto changeBlockAction = [=]()
    {
        editorState->currentTileId =
            editorState->currentTileId + 1 < tilesTextureVector.size() ? editorState->currentTileId + 1 : 0;
        editorState->currentTilePath = tilesTextureVector[editorState->currentTileId];
    };
    const auto keyActions = std::vector<components::core::KeyAction>{
        components::core::KeyAction{input::InputKey::MouseRight, changeBlockAction}};

    auto backgroundConfig = std::make_unique<components::ui::BackgroundConfig>(
        "editorBackground", utils::Vector2f{0, 0}, utils::Vector2f{rendererPoolSizeX, rendererPoolSizeY},
        graphics::VisibilityLayer::Background, pathToBackground, keyActions);

    return std::make_unique<components::ui::UIConfig>(std::move(backgroundConfig), std::move(buttonsConfig),
                                                      std::move(checkBoxesConfig), std::move(labelsConfig),
                                                      std::move(textFieldsConfig));
}
}