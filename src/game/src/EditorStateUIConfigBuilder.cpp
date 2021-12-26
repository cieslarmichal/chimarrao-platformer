#include "EditorStateUIConfigBuilder.h"

#include "CommonUIConfigElements.h"
#include "EditorState.h"
#include "ProjectPathReader.h"
#include "Vector.h"
#include "editor/TileType.h"

namespace game
{
namespace
{
const float rendererPoolSizeX = 80;
const float rendererPoolSizeY = 60;

const auto tilesTextureVector =
    std::vector<std::string>{utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/brick.png",
                             utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/2.png"};
}

std::unique_ptr<components::ui::UIConfig>
EditorStateUIConfigBuilder::createEditorUIConfig(EditorState* editorState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(editorState), createButtonConfigs(editorState),
        createCheckBoxConfigs(editorState), createLabelConfigs(editorState),
        createTextFieldConfigs(editorState), createImageConfigs(editorState));
}

std::unique_ptr<components::ui::BackgroundConfig>
EditorStateUIConfigBuilder::createBackgroundConfig(EditorState*)
{
    return std::make_unique<components::ui::BackgroundConfig>(
        "editorBackground", utils::Vector2f{0, 0}, utils::Vector2f{rendererPoolSizeX, rendererPoolSizeY},
        graphics::VisibilityLayer::Background, gameBackgroundPath);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
EditorStateUIConfigBuilder::createButtonConfigs(EditorState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
EditorStateUIConfigBuilder::createCheckBoxConfigs(EditorState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>>
EditorStateUIConfigBuilder::createLabelConfigs(EditorState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
EditorStateUIConfigBuilder::createTextFieldConfigs(EditorState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::ImageConfig>>
EditorStateUIConfigBuilder::createImageConfigs(EditorState*)
{
    return {};
}
}