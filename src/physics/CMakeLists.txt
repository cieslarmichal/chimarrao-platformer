set(SOURCES
        src/Quadtree.cpp
        src/RayCast.cpp
        src/DefaultCollisionSystem.cpp
        src/PhysicsFactory.cpp
        src/DefaultPhysicsFactory.cpp
        )

set(UT_SOURCES
        src/QuadtreeTest.cpp
        src/RayCastTest.cpp
        src/DefaultCollisionSystemTest.cpp
        )

add_library(physics ${SOURCES})
target_link_libraries(physics PUBLIC components utils)
target_include_directories(physics PUBLIC src)
target_compile_options(physics PUBLIC ${FLAGS})

add_executable(physicsUT ${UT_SOURCES} ${SOURCES})
target_link_libraries(physicsUT PUBLIC gtest_main gmock components utils)
target_compile_options(physicsUT PUBLIC ${FLAGS})

add_test(NAME physicsUT COMMAND physicsUT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

