#include <algorithm>
#include <random>

#include "distribution.h"

namespace base {
  // Creates a new distribution with 'size' elements.
  //  'uniform' indicates whether the distribution will be uniform,
  //  or randomly generated.
  Distribution::Distribution(int size, bool uniform) {
    if(uniform) {
      Distribution::GenerateUniformDistribution(size, this->pdist_,
                                                this->base_norm_);
    }
    else {
      Distribution::GenerateRandomDistribution(size, this->pdist_,
                                               this->base_norm_);
    }
  }

  // Build the object from a previous distribution.
  Distribution::Distribution(const std::vector<double>& dist,
                             long long base_norm) {
    this->pdist_ = dist;
    this->base_norm_ = base_norm;
  }

  // Generates a new random distribution.
  Distribution Distribution::GenerateRandomDistribution(int size) {
    std::vector<double> vdist;
    long long base_norm = 0;
    
    Distribution::GenerateRandomDistribution(size, vdist, base_norm);

    return Distribution(vdist, base_norm);
  }

  // Returns a uniform probability distribution with 'size' items.
  Distribution Distribution::GenerateUniformDistribution(int size) {
    std::vector<double> vdist(size, ((double)1.0f)/size);
    return Distribution(vdist, size);
  }

  // Generates a new random distribution.
  void Distribution::GenerateRandomDistribution(int size,
                                                std::vector<double>& vdist,
                                                long long& base_norm) {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,1e7);
    vdist.resize(size, 0);
    base_norm = 0;
    
    for(int i = 0; i < size; i++) {
      int neue = dist(rng);
      base_norm += neue;
      vdist[i] = neue;
    }
    
    for(int i = 0; i < size; i++) {
      vdist[i] /= base_norm;
    }
  }
  
  void Distribution::GenerateUniformDistribution(int size,
                                                 std::vector<double>& vdist,
                                                 long long& base_norm) {
    vdist.resize(size, 1.0f/size);
    base_norm = size;
  }

  // Returns p(x)
  double Distribution::operator[](unsigned x) {
    return this->pdist_[x];
  }

  // Creates a new random distribution, maintaining the same size.
  void Distribution::Randomize() {
    this->pdist_ = Distribution::GenerateRandomDistribution(this->pdist_.size()).pdist();
  }

  // Returns the shannon entropy of this distribution.
  double Distribution::ShannonEntropy() const {
    double entropy = 0;
    for(double value : this->pdist_) {
      if(value > 0)
        entropy += (value*log2(1.0f/value));
    }
    return entropy;
  }

  // Returns the guessing entropy of this distribution.
  double Distribution::GuessingEntropy() const {
    double entropy = 0; 
    std::vector<double> sorted_prior = this->pdist();
    std::sort(sorted_prior.begin(), sorted_prior.end());
    std::reverse(sorted_prior.begin(), sorted_prior.end());
    int i = 1;
    for(double value : sorted_prior) {
      if(value > 0)
        entropy += ((i++)*value);
    }
    return entropy;
  } 

  // Returns if the received vector contains a probability distribution.
  bool Distribution::isDistribution(const std::vector<double> &dist) {
    if(dist.size() == 0) return true;
    
    double sum = 0;
    for(double value : dist) {
      if(value < 0) return false;
      sum += value;
    }
    return (fabs(sum-1.0f) < this->eps);
  }
}
