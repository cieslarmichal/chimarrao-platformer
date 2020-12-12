#include "SaveMapState.h"

#include "ClickableComponent.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "HitboxComponent.h"
#include "MouseOverComponent.h"
#include "TextComponent.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
const auto textColor = graphics::Color(200, 200, 200);
const auto smallButtonSize = utils::Vector2f{8, 3.5};
const auto buttonSize = utils::Vector2f{20, 3.5};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
const auto mapNamingTextDescriptionPosition = utils::Vector2f{24, 21};
const auto mapNamingPromptPosition =
    utils::Vector2f{mapNamingTextDescriptionPosition.x + 12.0f, mapNamingTextDescriptionPosition.y};
const auto changeResolutionButtonSize = utils::Vector2f{2.5, 2};
}

SaveMapState::SaveMapState(const std::shared_ptr<window::Window>& windowInit,
                           const std::shared_ptr<input::InputManager>& inputManagerInit,
                           const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                           std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditorMenu{false},
      timeAfterButtonsCanBeClicked{0.3f}
{
    inputManager->registerObserver(this);

    createBackground();
    createMapPropertiesTitle();
    createMapNamingSection();
    createCancelButton();
    createSaveButton();

    initialize();
}

SaveMapState::~SaveMapState()
{
    inputManager->removeObserver(this);
}

void SaveMapState::initialize()
{
    for (auto& button : buttons)
    {
        button->loadDependentComponents();
        button->start();
        if (auto clickable = button->getComponent<components::ClickableComponent>())
        {
            clickable->disable();
        }
    }

    possibleLeaveFromStateTimer.start();
}

void SaveMapState::update(const utils::DeltaTime& deltaTime)
{
    if (buttonsActionsFrozen &&
        freezeClickableButtonsTimer.getElapsedSeconds() > timeAfterButtonsCanBeClicked)
    {
        unfreezeButtons();
    }

    if (possibleLeaveFromStateTimer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        (inputStatus->isKeyPressed(input::InputKey::Escape) || shouldBackToEditorMenu))
    {
        backToEditorMenu();
        return;
    }

    for (auto& button : buttons)
    {
        button->update(deltaTime);
    }
}

void SaveMapState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    for (auto& button : buttons)
    {
        button->lateUpdate(deltaTime);
    }
}

void SaveMapState::render()
{
    rendererPool->renderAll();
}

std::string SaveMapState::getName() const
{
    return "Save map state";
}

void SaveMapState::activate()
{
    active = true;

    possibleLeaveFromStateTimer.restart();
}

void SaveMapState::deactivate()
{
    active = false;
}

void SaveMapState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void SaveMapState::saveMap()
{
    // TODO: implement
    shouldBackToEditorMenu = true;
}

void SaveMapState::unfreezeButtons()
{
    buttonsActionsFrozen = false;
    for (auto& button : buttons)
    {
        button->getComponent<components::ClickableComponent>()->enable();
    }
}

void SaveMapState::backToEditorMenu()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void SaveMapState::createMapPropertiesTitle()
{
    const auto titlePosition = utils::Vector2f{30, 11};
    addText(titlePosition, "Save map", 40, graphics::Color::White);
}

void SaveMapState::createBackground()
{
    const auto backgroundColor = graphics::Color{172};
    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(rendererPool, utils::Vector2f{40, 37},
                                                            utils::Vector2f{20, 8}, backgroundColor,
                                                            graphics::VisibilityLayer::Second);
}

void SaveMapState::createCancelButton()
{
    const auto cancelButtonPosition = utils::Vector2f{29, 37};

    addButton(cancelButtonPosition, smallButtonSize, "Cancel", 20, utils::Vector2f{0.5, 0.25},
              [this] { shouldBackToEditorMenu = true; });
}

void SaveMapState::createSaveButton()
{
    const auto saveButtonPosition = utils::Vector2f{43, 37};

    addButton(saveButtonPosition, smallButtonSize, "Save", 20, utils::Vector2f{1.25, 0.25},
              [this] { saveMap(); });
}

void SaveMapState::createMapNamingSection()
{
    addText(mapNamingTextDescriptionPosition, "Map name:", 20, graphics::Color::White);
    addButton(mapNamingPromptPosition, buttonSize, "", 20, utils::Vector2f{7, 0.75},
              [this] { std::cout << "insert map name"; });
}

void SaveMapState::addButton(const utils::Vector2f& position, const utils::Vector2f& size,
                             const std::string& text, unsigned int fontSize,
                             const utils::Vector2f& textOffset, std::function<void(void)> clickAction)
{
    auto button = std::make_unique<components::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::GraphicsComponent>(
        rendererPool, size, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::TextComponent>(rendererPool, position, text, fontPath, fontSize,
                                                    graphics::Color::White, textOffset);
    button->addComponent<components::HitboxComponent>(size);
    button->addComponent<components::ClickableComponent>(inputManager, std::move(clickAction));

    const auto changeColorOnMouseOver = [=] { graphicsComponent->setColor(buttonHoverColor); };
    const auto changeColorOnMouseOut = [=] { graphicsComponent->setColor(buttonColor); };
    button->addComponent<components::MouseOverComponent>(inputManager, changeColorOnMouseOver,
                                                         changeColorOnMouseOut);
    buttons.push_back(std::move(button));
}

void SaveMapState::addNonClickableButton(const utils::Vector2f& position, const utils::Vector2f& size,
                                         const std::string& text, unsigned int fontSize,
                                         const utils::Vector2f& textOffset)
{
    auto button = std::make_unique<components::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::GraphicsComponent>(
        rendererPool, size, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::TextComponent>(rendererPool, position, text, fontPath, fontSize,
                                                    graphics::Color::White, textOffset);
    buttons.push_back(std::move(button));
}

void SaveMapState::addText(const utils::Vector2f& position, const std::string& description,
                           unsigned int fontSize, graphics::Color color)
{
    auto text = std::make_unique<components::ComponentOwner>(position);
    text->addComponent<components::TextComponent>(rendererPool, position, description, fontPath, fontSize,
                                                  color);
    texts.push_back(std::move(text));
}

}