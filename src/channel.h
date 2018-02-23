// channel.h
#ifndef channel_h
#define channel_h
#include <string>
#include <vector>

class Channel {
  public:
    Channel(int n_in, n_out) : n_in(n_in_), n_out(n_out_);
    
    // This function parses a channel string.
    void ParseInput(std::string input_str);
    
    // This function returns a string that represents the
    // current channel.
    std::string to_string();
    
    // This function transposes the current channel.
    // The input becomes the output, and the output becomes the
    // input; matrix-wise we'll have p(x|y) instead of p(y|x).
    void Transpose();
    
    // This function randomizes the current channel.
    // Maintaining the channel dimensions.
    void Randomize();

  private:

    // This is the channel matrix. ( p(y|x) )
    std::vector<std::vector<double> > c_matrix;

    // These ints keep the number of input lines we have,
    // and the number of output lines.
    int n_in_, n_out_;

    // These vectors keep the names of each input line and
    // each output line
    std::vector<std::string> in_names_, out_names_;
};

#endif
