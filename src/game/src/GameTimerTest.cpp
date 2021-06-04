#include "GameTimer.h"

#include <chrono>
#include <thread>

#include "gtest/gtest.h"

using namespace game;
using namespace std::literals::chrono_literals;

class GameTimerTest : public testing::Test
{
public:
    GameTimer timer;
};

TEST_F(GameTimerTest, getDurationFromLastUpdate)
{
    std::this_thread::sleep_for(10ms);
    const auto durationFromLastUpdate1 = timer.getDurationFromLastUpdate();
    std::this_thread::sleep_for(15ms);
    const auto durationFromLastUpdate2 = timer.getDurationFromLastUpdate();

    ASSERT_TRUE(durationFromLastUpdate1 >= 10ms);
    ASSERT_TRUE(durationFromLastUpdate2 >= 15ms && durationFromLastUpdate2 < 25ms);
}
