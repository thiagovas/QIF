// channel.h
#ifndef channel_h
#define channel_h
#include <string>
#include <vector>
#include <iostream>

// Channel matrix
// x_0 -> y_0, y_1, ..., y_m
// ...
// x_n -> y_0, y_1, ..., y_m

class Channel {
  public:
    Channel(int n_in=2, int n_out=2);

    int n_in() const {
      return this->n_in_;
    }

    int n_out() const {
      return this->n_out_;
    }
    
    std::vector<std::string> in_names() const {
      return this->in_names_;
    }
    
    std::vector<std::string> out_names() const {
      return this->out_names_;
    }

    // This function parses a channel string.
    void ParseInput(std::string input_str);

    // This function resets the class to an initial state.
    void Reset();

    // This function returns a string that represents the
    // current channel.
    std::string to_string() const;
    
    // TODO(thiagovas): Describe what this function does.
    bool CompatibleChannels(const Channel& c1, const Channel& c2) const;

    friend std::ostream& operator<< (std::ostream& stream, const Channel& c);

    // This function randomizes the current channel.
    // Maintaining the channel dimensions.
    void Randomize();
    

    // TODO(thiagovas): Improvement. Create a namespace 'metrics',
    //                    or something like that.
    // OLD METRICS
    // In bits
    double ShannonEntropyPrior() const;
    double ShannonEntropyOut() const;
    double ConditionalEntropy() const;
    double ConditionalEntropyHyper() const;
    double JointEntropy() const;
    double GuessingEntropy() const;
    double MutualInformation() const;
    double NormalizedMutualInformation() const;
    double SymmetricUncertainty() const;
    
    double BayesVulnerabilityPrior() const; // V(X)
    double BayesVulnerabilityOut() const; // V(Y)
    double BayesVulnerabilityPosterior() const; // V(X|Y)
    double BayesVulnerabilityReversePosterior() const; // V(Y|X)
    
    // Multiplicative Leakages.
    double BayesLeakageMultPosterior() const; // V(X|Y) / V(X)
    double BayesLeakageMultReversePosterior() const; // V(Y|X) / V(Y)
    
    // Additive leakages.
    double BayesLeakageAddPosterior() const; // V(X|Y) - V(X)
    double BayesLeakageAddReversePosterior() const; // V(Y|X) - V(Y)


  private:
    // This is the channel matrix. ( p(y|x) )
    std::vector<std::vector<double> > c_matrix;
    
    // This is the posterior probability matrix. ( hyper distribution p(x|y) )
    std::vector<std::vector<double> > h_matrix;
    
    // This is the prior distribution.
    std::vector<double> prior_distribution;
    
    // This is the output distribution.
    std::vector<double> out_distribution;

    // This is the joint matrix. ( p(x,y) )
    std::vector<std::vector<double> > j_matrix;

    // The maximum p(x, y) per x; used by the bayes' metrics.
    std::vector<double> max_pinput;

    // The maximum p(x, y) per y; used by the bayes' metrics.
    std::vector<double> max_poutput;

    // These ints keep the number of input lines we have,
    // and the number of output lines.
    int n_in_, n_out_;

    // This is the norm used to randomly generate the channel.
    int base_norm_;

    // These vectors keep the names of each input line and
    // each output line
    std::vector<std::string> in_names_, out_names_;
};

#endif
