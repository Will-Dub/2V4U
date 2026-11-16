#include "../../App/Inc/Logic/ThrottleMapper.hpp"

#include <gtest/gtest.h>

using namespace App::Logic;

class ThrottleMapperTest : public ::testing::Test
{
  protected:
    ThrottleMapConfig config{0.04f, 1.5f, 100.0f, 1.5f, 4.0f};
    ThrottleMapper mapper{config};
};

TEST_F(ThrottleMapperTest, GivenThrottleInDeadzone_WhenUpdated_ThenOutputZero)
{
    EXPECT_FLOAT_EQ(mapper.update(0.03f, 0.01f), 0.0f);
}

TEST_F(ThrottleMapperTest, GivenFullThrottle_WhenOneTick_ThenRisesByRateOnly)
{
    EXPECT_NEAR(mapper.update(1.0f, 0.01f), 1.5f, 0.01f); // 1.5 ratio/s * 0.01 s * 100 A
}

TEST_F(ThrottleMapperTest, GivenFullThrottleHeld_WhenSettled_ThenOutputIsMaxAmps)
{
    for (int i = 0; i < 200; i++)
        mapper.update(1.0f, 0.01f);
    EXPECT_NEAR(mapper.getOutput(), 100.0f, 0.01f);
}

TEST_F(ThrottleMapperTest, GivenAtMax_WhenThrottleReleased_ThenFallsFasterThanItRose)
{
    for (int i = 0; i < 200; i++)
        mapper.update(1.0f, 0.01f);
    EXPECT_NEAR(mapper.update(0.0f, 0.01f), 100.0f - 4.0f, 0.01f);
}

TEST_F(ThrottleMapperTest, GivenOutputRaised_WhenReset_ThenOutputZero)
{
    mapper.update(1.0f, 0.5f);
    mapper.reset();
    EXPECT_FLOAT_EQ(mapper.getOutput(), 0.0f);
}