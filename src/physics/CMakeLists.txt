set(SOURCES
        src/PhysicsSfmlComponent.cpp
        )

set(UT_SOURCES src/PhysicsSfmlComponentTest.cpp)

add_library(physics ${SOURCES})
target_link_libraries(physics PUBLIC utils sfml-graphics sfml-window sfml-system)
target_include_directories(physics PUBLIC src)


add_executable(physicsUT ${UT_SOURCES})
target_link_libraries(physicsUT PUBLIC gtest_main gmock physics)
