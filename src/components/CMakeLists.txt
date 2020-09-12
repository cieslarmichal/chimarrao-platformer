set(SOURCES

        )

add_library(components ${SOURCES})
target_link_libraries(components PUBLIC utils)
target_include_directories(components PUBLIC src)

set(UT_SOURCES

        )

add_executable(componentsUT ${UT_SOURCES})
target_link_libraries(componentsUT PUBLIC gtest_main gmock)
