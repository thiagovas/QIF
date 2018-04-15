#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <random>
#include <sstream>

#include "channel.h"

namespace channel {

  Channel::Channel(int n_in, int n_out) : n_in_(n_in), n_out_(n_out) {
    this->Reset();
    this->Randomize();
  }

  Channel::Channel(const std::vector<std::vector<double> >& j_matrix,
      const std::vector<double>& prior_distribution,
      int base_norm) {
    this->Reset();
    this->base_norm_ = base_norm_;
    this->build_channel(j_matrix, prior_distribution);
  }

  // This function resets the class to an initial state.
  void Channel::Reset() {
    // The prior is a uniform distribution by default.
    this->prior_distribution_.resize(this->n_in_, 1.0f/this->n_in_);
    this->out_distribution_.resize(this->n_out_, 0);

    this->max_pinput_.resize(this->n_in_, 0);

    this->max_poutput_.resize(this->n_out_, 0);

    // Important: The first index of the matrices always represents
    // the x variable.
    this->c_matrix_.assign(this->n_in_, std::vector<double>(this->n_out_, 0));
    this->h_matrix_.assign(this->n_in_, std::vector<double>(this->n_out_, 0));
    this->j_matrix_.assign(this->n_in_, std::vector<double>(this->n_out_, 0));
  }


  // This function parses a channel string.
  void Channel::ParseInput(std::string input_str) {
    std::stringstream ss;
    ss << input_str;
    
    ss >> this->n_in_;
    ss >> this->n_out_;
    ss >> this->base_norm_;


    // Initializing every class property.
    this->Reset();

    for(int i = 0; i < this->n_in_; i++)
      for(int j = 0; j < this->n_out_; j++)
        ss >> this->j_matrix_[i][j];

    for(int i = 0; i < this->n_in_; i++)
      ss >> this->prior_distribution_[i]; 

    this->build_channel(this->j_matrix_, this->prior_distribution_);
  }

  // This function returns a string that represents the
  // current channel.
  std::string Channel::to_string() const {
    // Standard output:
    // First line: Two integers separated by a space: n_in n_out
    // Second line: The base_norm_
    // Follows the j_matrix_ printed in n_in lines
    // The n_in+3 line contains the prior distribution.


    std::stringstream ss;

    ss << std::fixed << std::setprecision(8);

    ss << this->n_in_ << " " << this->n_out_ << std::endl;
    ss << this->base_norm_ << std::endl;

    for(int i = 0; i < this->n_in_; i++)
    {
      if(not this->j_matrix_[i].empty())
        ss << this->j_matrix_[i][0];
      for(int j = 1; j < this->n_out_; j++)
        ss << " " << this->j_matrix_[i][j];
      ss << std::endl;
    }

    if(not this->prior_distribution_.empty())
      ss << this->prior_distribution_[0];
    for(int i = 1; i < this->n_in_; i++)
      ss << " " << this->prior_distribution_[i];
    ss << std::endl;

    return ss.str();
  }


  // This function randomizes the current channel.
  // Maintaining the channel dimensions.
  void Channel::Randomize() {
    // RNG
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,1000);

    this->base_norm_ = 0;
    for(int i = 0; i < this->n_in_; i++) {
      for(int j = 0; j < this->n_out_; j++) {
        int neue = dist(rng);
        this->base_norm_ += neue;
        this->j_matrix_[i][j] = neue;
      }
    }
    for(int i = 0; i < this->n_in_; i++) {
      for(int j = 0; j < this->n_out_; j++) {
        this->j_matrix_[i][j] /= this->base_norm_;
      }
    }

    this->build_channel(this->j_matrix_, this->prior_distribution_);
  }

  bool Channel::CompatibleChannels(const Channel& c1, const Channel& c2) const {
    return (c1.n_in() == c2.n_in() && c1.in_names() == c2.in_names());
  }

  std::ostream& operator<< (std::ostream& stream, const Channel& channel) {
    stream << channel.to_string();
    return stream;
  }

  double Channel::ShannonEntropyPrior() const {
    double entropy = 0;
    for(int i = 0; i < this->n_in_; i++) {
      entropy += (this->prior_distribution_[i]*log2(1.0f/this->prior_distribution_[i]));
    }
    return entropy;
  }

  double Channel::ShannonEntropyOut() const {
    double entropy = 0;
    for(int i = 0; i < this->n_out_; i++) {
      entropy += (this->out_distribution_[i]*log2(1.0f/this->out_distribution_[i]));
    }
    return entropy;
  }

  // H(X|Y)
  double Channel::ConditionalEntropyHyper() const {
    double entropy = 0;
    for(int j = 0; j < this->n_out_; j++) {
      double conditional_entropy_X = 0;
      for(int i = 0; i < this->n_in_; i++) {
        conditional_entropy_X += (this->h_matrix_[i][j] * log2(1.0f/this->h_matrix_[i][j]));
      }
      entropy += (this->out_distribution_[j] * conditional_entropy_X);
    }
    return entropy;
  }


  // H(Y|X)
  double Channel::ConditionalEntropy() const {
    double entropy = 0;
    for(int i = 0; i < this->n_in_; i++) {
      double conditional_entropy_Y = 0;
      for(int j = 0; j < this->n_out_; j++) {
        conditional_entropy_Y += (this->c_matrix_[i][j] * log2(1.0f/this->c_matrix_[i][j]));
      }
      entropy += (this->prior_distribution_[i] * conditional_entropy_Y);
    }
    return entropy;

  }

  double Channel::JointEntropy() const {
    double entropy = 0;
    for(int i = 0; i < this->n_in_; i++) {
      for(int j = 0; j < this->n_out_; j++) {
        entropy += (this->j_matrix_[i][j]*log2(1.0f/this->j_matrix_[i][j]));
      }
    }
    return entropy;
  }

  double Channel::GuessingEntropy() const {
    double entropy = 0;
    std::vector<double> v = this->prior_distribution_;
    std::sort(v.begin(),v.end());
    std::reverse(v.begin(), v.end());
    for(int i = 1; i <= this->n_in_; i++)
      entropy += (i*v[i-1]);
    return entropy;
  }


  // 
  double Channel::MutualInformation() const {
    return (this->ShannonEntropyPrior() - this->ConditionalEntropy());
  }


  double Channel::NormalizedMutualInformation() const {
    return this->MutualInformation() / sqrt(this->ShannonEntropyPrior()*this->ShannonEntropyOut());
  }

  double Channel::SymmetricUncertainty() const {
    return 2*this->MutualInformation() / (this->ShannonEntropyPrior() + this->ShannonEntropyOut());
  }

  // Builds this channel from a joint matrix and a prior distribution.
  void Channel::build_channel(std::vector<std::vector<double> > j_matrix,
      std::vector<double> prior_distribution) {
    this->n_in_ = j_matrix.size();
    this->n_out_ = j_matrix[0].size();
    this->j_matrix_ = j_matrix;
    this->prior_distribution_ = prior_distribution;

    for(int i = 0; i < this->n_in_; i++) {
      for(int j = 0; j < this->n_out_; j++) {
        this->c_matrix_[i][j] = this->j_matrix_[i][j] / this->prior_distribution_[i];
        this->out_distribution_[j] += this->j_matrix_[i][j];

        this->max_pinput_[i] = std::max(this->max_pinput_[i], this->j_matrix_[i][j]);
        this->max_poutput_[j] = std::max(this->max_poutput_[j], this->j_matrix_[i][j]);
      }
    }

    for( int i=0; i<this->n_in_; i++ ) {
      for( int j=0; j<this->n_out_; j++ ) {
        this->h_matrix_[i][j] = this->j_matrix_[i][j] / this->out_distribution_[j];
      }
    }
  }

} // namespace channel
