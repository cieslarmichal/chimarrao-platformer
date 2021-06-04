#include "DefaultTimer.h"

#include <chrono>
#include <thread>

#include "gtest/gtest.h"

using namespace utils;
using namespace std::literals::chrono_literals;

class DefaultTimerTest : public testing::Test
{
public:
    DefaultTimer timer;
};

TEST_F(DefaultTimerTest, getElapsedSeconds)
{
    std::this_thread::sleep_for(20ms);

    const auto elapsedSeconds = timer.getElapsedSeconds();

    ASSERT_TRUE(elapsedSeconds >= 0.02);
}

TEST_F(DefaultTimerTest, restart)
{
    std::this_thread::sleep_for(20ms);

    timer.restart();

    const auto elapsedSeconds = timer.getElapsedSeconds();
    ASSERT_TRUE(elapsedSeconds < 0.02);
}