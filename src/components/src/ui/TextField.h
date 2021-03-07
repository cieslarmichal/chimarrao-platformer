#pragma once

#include <memory>

#include "InputManager.h"
#include "InputObserver.h"
#include "RendererPool.h"
#include "TextFieldConfig.h"
#include "Timer.h"
#include "UIComponent.h"
#include "core/ComponentOwner.h"

namespace components::ui
{
class TextField : public UIComponent, public input::InputObserver
{
public:
    TextField(const std::shared_ptr<input::InputManager>&, const std::shared_ptr<graphics::RendererPool>&,
              std::unique_ptr<TextFieldConfig>);
    ~TextField();

    void update(utils::DeltaTime) override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;
    void setColor(graphics::Color);

private:
    void setText(const std::string&);

    std::shared_ptr<input::InputManager> inputManager;
    const input::InputStatus* inputStatus;
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
    std::string inputBuffer;
    bool textFieldClicked;
    const unsigned int inputBufferMaximumSize;
    utils::Timer deleteCharactersTimer;
    const float timeAfterNextLetterCanBeDeleted;
    std::function<void(std::string)> clickOutsideFieldAction;
};
}