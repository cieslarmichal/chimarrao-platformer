#include "SettingsState.h"

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
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto buttonSize = utils::Vector2f{25, 5};
const auto sectionTextFontSize{37};
const auto displayModeFontSize{12};
const auto displayModeButtonSize = utils::Vector2f{8, 3};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
const auto backgroundPath =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/menu_background.jpg";

const auto settingsTitlePosition = utils::Vector2f{32, 6};
const auto displayModeSectionPosition = utils::Vector2f{38, 18};
const auto windowModeButtonPosition = utils::Vector2f{
    displayModeSectionPosition.x + 15.8f + displayModeButtonSize.x, displayModeSectionPosition.y + 0.5f};
const auto fullscreenModeButtonPosition =
    utils::Vector2f{windowModeButtonPosition.x + displayModeButtonSize.x + 0.5f, windowModeButtonPosition.y};
const auto resolutionSectionPosition = utils::Vector2f{38, 24};
const auto resolutionTextFieldPosition = utils::Vector2f{
    resolutionSectionPosition.x + 15.8f + displayModeButtonSize.x, resolutionSectionPosition.y + 0.8f};
const auto changeResolutionButtonSize = utils::Vector2f{2.5, 2};
const auto resolutionDecreaseButtonPosition =
    utils::Vector2f{resolutionTextFieldPosition.x - 3, resolutionSectionPosition.y + 1.15f};
const auto resolutionIncreaseButtonPosition =
    utils::Vector2f{resolutionTextFieldPosition.x + 11.2f, resolutionSectionPosition.y + 1.15f};
const auto vsyncSectionPosition = utils::Vector2f{38, 30};
const auto frameLimitSectionPosition = utils::Vector2f{38, 36};
const auto backToMenuButtonPosition = utils::Vector2f{34.5, 48};
}

SettingsState::SettingsState(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<input::InputManager>& inputManagerInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      shouldBackToMenu{false},
      inputStatus{nullptr}
{
    inputManager->registerObserver(this);

    createBackground();
    createSettingsTitle();
    createBackToGameButton();
    createDisplayModeSection();
    createResolutionSection();
    createVsyncSection();
    createFrameLimitSection();

    initialize();
}

SettingsState::~SettingsState()
{
    inputManager->removeObserver(this);
}

void SettingsState::initialize()
{
    for (auto& button : buttons)
    {
        button->loadDependentComponents();
        button->start();
    }
}

void SettingsState::update(const utils::DeltaTime& deltaTime)
{
    if (shouldBackToMenu)
    {
        backToMenu();
        return;
    }

    for (auto& button : buttons)
    {
        button->update(deltaTime);
    }
}

void SettingsState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    for (auto& button : buttons)
    {
        button->lateUpdate(deltaTime);
    }
}

void SettingsState::render()
{
    rendererPool->renderAll();
}

std::string SettingsState::getName() const
{
    return "Settings state";
}

void SettingsState::activate()
{
    active = true;

    for (auto& button : buttons)
    {
        button->enable();
    }
}

void SettingsState::deactivate()
{
    active = false;

    for (auto& button : buttons)
    {
        button->disable();
    }
}

void SettingsState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void SettingsState::backToMenu()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void SettingsState::createBackground()
{
    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(rendererPool, utils::Vector2f{80, 60},
                                                            utils::Vector2f{0, 0}, backgroundPath,
                                                            graphics::VisibilityLayer::Background);
}

void SettingsState::createSettingsTitle()
{
    addText(settingsTitlePosition, "Settings", 37);
}

void SettingsState::createBackToGameButton()
{
    const auto backToGameButtonSize = utils::Vector2f{13, 5};
    addButton(backToMenuButtonPosition, backToGameButtonSize, "Done", sectionTextFontSize,
              utils::Vector2f{2, 0}, [this] { shouldBackToMenu = true; });
}

void SettingsState::createDisplayModeSection()
{
    addText(displayModeSectionPosition, "Display mode:", 30);
    addButton(windowModeButtonPosition, displayModeButtonSize, "Window", displayModeFontSize,
              utils::Vector2f{2, 0.9}, [this] { std::cerr << "window mode"; });
    addButton(fullscreenModeButtonPosition, displayModeButtonSize, "Fullscreen", displayModeFontSize,
              utils::Vector2f{0.5, 0.9}, [this] { std::cerr << "fullscreen mode"; });
}

void SettingsState::createResolutionSection()
{
    addText(resolutionSectionPosition, "Resolution:", 30);
    addText(resolutionTextFieldPosition, "1920x1080", 20);
    addButton(resolutionDecreaseButtonPosition, changeResolutionButtonSize, "<", 20, utils::Vector2f{0.6, -0.3},
              [this] { std::cerr << "decrease resolution"; });
    addButton(resolutionIncreaseButtonPosition, changeResolutionButtonSize, ">", 20, utils::Vector2f{0.6, -0.3},
              [this] { std::cerr << "increase resolution"; });
}

void SettingsState::createVsyncSection()
{
    addText(vsyncSectionPosition, "Vsync:", 30);
}

void SettingsState::createFrameLimitSection()
{
    addText(frameLimitSectionPosition, "Frame limit:", 30);
}

void SettingsState::addButton(const utils::Vector2f& position, const utils::Vector2f& size,
                              const std::string& text, unsigned int fontSize,
                              const utils::Vector2f& textOffset, std::function<void(void)> clickAction)
{
    auto button = std::make_unique<components::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::GraphicsComponent>(
        rendererPool, size, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::TextComponent>(rendererPool, position, text, fontPath, fontSize,
                                                    graphics::Color::Black, textOffset);
    button->addComponent<components::HitboxComponent>(size);
    button->addComponent<components::ClickableComponent>(inputManager, std::move(clickAction));

    const auto changeColorOnMouseOver = [=] { graphicsComponent->setColor(buttonHoverColor); };
    const auto changeColorOnMouseOut = [=] { graphicsComponent->setColor(buttonColor); };
    button->addComponent<components::MouseOverComponent>(inputManager, changeColorOnMouseOver,
                                                         changeColorOnMouseOut);
    buttons.push_back(std::move(button));
}

void SettingsState::addText(const utils::Vector2f& position, const std::string& description,
                            unsigned int fontSize)
{
    auto text = std::make_unique<components::ComponentOwner>(position);
    text->addComponent<components::TextComponent>(rendererPool, position, description, fontPath, fontSize,
                                                  graphics::Color::Black);
    texts.push_back(std::move(text));
}

}