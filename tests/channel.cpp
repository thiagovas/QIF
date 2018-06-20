#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "channel/channel.h"
#include "gtest/gtest.h"
using namespace std;

TEST(HelloTestAcc, cname) {
  EXPECT_EQ("Hello Bazel", "Hello Bazel");
}

// This unit test checks whether the ParseInput can understand the output
// of the to_string function.
TEST(ParsingTest, WithCnameWithInput) {
  // TODO(thiagovas): To Be Implemented...
}

TEST(ParsingTest, WithCnameWithoutInput) {
  // TODO(thiagovas): To Be Implemented...
}

TEST(ParsingTest, WithoutCnameWithInput) {
  // TODO(thiagovas): To Be Implemented...
}

TEST(ParsingTest, WithoutCnameWithoutInput) {
  // TODO(thiagovas): To Be Implemented...
}

/*
 Functions to be tested:

 void ParseInput(std::string input_str);
 void ParseFile(std::string fname);
 void Reset();
 void Identity();
 std::string to_string() const;
 static bool CompatibleChannels(const Channel& c1, const Channel& c2);
 friend std::ostream& operator<< (std::ostream& stream, const Channel& c);
 friend Channel operator|| (const Channel& c1, const Channel& c2);
 friend Channel operator* (const Channel& c1, const Channel& c2);
 static Channel hidden_choice (const Channel& c1, const Channel& c2, const double prob);
 void Randomize();
*/

