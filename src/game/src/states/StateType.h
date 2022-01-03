#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

#include "StringHelper.h"
#include "exceptions/StateTypeNotFound.h"

namespace game
{
enum class StateType
{
    Controls,
    EditorMenu,
    Editor,
    CustomGame,
    Menu,
    Pause,
    SaveMap,
    Settings,
    ChooseMap,
    StoryGame
};

inline std::string toString(StateType stateType)
{
    std::unordered_map<StateType, std::string> stateTypeToString{
        {StateType::Controls, "Controls"},   {StateType::EditorMenu, "EditorMenu"},
        {StateType::Editor, "Editor"},       {StateType::CustomGame, "Game"},
        {StateType::Menu, "Menu"},           {StateType::Pause, "Pause"},
        {StateType::SaveMap, "SaveMap"},     {StateType::Settings, "Settings"},
        {StateType::ChooseMap, "ChooseMap"}, {StateType::StoryGame, "StoryGame"}};

    try
    {
        return stateTypeToString.at(stateType);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::StateTypeNotFound{e.what()};
    }
}

inline StateType toStateType(const std::string& stateTypeAsString)
{
    const auto stateTypeLowerString = utils::StringHelper::getLowerCases(stateTypeAsString);

    std::unordered_map<std::string, StateType> stringToStateType{
        {"controls", StateType::Controls},   {"editormenu", StateType::EditorMenu},
        {"editor", StateType::Editor},       {"customgame", StateType::CustomGame},
        {"menu", StateType::Menu},           {"pause", StateType::Pause},
        {"savemap", StateType::SaveMap},     {"settings", StateType::Settings},
        {"choosemap", StateType::ChooseMap}, {"storygame", StateType::StoryGame}};

    try
    {
        return stringToStateType.at(stateTypeLowerString);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::StateTypeNotFound{e.what()};
    }
}

inline std::ostream& operator<<(std::ostream& os, StateType stateType)
{
    return os << "StateType::" << toString(stateType);
}
}