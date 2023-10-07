#define BOOST_TEST_MODULE test_version

#include "proj_info.h"

#include <gtest/gtest.h>

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(SuiteName, Name) {
    EXPECT_GT(project_info::version(), 0);
}
