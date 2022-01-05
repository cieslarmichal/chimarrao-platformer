#pragma once

#include "gmock/gmock.h"

#include "DialoguesReader.h"

namespace game
{
class DialoguesReaderMock : public DialoguesReader
{
public:
    MOCK_METHOD(DialogueTrack, read, (const std::string& dialogueTrackPath), (const override));
};
}