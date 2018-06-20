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

TEST(Measures, MutualInformation) {
  // Test drawn from Mackay's book.
  // Execise 8.6

  vector<vector<double> > j_matrix = {
    {1.0/8.0, 1.0/16.0, 1.0/32.0, 1.0/32.0},
    {1.0/16.0, 1.0/8.0, 1.0/32.0, 1.0/32.0},
    {1.0/16.0, 1.0/16.0, 1.0/16.0, 1.0/16.0},
    {0.25, 0, 0, 0}
  };
  
  vector<vector<double> > c_matrix(4, vector<double>(4, 0));
  vector<double> prior(4, 0);
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      prior[i] += j_matrix[i][j];
  
  // Computing c_matrix...
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      c_matrix[i][j] = j_matrix[i][j]/prior[i];
  
  channel::Channel c(c_matrix, prior);
  
  // Expected value: 3.0/8.0
  ASSERT_TRUE(fabs(c.MutualInformation() - 3.0/8.0) < 1e-6) << " expected " << 3.0/8.0 <<  ", got " << c.MutualInformation() << "\n";
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
 double NormalizedMutualInformation() const;
 double SymmetricUncertainty() const;
*/

