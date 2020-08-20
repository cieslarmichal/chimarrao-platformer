set(SOURCES
        src/BitMask.cpp
        src/Timer.cpp
        src/GetExecutablePath.cpp
        src/GetProjectPath.cpp
        src/StringHelper.cpp
        src/IncrementalFilePathsCreator.cpp
        )

set(UT_SOURCES
        src/BitMaskTest.cpp
        src/TimerTest.cpp
        src/GetExecutablePathTest.cpp
        src/GetProjectPathTest.cpp
        src/StringHelperTest.cpp
        src/IncrementalFilePathsCreatorTest.cpp
        )

add_library(utils ${SOURCES})
add_executable(utilsUT ${UT_SOURCES})

target_include_directories(utils PUBLIC src)

target_link_libraries(utils PUBLIC ${SFML_LIBRARIES})
target_link_libraries(utilsUT PUBLIC gtest_main gmock utils)
