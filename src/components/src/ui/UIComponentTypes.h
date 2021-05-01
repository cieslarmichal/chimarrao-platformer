#pragma once

namespace components::ui
{
enum class UIComponentType
{
    Background,
    Button,
    CheckBox,
    Label,
    TextField,
    Image
};

enum class UIComponentTypeWithLabel
{
    Button,
    Label
};

enum class UIComponentTypeWithCheck
{
    CheckBox
};
}