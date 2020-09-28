set(SOURCES
        src/WindowSfml.cpp
        src/WindowFactory.cpp
        src/DefaultWindowFactory.cpp
        )

add_library(window ${SOURCES})
target_link_libraries(window PUBLIC ${SFML_LIBRARIES} utils)
target_include_directories(window PUBLIC src)
