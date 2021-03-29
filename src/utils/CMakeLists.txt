set(SOURCES
        src/Timer.cpp
        src/GetExecutablePath.cpp
        src/GetProjectPath.cpp
        src/StringHelper.cpp
        src/IncrementalFilePathsCreator.cpp
        src/RandomNumberMersenneTwisterGenerator.cpp
        src/UniqueName.cpp
        src/exceptions/FileNotFound.h
        )

set(UT_SOURCES
        src/TimerTest.cpp
        src/GetExecutablePathTest.cpp
        src/GetProjectPathTest.cpp
        src/StringHelperTest.cpp
        src/IncrementalFilePathsCreatorTest.cpp
        src/RandomNumberMersenneTwisterGeneratorTest.cpp
        src/UniqueNameTest.cpp
        )

add_library(utils ${SOURCES})
target_include_directories(utils PUBLIC src ${SFML_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
target_link_libraries(utils PUBLIC ${SFML_LIBRARIES})

add_executable(utilsUT ${UT_SOURCES})
target_link_libraries(utilsUT PUBLIC gtest_main gmock utils)
add_test(utilsUT utilsUT --gtest_color=yes)
