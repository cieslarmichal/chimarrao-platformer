#pragma once

namespace components::ui
{
enum class UIComponentType
{
    Background,
    Button,
    CheckBox,
    Label,
    TextField
};

enum class UIComponentTypeWithText
{
    Button,
    Label,
    TextField
};

enum class UIComponentTypeWithCheck
{
    CheckBox
};
}