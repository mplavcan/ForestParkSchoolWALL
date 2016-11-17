// Unit test for FunWithCircuits application
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <FunWithCircuits.h>
#include <WallMock.h>

namespace testing {

using testing::StrictMock;

class ApplicationFixture : public Test {
protected:
       
    ApplicationFixture() {
        WallMock *wall = (new WallMock)->usingFactory(NULL);
    }

    ~ApplicationFixture() {   
    }
};


TEST_F(ApplicationFixture, TestNothing)
{
    ASSERT_TRUE(true);
}

}; // namespace



// Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
