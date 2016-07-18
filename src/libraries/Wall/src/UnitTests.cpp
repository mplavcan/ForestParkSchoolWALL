#include <gtest/gtest.h>
#include "Wall.h"

class WallFixture : public ::testing::Test {
protected:
    
    Wall wall;

    WallFixture() {
    }

    virtual ~WallFixture() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};


TEST_F(WallFixture, FirstTest)
{
    
    ASSERT_EQ(true, wall.Initialize());
}


// Entry point for Google Test
//
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}