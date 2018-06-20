#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "channel/channel.h"
#include "gtest/gtest.h"
using namespace std;


class MeasuresTest : public ::testing::Test {
  public:
    vector<vector<double> > j_matrix;
    vector<vector<double> > c_matrix;
    vector<double> prior;

    MeasuresTest() {
      this->j_matrix = {
        {1.0/8.0, 1.0/16.0, 1.0/32.0, 1.0/32.0},
        {1.0/16.0, 1.0/8.0, 1.0/32.0, 1.0/32.0},
        {1.0/16.0, 1.0/16.0, 1.0/16.0, 1.0/16.0},
        {0.25, 0, 0, 0}
      };

      this->c_matrix = vector<vector<double> >(4, vector<double>(4, 0));
      this->prior = vector<double>(4, 0);
      for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
          this->prior[i] += this->j_matrix[i][j];

      // Computing c_matrix...
      for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
          this->c_matrix[i][j] = this->j_matrix[i][j]/this->prior[i];
    }
};


TEST_F(MeasuresTest, MutualInformation) {
  // Test drawn from Mackay's book.
  // Execise 8.6
  channel::Channel c(c_matrix, prior);

  // Expected value: 3.0/8.0
  ASSERT_TRUE(fabs(c.MutualInformation() - 3.0/8.0) < 1e-6) << " expected " << 3.0/8.0 <<  ", got " << c.MutualInformation() << "\n";
}


TEST_F(MeasuresTest, ShannonEntropyPrior) {
  channel::Channel c(c_matrix, prior);
  ASSERT_TRUE(fabs(c.ShannonEntropyPrior() - 2) < 1e-6);
}


TEST_F(MeasuresTest, ShannonEntropyOut) {
  channel::Channel c(c_matrix, prior);
  ASSERT_TRUE(fabs(c.ShannonEntropyOut() - 7.0/4.0) < 1e-6);
}


TEST_F(MeasuresTest, ConditionalEntropy) {
  channel::Channel c(c_matrix, prior);
  ASSERT_TRUE(fabs(c.ConditionalEntropy() - 11.0/8.0) < 1e-6);
}


TEST_F(MeasuresTest, ConditionalEntropyHyper) {
  channel::Channel c(c_matrix, prior);
  ASSERT_TRUE(fabs(c.ConditionalEntropyHyper() - 13.0/8.0) < 1e-6);
}


TEST_F(MeasuresTest, JointEntropy) {
  channel::Channel c(c_matrix, prior);
  ASSERT_TRUE(fabs(c.JointEntropy() - 27.0/8.0) < 1e-6);
}


TEST_F(MeasuresTest, GuessingEntropy) {
  channel::Channel c(c_matrix, prior);
  ASSERT_TRUE(fabs(c.GuessingEntropy() - 2.5) < 1e-6);
}


TEST_F(MeasuresTest, NormalizedMutualInformation) {
  // TODO(thiagovas): To Be Implemented...
}

TEST_F(MeasuresTest, SymmetricUncertainty) {
  // TODO(thiagovas): To Be Implemented...
}
