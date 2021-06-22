#include "Text.h"

#include "gtest/gtest.h"

#include "GraphicsIdGenerator.h"
#include "ProjectPathReader.h"

using namespace graphics;
using namespace ::testing;

namespace
{
const utils::Vector2f position{3.0f, 2.0f};
const auto color{Color::Magenta};
const auto textId = GraphicsIdGenerator::generateId();
const std::string textValue{"text"};
const std::string testDirectory{utils::ProjectPathReader::getProjectRootPath() +
                                "src/graphics/src/testResources/"};
const std::string existingFontPath{testDirectory + "VeraMono.ttf"};
const unsigned characterSize = 5;

}

class TextTest : public Test
{
public:
    TextTest()
    {
        font.loadFromFile(existingFontPath);
    }

    sf::Font font;
};

TEST_F(TextTest, getRendererId)
{
    Text text{textId, position, textValue, font, characterSize, color};

    EXPECT_EQ(textId, text.getGraphicsId());
}

TEST_F(TextTest, getText)
{
    Text text{textId, position, textValue, font, characterSize, color};

    EXPECT_EQ(textValue, text.getText());
}