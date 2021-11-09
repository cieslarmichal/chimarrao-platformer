#include "DefaultUIManager.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "ProjectPathReader.h"
#include "exceptions/InvalidUIConfig.h"
#include "exceptions/UIComponentNotFound.h"
#include "exceptions/UIConfigNotFound.h"

using namespace components::ui;
using namespace ::testing;

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto dummyColor = graphics::Color(251, 190, 102);
const auto dummySize = utils::Vector2f{25.f, 5.f};
const auto dummyPosition = utils::Vector2f{25.f, 5.f};
const auto dummyFontSize = 4;
const auto dummyText = "dummyText";
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
const auto imagePath = projectPath + "resources/BG/menu_background.jpg";
const auto backgroundName = "dummyBackground";
const auto buttonName = "dummyButton";
const auto checkBoxName = "dummyCheckBox";
const auto labelName = "dummyLabel";
const auto textFieldName = "dummyTextField";
const auto imageName = "dummyImage";
const std::string text{"text"};
boost::optional<std::string> textFromRendererPool{text};
const auto nonExistingComponent = "non existing component";

std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig()
{
    return std::make_unique<components::ui::BackgroundConfig>(
        backgroundName, utils::Vector2f{0.f, 0.f}, utils::Vector2f{80.f, 60.f},
        graphics::VisibilityLayer::Background, imagePath);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs()
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;

    const auto dummyButtonMouseOver = [=] {};
    const auto dummyButtonMouseOut = [=] {};
    auto dummyButtonMouseOverActions =
        components::ui::MouseOverActions{dummyButtonMouseOver, dummyButtonMouseOut};
    auto dummyButtonClickAction = [=] {};
    auto dummyButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        buttonName, dummyPosition, dummySize, dummyColor, dummyText, dummyColor, dummyFontSize, fontPath,
        utils::Vector2f{2.f, 0.f}, dummyButtonClickAction, dummyButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(dummyButtonConfig));

    return buttonsConfig;
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs()
{
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> checkBoxesConfig;

    const auto dummyCheckBoxOnMouseOver = [=] {};
    const auto dummyCheckBoxOnMouseOut = [=] {};
    auto dummyCheckBoxMouseOverActions =
        components::ui::MouseOverActions{dummyCheckBoxOnMouseOver, dummyCheckBoxOnMouseOut};
    auto dummyCheckBoxClickAction = [=] {};
    auto dummyCheckBoxConfig = std::make_unique<components::ui::CheckBoxConfig>(
        checkBoxName, dummyPosition, dummySize, dummyColor, false, dummyFontSize, fontPath,
        utils::Vector2f{0.8f, 0.0f}, dummyCheckBoxClickAction, dummyCheckBoxMouseOverActions);
    checkBoxesConfig.emplace_back(std::move(dummyCheckBoxConfig));

    return checkBoxesConfig;
}

std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs()
{
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;

    auto dummyLabelConfig = std::make_unique<components::ui::LabelConfig>(
        labelName, dummyPosition, dummyColor, dummyText, dummyFontSize, fontPath);
    labelsConfig.emplace_back(std::move(dummyLabelConfig));

    return labelsConfig;
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>> createTextFieldConfigs()
{
    std::vector<std::unique_ptr<components::ui::TextFieldConfig>> textFieldsConfig;

    const auto dummyTextFieldOnMouseOver = [=] {};
    const auto dummyTextFieldOnMouseOut = [=] {};
    const auto dummyTextFieldMouseOverActions =
        components::ui::MouseOverActions{dummyTextFieldOnMouseOver, dummyTextFieldOnMouseOut};
    const auto dummyTextFieldClickInsideAction = [=] {};
    auto dummyTextFieldClickOutsideAction = [=]() {};
    auto dummyTextFieldConfig = std::make_unique<components::ui::TextFieldConfig>(
        textFieldName, dummyPosition, dummySize, dummyColor, dummyText, dummyFontSize, fontPath,
        utils::Vector2f{0.75f, 0.4f}, dummyTextFieldClickInsideAction, dummyTextFieldClickOutsideAction,
        dummyTextFieldMouseOverActions);
    textFieldsConfig.emplace_back(std::move(dummyTextFieldConfig));

    return textFieldsConfig;
}

std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs()
{
    std::vector<std::unique_ptr<components::ui::ImageConfig>> imagesConfig;

    auto imageConfig = std::make_unique<components::ui::ImageConfig>(
        imageName, dummyPosition, dummySize, graphics::VisibilityLayer::First, imagePath);
    imagesConfig.push_back(std::move(imageConfig));

    return imagesConfig;
}

std::unique_ptr<components::ui::UIConfig> createUIConfig()
{
    return std::make_unique<components::ui::UIConfig>(createBackgroundConfig(), createButtonConfigs(),
                                                      createCheckBoxConfigs(), createLabelConfigs(),
                                                      createTextFieldConfigs(), createImageConfigs());
}

std::unique_ptr<components::ui::UIConfig> createUIConfigWithoutBackgroundConfig()
{
    return std::make_unique<components::ui::UIConfig>(nullptr, createButtonConfigs(), createCheckBoxConfigs(),
                                                      createLabelConfigs(), createTextFieldConfigs(),
                                                      createImageConfigs());
}
}

class DefaultUIManagerTest : public Test
{
public:
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    DefaultUIManager uiManager{sharedContext};
};

TEST_F(DefaultUIManagerTest, createUIWithoutUIConfig_shouldThrowUIConfigNotFound)
{
    ASSERT_THROW(uiManager.createUI(nullptr), exceptions::UIConfigNotFound);
}

TEST_F(DefaultUIManagerTest, createUIWithUIConfigWithoutBackgroundConfig_shouldThrowInvalidUIConfig)
{
    auto UIConfigWithoutBackgroundConfig = createUIConfigWithoutBackgroundConfig();

    ASSERT_THROW(uiManager.createUI(std::move(UIConfigWithoutBackgroundConfig)), exceptions::InvalidUIConfig);
}

TEST_F(DefaultUIManagerTest, createUIWithValidUIConfig_shouldNoThrow)
{
    ASSERT_NO_THROW(uiManager.createUI(createUIConfig()));
}

TEST_F(DefaultUIManagerTest, activate_shouldActivateAllComponents_exceptButtonAndCheckBox)
{
    uiManager.createUI(createUIConfig());
    uiManager.deactivate();

    uiManager.activate();

    ASSERT_TRUE(uiManager.isComponentActive(backgroundName));
    ASSERT_FALSE(uiManager.isComponentActive(buttonName));
    ASSERT_FALSE(uiManager.isComponentActive(checkBoxName));
    ASSERT_TRUE(uiManager.isComponentActive(labelName));
    ASSERT_TRUE(uiManager.isComponentActive(textFieldName));
    ASSERT_TRUE(uiManager.isComponentActive(imageName));
}

TEST_F(DefaultUIManagerTest, deactivate_shouldDeactivateAllComponents)
{
    uiManager.createUI(createUIConfig());
    uiManager.activate();

    uiManager.deactivate();

    ASSERT_FALSE(uiManager.isComponentActive(backgroundName));
    ASSERT_FALSE(uiManager.isComponentActive(buttonName));
    ASSERT_FALSE(uiManager.isComponentActive(checkBoxName));
    ASSERT_FALSE(uiManager.isComponentActive(labelName));
    ASSERT_FALSE(uiManager.isComponentActive(textFieldName));
    ASSERT_FALSE(uiManager.isComponentActive(imageName));
}

TEST_F(DefaultUIManagerTest, deactivateComponent)
{
    uiManager.createUI(createUIConfig());
    uiManager.deactivateComponent(backgroundName);

    ASSERT_FALSE(uiManager.isComponentActive(backgroundName));
    ASSERT_FALSE(uiManager.isComponentActive(buttonName));
    ASSERT_FALSE(uiManager.isComponentActive(checkBoxName));
    ASSERT_TRUE(uiManager.isComponentActive(labelName));
    ASSERT_TRUE(uiManager.isComponentActive(textFieldName));
    ASSERT_TRUE(uiManager.isComponentActive(imageName));
}

TEST_F(DefaultUIManagerTest, deactivateComponentWithNonExistingUIComponent_shouldThrowUIComponentNotFound)
{
    uiManager.createUI(createUIConfig());

    ASSERT_THROW(uiManager.deactivateComponent(nonExistingComponent), exceptions::UIComponentNotFound);
}

TEST_F(DefaultUIManagerTest, activateComponent)
{
    uiManager.createUI(createUIConfig());
    uiManager.deactivate();

    uiManager.activateComponent(textFieldName);

    ASSERT_FALSE(uiManager.isComponentActive(backgroundName));
    ASSERT_FALSE(uiManager.isComponentActive(buttonName));
    ASSERT_FALSE(uiManager.isComponentActive(checkBoxName));
    ASSERT_FALSE(uiManager.isComponentActive(labelName));
    ASSERT_TRUE(uiManager.isComponentActive(textFieldName));
    ASSERT_FALSE(uiManager.isComponentActive(imageName));
}

TEST_F(DefaultUIManagerTest, activateComponentWithNonExistingUIComponent_shouldThrowUIComponentNotFound)
{
    uiManager.createUI(createUIConfig());

    ASSERT_THROW(uiManager.activateComponent(nonExistingComponent), exceptions::UIComponentNotFound);
}

TEST_F(DefaultUIManagerTest, isComponentActiveWithNonExistingUIComponent_shouldThrowUIComponentNotFound)
{
    uiManager.createUI(createUIConfig());

    ASSERT_THROW(uiManager.isComponentActive(nonExistingComponent), exceptions::UIComponentNotFound);
}

TEST_F(DefaultUIManagerTest, setTextWithNonExistingUIComponent_shouldThrowUIComponentNotFound)
{
    uiManager.createUI(createUIConfig());

    ASSERT_THROW(uiManager.setText(nonExistingComponent, dummyText), exceptions::UIComponentNotFound);
}

TEST_F(DefaultUIManagerTest, setText)
{
    uiManager.createUI(createUIConfig());
    const auto newText = "yerba rules";

    uiManager.setText(textFieldName, newText);

    ASSERT_EQ(*uiManager.getText(textFieldName), newText);
}

TEST_F(DefaultUIManagerTest, getTextWithNonExistingUIComponent_shouldThrowUIComponentNotFound)
{
    uiManager.createUI(createUIConfig());

    ASSERT_THROW(uiManager.getText(nonExistingComponent), exceptions::UIComponentNotFound);
}

TEST_F(DefaultUIManagerTest, getText)
{
    uiManager.createUI(createUIConfig());
    EXPECT_CALL(*rendererPool, getText(_)).WillOnce(Return(textFromRendererPool));

    ASSERT_EQ(*uiManager.getText(labelName), textFromRendererPool);
}

TEST_F(DefaultUIManagerTest, setColorWithNonExistingUIComponent_shouldThrowUIComponentNotFound)
{
    uiManager.createUI(createUIConfig());

    ASSERT_THROW(uiManager.setColor(nonExistingComponent, dummyColor), exceptions::UIComponentNotFound);
}

TEST_F(DefaultUIManagerTest, setColorWithExistingUIComponent_shouldNoThrow)
{
    uiManager.createUI(createUIConfig());

    ASSERT_NO_THROW(uiManager.setColor(buttonName, dummyColor));
}

TEST_F(DefaultUIManagerTest, setCheckedWithNonExistingUIComponent_shouldThrowUIComponentNotFound)
{
    uiManager.createUI(createUIConfig());

    ASSERT_THROW(uiManager.setChecked(nonExistingComponent, true), exceptions::UIComponentNotFound);
}

TEST_F(DefaultUIManagerTest, setCheckedWithExistingUIComponent_shouldNoThrow)
{
    uiManager.createUI(createUIConfig());

    ASSERT_NO_THROW(uiManager.setChecked(checkBoxName, true));
}

TEST_F(DefaultUIManagerTest, changeClickActionWithNonExistingUIComponent_shouldThrowUIComponentNotFound)
{
    uiManager.createUI(createUIConfig());

    ASSERT_THROW(uiManager.changeClickAction(nonExistingComponent, {}), exceptions::UIComponentNotFound);
}

TEST_F(DefaultUIManagerTest, changeClickActionWithExistingUIComponent_shouldNoThrow)
{
    uiManager.createUI(createUIConfig());

    ASSERT_NO_THROW(uiManager.changeClickAction(checkBoxName, {}));
}

TEST_F(DefaultUIManagerTest, invokeClickActionWithNonExistingUIComponent_shouldThrowUIComponentNotFound)
{
    uiManager.createUI(createUIConfig());

    ASSERT_THROW(uiManager.invokeClickAction(nonExistingComponent, input::InputKey::MouseLeft),
                 exceptions::UIComponentNotFound);
}

TEST_F(DefaultUIManagerTest, invokeClickActionWithExistingUIComponent_shouldNoThrow)
{
    uiManager.createUI(createUIConfig());

    ASSERT_NO_THROW(uiManager.invokeClickAction(checkBoxName, input::InputKey::MouseLeft));
}
