#include "InputKeySfmlMapper.h"

#include "gtest/gtest.h"

using namespace input;
using namespace ::testing;

class InputKeySfmlMapperTest : public Test
{
public:
    const InputKey keyboardKey1 = InputKey::A;
    const InputKey keyboardKey2 = InputKey::Enter;
    const InputKey keyboardKey3 = InputKey::Space;
    const InputKey mouseKey1 = InputKey::MouseLeft;
    const InputKey mouseKey2 = InputKey::MouseRight;
    const sf::Keyboard::Key keyboardKey1Mapped = sf::Keyboard::Key::A;
    const sf::Keyboard::Key keyboardKey2Mapped = sf::Keyboard::Key::Enter;
    const sf::Keyboard::Key keyboardKey3Mapped = sf::Keyboard::Key::Space;
    const sf::Mouse::Button mouseKey1Mapped = sf::Mouse::Button::Left;
    const sf::Mouse::Button mouseKey2Mapped = sf::Mouse::Button::Right;
};

TEST_F(InputKeySfmlMapperTest, givenInvalidKeyboardKey_shouldThrowInputKeyNotFound)
{
    ASSERT_THROW(InputKeySfmlMapper::getKeyboardSfmlKey(mouseKey1), exceptions::InputKeyNotFound);
}

TEST_F(InputKeySfmlMapperTest, givenValidKeyboardKey_shouldReturnSfmlEquivalent)
{
    ASSERT_EQ(InputKeySfmlMapper::getKeyboardSfmlKey(keyboardKey1), keyboardKey1Mapped);
    ASSERT_EQ(InputKeySfmlMapper::getKeyboardSfmlKey(keyboardKey2), keyboardKey2Mapped);
    ASSERT_EQ(InputKeySfmlMapper::getKeyboardSfmlKey(keyboardKey3), keyboardKey3Mapped);
}

TEST_F(InputKeySfmlMapperTest, givenInvalidMouseKey_shouldThrowInputKeyNotFound)
{
    ASSERT_THROW(InputKeySfmlMapper::getMouseSfmlKey(keyboardKey1), exceptions::InputKeyNotFound);
}

TEST_F(InputKeySfmlMapperTest, givenValidMouseKey_shouldReturnSfmlEquivalent)
{
    ASSERT_EQ(InputKeySfmlMapper::getMouseSfmlKey(mouseKey1), mouseKey1Mapped);
    ASSERT_EQ(InputKeySfmlMapper::getMouseSfmlKey(mouseKey2), mouseKey2Mapped);
}