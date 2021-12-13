set(SOURCES
        src/SfmlMusicManager.cpp
        )

set(UT_SOURCES
        )

add_library(audio SHARED ${SOURCES})
target_link_libraries(audio PUBLIC utils)
target_include_directories(audio PUBLIC src)
target_compile_options(audio PUBLIC ${FLAGS})

add_executable(audioUT ${UT_SOURCES} ${SOURCES})
target_link_libraries(audioUT PUBLIC gmock_main gtest utils)
target_compile_options(audioUT PUBLIC ${FLAGS})

add_test(NAME audioUT COMMAND audioUT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

