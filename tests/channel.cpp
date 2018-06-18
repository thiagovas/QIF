#include "channel/channel.h"
#include "gtest/gtest.h"

TEST(HelloTestAcc, cname) {
  EXPECT_EQ("Hello Bazel", "Hello Bazel");
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
 double ShannonEntropyPrior() const;
 double ShannonEntropyOut() const;
 double ConditionalEntropy() const;
 double ConditionalEntropyHyper() const;
 double JointEntropy() const;
 double GuessingEntropy() const;
 double MutualInformation() const;
 double NormalizedMutualInformation() const;
 double SymmetricUncertainty() const;
*/

