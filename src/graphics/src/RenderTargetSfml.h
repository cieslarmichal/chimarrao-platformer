#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>

#include "ContextRenderer.h"
#include "Vector.h"

namespace graphics
{

class RenderTargetSfml : public ContextRenderer, public sf::RenderTarget
{
public:
    RenderTargetSfml(const utils::Vector2u& windowSize, const utils::Vector2u& areaSize);

    void initialize() override;
    void clear(const sf::Color&) override;
    void draw(const RectangleShape&) override;
    void setView() override;
    void setViewSize(const utils::Vector2u& windowsSize) override;
    sf::Vector2u getSize() const override;
    bool activate(bool active) override;

private:
    utils::Vector2u windowSize;
    const utils::Vector2u areaSize;
    sf::View view;
};

}