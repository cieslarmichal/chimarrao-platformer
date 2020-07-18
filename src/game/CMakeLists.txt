set(SOURCES
        src/MovableEntity.cpp
        src/NonMovableEntity.cpp
        src/Game.cpp
        )

add_library(game ${SOURCES})

target_include_directories(game PUBLIC ${SFML_INCLUDE_DIR})

target_link_libraries (game PUBLIC sfml-graphics sfml-window sfml-system)