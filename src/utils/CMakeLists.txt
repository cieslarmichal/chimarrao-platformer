file(WRITE src/dummy.cpp "")

set(SOURCES src/dummy.cpp)

add_library(utils ${SOURCES})

target_include_directories(utils PUBLIC src)

target_link_libraries (utils PUBLIC sfml-graphics sfml-window sfml-system)