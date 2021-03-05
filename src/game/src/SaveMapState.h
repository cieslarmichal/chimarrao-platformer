#pragma once

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "core/ComponentOwner.h"

namespace game
{
class SaveMapState : public State, public input::InputObserver
{
public:
    explicit SaveMapState(const std::shared_ptr<window::Window>&, const std::shared_ptr<input::InputManager>&,
                          const std::shared_ptr<graphics::RendererPool>&,
                          std::stack<std::unique_ptr<State>>&);
    ~SaveMapState();

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;

private:
    void saveMap();
    void unfreezeButtons();
    void backToEditorMenu();
    void createMapPropertiesTitle();
    void createBackground();
    void createCancelButton();
    void createSaveButton();
    void createMapNamingSection();
    void addMapNameInputField();
    void addButton(const utils::Vector2f& position, const utils::Vector2f& size, const std::string& text,
                   unsigned int fontSize, const utils::Vector2f& textOffset,
                   std::function<void(void)> clickAction);
    void addText(const utils::Vector2f& position, const std::string& description, unsigned int fontSize,
                 graphics::Color);

    const input::InputStatus* inputStatus;
    utils::Timer possibleLeaveFromStateTimer;
    const float timeAfterLeaveStateIsPossible;
    bool shouldBackToEditorMenu;
    std::vector<std::unique_ptr<components::core::ComponentOwner>> texts;
    std::unique_ptr<components::core::ComponentOwner> background;
    std::vector<std::unique_ptr<components::core::ComponentOwner>> buttons;
    std::unique_ptr<components::core::ComponentOwner> mapNameInputTextField;
    bool buttonsActionsFrozen = true;
    utils::Timer freezeClickableButtonsTimer;
    const float timeAfterButtonsCanBeClicked;
    std::string mapNameBuffer;
    bool mapNameFieldClicked;
    unsigned int mapNameMaximumSize;
    utils::Timer inputMapNameDeleteCharactersTimer;
    const float timeAfterNextLetterCanBeDeleted;
};
}