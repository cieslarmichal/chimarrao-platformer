set(SOURCES
        src/InputSfml.cpp
        src/DefaultInputManager.cpp
        src/InputKeySfmlMapper.cpp
        src/InputManagerFactory.cpp
        src/DefaultInputManagerFactory.cpp
        )

set(UT_SOURCES
        src/InputSfmlTest.cpp
        src/InputKeyTest.cpp
        src/DefaultInputManagerTest.cpp
        src/InputKeySfmlMapperTest.cpp
        )

add_library(input SHARED ${SOURCES})
target_link_libraries(input PUBLIC window utils)
target_include_directories(input PUBLIC src)
target_compile_options(input PUBLIC ${FLAGS})

add_executable(inputUT ${UT_SOURCES} ${SOURCES})
target_link_libraries(inputUT PUBLIC gmock_main gtest window utils)
target_compile_options(inputUT PUBLIC ${FLAGS})
add_test(NAME inputUT COMMAND inputUT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

