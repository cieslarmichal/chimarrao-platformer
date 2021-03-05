#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>

#include "ContextRenderer.h"
#include "Vector.h"
#include "Window.h"

namespace graphics
{

class RenderTargetSfml : public ContextRenderer, public sf::RenderTarget
{
public:
    RenderTargetSfml(std::shared_ptr<window::Window>, const utils::Vector2u& windowSize,
                     const utils::Vector2u& areaSize);

    void initialize() override;
    void clear(const Color&) override;
    void draw(const sf::Drawable&) override;
    void setView() override;
    void setViewSize(const utils::Vector2u& windowsSize) override;
    void synchronizeViewSize() override;
    sf::Vector2u getSize() const override;
    bool setActive(bool active) override;

private:
    std::shared_ptr<window::Window> window;
    utils::Vector2u windowSize;
    const utils::Vector2u areaSize;
    sf::View view;
};

}