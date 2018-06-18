#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "base/distribution.h"
#include "gtest/gtest.h"
using base::Distribution;


// Testing cases which isdistribution must return true.
TEST(IsDistribution, RetTrue) {
  for(int i = 0; i < 10; i++) {
    Distribution d1(i, true);
    ASSERT_TRUE(d1.isDistribution(d1.pdist())) << "Error when i=" << i;
    
    Distribution d2(i, false);
    ASSERT_TRUE(d2.isDistribution(d2.pdist())) << "Error when i=" << i;
  }
}


// Testing cases which isdistribution must return false.
TEST(IsDistribution, RetFalse) {
  std::vector<double> vec1(1, 0);
  Distribution d1(vec1, 0);
  ASSERT_FALSE(d1.isDistribution(vec1));
  
  
  std::vector<double> vec2(5, 1);
  Distribution d2(vec2, 0);
  ASSERT_FALSE(d2.isDistribution(vec2));
}

// Testing if a uniform distribution is being correctly generated.
TEST(DistributionGeneration, uniform) {
  std::vector<double> dist = Distribution::GenerateUniformDistribution(1)
                                           .pdist();
  ASSERT_TRUE(dist.size() == 1);
  ASSERT_TRUE(fabs(dist[0]-1) < 1e-9);
  
  dist = Distribution::GenerateUniformDistribution(2).pdist();
  ASSERT_TRUE(dist.size() == 2);
  ASSERT_TRUE(fabs(dist[0]-0.5) < 1e-9);
  ASSERT_TRUE(fabs(dist[1]-0.5) < 1e-9);
}
