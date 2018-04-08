// channel.h
#ifndef channel_h
#define channel_h
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <cmath>

// Channel matrix
// x_0 -> y_0, y_1, ..., y_m
// ...
// x_n -> y_0, y_1, ..., y_m

class Channel {
  public:
    Channel(int n_in, int n_out);
    
    // This function parses a channel string.
    void ParseInput(std::string input_str);
    
    // This function returns a string that represents the
    // current channel.
    std::string to_string();

    bool CompatibleChannels(Channel c1, Channel c2);
    
    friend std::ostream& operator<< (std::ostream& stream, const Channel& c);


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

    // These ints keep the number of input lines we have,
    // and the number of output lines.
    int n_in_, n_out_;

    // These vectors keep the names of each input line and
    // each output line
    std::vector<std::string> in_names_, out_names_;

    // This function randomizes the current channel.
    // Maintaining the channel dimensions.
    void Randomize();

    // This function transposes the current channel.
    // The input becomes the output, and the output becomes the
    // input; matrix-wise we'll have p(x|y) instead of p(y|x).
    void Transpose();

    // OLD METRICS
    // In bits
    double ShannonEntropy();
    double ConditionalEntropy();
    double JointEntropy();
    double GuessingEntropy();
    double MutualInformation();
};

#endif
