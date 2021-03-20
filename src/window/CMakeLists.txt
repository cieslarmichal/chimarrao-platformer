set(SOURCES
        src/WindowSfml.cpp
        src/WindowFactory.cpp
        src/DefaultWindowFactory.cpp
        src/DefaultWindowObservationHandler.cpp
        src/SupportedResolutionsRetriever.cpp
        src/SupportedFrameLimitsRetriever.cpp
        )

set(UT_SOURCES
        src/WindowSfmlTest.cpp
        src/DefaultWindowObservationHandlerTest.cpp
        src/SupportedResolutionsRetrieverTest.cpp
        src/SupportedFrameLimitsRetrieverTest.cpp
        )

add_library(window ${SOURCES})
target_link_libraries(window PUBLIC ${SFML_LIBRARIES} utils)
target_include_directories(window PUBLIC src)

add_executable(windowUT ${UT_SOURCES})
target_link_libraries(windowUT PUBLIC gmock_main gtest window)
add_test(windowUT windowUT --gtest_color=yes)
