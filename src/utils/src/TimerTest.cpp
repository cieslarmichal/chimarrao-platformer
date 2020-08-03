#include "Timer.h"

#include <chrono>
#include <thread>

#include "gtest/gtest.h"

using namespace utils;
using namespace std::literals::chrono_literals;

class TimerTest : public testing::Test
{
public:
    Timer timer;
};

TEST_F(TimerTest, timerCountTimePropertly)
{
    timer.start();
    std::this_thread::sleep_for(10ms);
    ASSERT_TRUE(timer.getDurationFromLastUpdate() >= 10ms);
}