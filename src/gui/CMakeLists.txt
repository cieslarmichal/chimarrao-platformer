set(SOURCES
        src/WindowSfml.cpp
        src/Button.cpp
        src/GuiFactory.cpp
        src/DefaultGuiFactory.cpp
        )

add_library(gui ${SOURCES})
target_link_libraries(gui PUBLIC ${SFML_LIBRARIES} utils input components)
target_include_directories(gui PUBLIC src)

set(UT_SOURCES
        src/ButtonTest.cpp
        )

add_executable(guiUT ${UT_SOURCES})
target_link_libraries(guiUT PUBLIC gtest_main gmock gui)

add_test(guiUT guiUT --gtest_color=yes)
