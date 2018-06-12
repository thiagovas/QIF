#include "channel/channel.h"
#include "gtest/gtest.h"

TEST(HelloTestAcc, cname) {
  EXPECT_EQ("Hello Bazel", "Hello Bazel");
}

TEST(HelloTestErr, cname) {
  EXPECT_EQ("Bazel", "Hello Bazel");
}
