#ifndef _base_distribution_h
#define _base_distribution_h

#include <vector>

namespace base {

  class Distribution {
    public:
      // Creates a new distribution with 'size' elements.
      //  'uniform' indicates whether the distribution will be uniform,
      //  or randomly generated.
      Distribution(int size, bool uniform=true);

      // Build the object from a previous distribution.
      Distribution(const std::vector<double>& dist, long long base_norm=-1);
      
      // Getter to pdist.
      const std::vector<double>& pdist() const { return this->pdist_; }

      // Generates a new random distribution.
      static Distribution GenerateRandomDistribution(int size);
      
      // Returns a uniform probability distribution with 'size' items.
      static Distribution GenerateUniformDistribution(int size);

      // Returns p(x)
      double operator[](unsigned x);

      // Creates a new random distribution, maintaining the same size.
      void Randomize();

      // Returns the shannon entropy of this distribution.
      double ShannonEntropy() const;

      // Returns if the received vector contains a probability distribution.
      bool isDistribution(const std::vector<double> &dist);
      
    private:
      // The vector that keeps the distribution.
      std::vector<double> pdist_;
      
      long long base_norm_;

      // Epsilon value used by this class to make comparisons between floats.
      const double eps = 1e-8;


      // Generates a new random distribution.
      static void GenerateRandomDistribution(int size, std::vector<double>& vdist, long long& base_norm);

      static void GenerateUniformDistribution(int size, std::vector<double>& vdist, long long& base_norm);
  };

} // namespace base

#endif
