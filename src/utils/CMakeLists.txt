set(SOURCES
        src/Bitmask.cpp
        src/DefaultTimer.cpp
        src/ProjectPathReader.cpp
        src/StringHelper.cpp
        src/IncrementalFilePathsCreator.cpp
        src/RandomNumberMersenneTwisterGenerator.cpp
        src/UniqueName.cpp
        src/DefaultFileAccess.cpp
        src/DefaultFileAccessFactory.cpp
        src/FileAccessFactory.cpp
        src/TimerFactory.cpp
        src/UniqueIdGenerator.cpp
        )

set(UT_SOURCES
        src/BitmaskTest.cpp
        src/DefaultTimerTest.cpp
        src/ProjectPathReaderTest.cpp
        src/StringHelperTest.cpp
        src/IncrementalFilePathsCreatorTest.cpp
        src/RandomNumberMersenneTwisterGeneratorTest.cpp
        src/UniqueNameTest.cpp
        src/DefaultFileAccessTest.cpp
        )

add_library(utils STATIC ${SOURCES})
target_include_directories(utils PUBLIC src ${SFML_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
target_link_libraries(utils PUBLIC ${SFML_LIBRARIES})
target_compile_options(utils PUBLIC ${FLAGS})

add_executable(utilsUT ${UT_SOURCES} ${SOURCES})
target_include_directories(utilsUT PUBLIC ${SFML_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
target_link_libraries(utilsUT PUBLIC gmock_main gtest ${SFML_LIBRARIES})
target_compile_options(utilsUT PUBLIC ${FLAGS})
add_test(NAME utilsUT COMMAND utilsUT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
