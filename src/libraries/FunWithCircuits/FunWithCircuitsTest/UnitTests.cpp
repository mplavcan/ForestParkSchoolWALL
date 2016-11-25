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
       
    WallMock *mockedWall;
    FunWithCircuits *app;

    ApplicationFixture() {
        mockedWall = new WallMock;
        app = new FunWithCircuits(mockedWall);
    }

    ~ApplicationFixture() {   
        delete app;
        delete mockedWall;
    }
};


TEST_F(ApplicationFixture, TestActivateTransducer)
{
    const unsigned int threshold = 100;
    const unsigned int inputValue = threshold + 1;
    EXPECT_CALL(*mockedWall, turnTransducerOn());
    app->driveTransducer(inputValue, threshold);
}

}; // namespace



// Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
