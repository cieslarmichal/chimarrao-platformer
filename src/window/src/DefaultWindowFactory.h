#pragma once

#include <memory>

#include "WindowFactory.h"

namespace window
{
class DefaultWindowFactory : public WindowFactory
{
public:
    std::unique_ptr<Window> createWindow(const utils::Vector2u& windowSize,
                                         const std::string& title) const override;
};
}
