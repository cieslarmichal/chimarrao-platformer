#include "RenderTargetSfml.h"

#include <boost/numeric/conversion/cast.hpp>

namespace graphics
{
namespace
{
sf::View getLetterboxView(sf::View view, unsigned windowWidth, unsigned windowHeight);
}

RenderTargetSfml::RenderTargetSfml(std::shared_ptr<window::Window> windowInit,
                                   const utils::Vector2u& windowSizeInit, const utils::Vector2u& areaSizeInit)
    : window{std::move(windowInit)}, windowSize{windowSizeInit}, areaSize{areaSizeInit}
{
}

void RenderTargetSfml::initialize()
{
    RenderTarget::initialize();
}

void RenderTargetSfml::clear(const Color& color)
{
    RenderTarget::clear(color);
}

void RenderTargetSfml::draw(const sf::Drawable& drawable)
{
    sf::RenderTarget::draw(drawable);
}

void RenderTargetSfml::setView()
{
    view.setSize(boost::numeric_cast<float>(areaSize.x), boost::numeric_cast<float>(areaSize.y));
    view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
    view = getLetterboxView(view, windowSize.x, windowSize.y);
    window->setView(view);

    sf::RenderTarget::setView(view);
}

void RenderTargetSfml::setViewSize(const utils::Vector2u& size)
{
    windowSize = size;
    initialize();
}

void RenderTargetSfml::synchronizeViewSize()
{
    initialize();
}

sf::Vector2u RenderTargetSfml::getSize() const
{
    return windowSize;
}

bool RenderTargetSfml::setActive(bool)
{
    return true;
}

namespace
{
sf::View getLetterboxView(sf::View view, unsigned windowWidth, unsigned windowHeight)
{
    // https://github.com/SFML/SFML/wiki/Source:-Letterbox-effect-using-a-view
    // Compares the aspect ratio of the window to the aspect ratio of the view,
    // and sets the view's viewport accordingly in order to archieve a letterbox
    // effect. A new view (with a new viewport set) is returned.

    float windowRatio = boost::numeric_cast<float>(windowWidth) / boost::numeric_cast<float>(windowHeight);
    float viewRatio = view.getSize().x / view.getSize().y;
    float sizeX = 1.f;
    float sizeY = 1.f;
    float posX = 0.f;
    float posY = 0.f;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
    {
        horizontalSpacing = false;
    }

    // If horizontalSpacing is true, the black bars will appear on the left and
    // right side. Otherwise, the black bars will appear on the top and bottom.

    if (horizontalSpacing)
    {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) * 0.5F;
    }

    else
    {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) * 0.5F;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

    return view;
}
}
}