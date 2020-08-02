set(SOURCES
        src/PhysicsIdGenerator.cpp
        src/PhysicsEntity.cpp
        )

set(UT_SOURCES
        src/PhysicsEntityTest.cpp
        )

add_library(physics ${SOURCES})
target_link_libraries(physics PUBLIC utils ${SFML_LIBRARIES})
target_include_directories(physics PUBLIC src)


add_executable(physicsUT ${UT_SOURCES})
target_link_libraries(physicsUT PUBLIC gtest_main gmock physics)
