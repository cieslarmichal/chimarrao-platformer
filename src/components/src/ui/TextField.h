#pragma once

#include <memory>

#include "InputManager.h"
#include "RendererPool.h"
#include "TextFieldConfig.h"
#include "Timer.h"
#include "UIComponent.h"
#include "core/ComponentOwner.h"

namespace components::ui
{
class TextField : public UIComponent
{
public:
    TextField(const std::shared_ptr<graphics::RendererPool>&, std::unique_ptr<TextFieldConfig>);

    void update(utils::DeltaTime, const input::Input&) override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void setColor(graphics::Color);
    bool isActive() const override;
    void setText(const std::string&);

private:
    std::string name;
    std::unique_ptr<components::core::ComponentOwner> coreComponentsOwner;
    std::string inputBuffer;
    bool textFieldClicked;
    const unsigned int inputBufferMaximumSize;
    utils::Timer deleteCharactersTimer;
    const float timeAfterNextLetterCanBeDeleted;
    std::function<void()> clickInsideFieldAction;
    std::function<void(std::string)> clickOutsideFieldAction;
};
}