#pragma once

#include <string>
#include <vector>

namespace game
{
class DialoguesValidator
{
public:
    bool validate(const std::vector<std::string>& dialogueLines) const;
};
}
