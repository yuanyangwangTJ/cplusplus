#include <gtest/gtest.h>

// 一个简单的函数用于测试
int Add(int a, int b) {
    return a + b;
}

// 测试用例
TEST(AddTest, PositiveNumbers) {
    EXPECT_EQ(Add(1, 2), 3);
    EXPECT_EQ(Add(100, 200), 300);
}

TEST(AddTest, NegativeNumbers) {
    EXPECT_EQ(Add(-1, -2), -3);
    EXPECT_EQ(Add(-10, -20), -30);
}

TEST(AddTest, ZeroValues) {
    EXPECT_EQ(Add(0, 0), 0);
    EXPECT_EQ(Add(0, 10), 10);
    EXPECT_EQ(Add(-5, 0), -5);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
