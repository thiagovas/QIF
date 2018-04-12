#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <random>
#include <sstream>

#include "channel.h"

Channel::Channel(int n_in, int n_out) : n_in_(n_in), n_out_(n_out) {
  this->Reset();
  this->Randomize();
}


// This function resets the class to an initial state.
void Channel::Reset() {
  // The prior is a uniform distribution by default.
  this->prior_distribution.resize(this->n_in_, 1.0f/this->n_in_);
  this->out_distribution.resize(this->n_out_, 0);

  this->max_pinput.resize(this->n_in_, 0);

  this->max_poutput.resize(this->n_out_, 0);
  
  // Important: The first index of the matrices always represents
  // the x variable.
  this->c_matrix.resize(this->n_in_, std::vector<double>(this->n_out_, 0));
  this->h_matrix.resize(this->n_in_, std::vector<double>(this->n_out_, 0));
  this->j_matrix.resize(this->n_in_, std::vector<double>(this->n_out_, 0));
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
  
  for(unsigned i = 0; i < this->j_matrix.size(); i++)
    for(unsigned j = 0; j < this->j_matrix.size(); j++)
      ss >> this->j_matrix[i][j];
  
  for(unsigned i = 0; i < this->prior_distribution.size(); i++)
    ss >> this->prior_distribution[i]; 
  
  for( int i=0; i<this->n_in_; i++ ) {
    for( int j=0; j<this->n_out_; j++ ) {
      this->c_matrix[i][j] = this->j_matrix[i][j] / this->prior_distribution[i];
      this->out_distribution[j] += this->j_matrix[i][j];

      this->max_pinput[i] = std::max(this->max_pinput[i], this->j_matrix[i][j]);
      this->max_poutput[j] = std::max(this->max_poutput[j], this->j_matrix[i][j]);
    }
  }

  for( int i=0; i<this->n_in_; i++ ) {
    for( int j=0; j<this->n_out_; j++ ) {
      this->h_matrix[i][j] = this->j_matrix[i][j] / this->out_distribution[j];
    }
  }
}

// This function returns a string that represents the
// current channel.
std::string Channel::to_string() const {
  // Standard output:
  // First line: Two integers separated by a space: n_in n_out
  // Second line: The base_norm_
  // Follows the j_matrix printed in n_in lines
  // The n_in+3 line contains the prior distribution.
  
  
  std::stringstream ss;
  
  ss << std::fixed << std::setprecision(8);

  ss << this->n_in_ << " " << this->n_out_ << std::endl;
  ss << this->base_norm_ << std::endl;

  for(unsigned i = 0; i < this->j_matrix.size(); i++)
  {
    if(not this->j_matrix[i].empty())
      ss << this->j_matrix[i][0];
    for(unsigned j = 1; j < this->j_matrix.size(); j++)
      ss << " " << this->j_matrix[i][j];
    ss << std::endl;
  }
  
  if(not this->prior_distribution.empty())
    ss << this->prior_distribution[0];
  for(unsigned i = 1; i < this->prior_distribution.size(); i++)
    ss << " " << this->prior_distribution[i];
  ss << std::endl;

  return ss.str();
}


// This function randomizes the current channel.
// Maintaining the channel dimensions.
void Channel::Randomize() {
  // RNG
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist(0,100);
  
  this->base_norm_ = 0;
  for( int i=0; i<this->n_in_; i++ ) {
    for( int j=0; j<this->n_out_; j++ ) {
      int neue = dist(rng);
      this->base_norm_ += neue;
      this->j_matrix[i][j] = neue;
    }
  }
  for( int i=0; i<this->n_in_; i++ ) {
    for( int j=0; j<this->n_out_; j++ ) {
      this->j_matrix[i][j] /= this->base_norm_;
      this->c_matrix[i][j] = this->j_matrix[i][j] / this->prior_distribution[i];
      this->out_distribution[j] += this->j_matrix[i][j];

      this->max_pinput[i] = std::max(this->max_pinput[i], this->j_matrix[i][j]);
      this->max_poutput[j] = std::max(this->max_poutput[j], this->j_matrix[i][j]);
    }
  }

  for( int i=0; i<this->n_in_; i++ ) {
    for( int j=0; j<this->n_out_; j++ ) {
      this->h_matrix[i][j] = this->j_matrix[i][j] / this->out_distribution[j];
    }
  }
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
    entropy += (this->prior_distribution[i]*log2(1.0f/this->prior_distribution[i]));
  }
  return entropy;
}

double Channel::ShannonEntropyOut() const {
  double entropy = 0;
  for(int i = 0; i < this->n_out_; i++) {
    entropy += (this->out_distribution[i]*log2(1.0f/this->out_distribution[i]));
  }
  return entropy;
}

// H(X|Y)
double Channel::ConditionalEntropyHyper() const {
  double entropy = 0;
  for(int j = 0; j < this->n_out_; j++) {
    double conditional_entropy_X = 0;
    for(int i = 0; i < this->n_in_; i++) {
      conditional_entropy_X += (this->h_matrix[i][j] * log2(1.0f/this->h_matrix[i][j]));
    }
    entropy += (this->out_distribution[j] * conditional_entropy_X);
  }
  return entropy;
}


// H(Y|X)
double Channel::ConditionalEntropy() const {
  double entropy = 0;
  for(int i = 0; i < this->n_in_; i++) {
    double conditional_entropy_Y = 0;
    for(int j = 0; j < this->n_out_; j++) {
      conditional_entropy_Y += (this->c_matrix[i][j] * log2(1.0f/this->c_matrix[i][j]));
    }
    entropy += (this->prior_distribution[i] * conditional_entropy_Y);
  }
  return entropy;
  
}

double Channel::JointEntropy() const {
  double entropy = 0;
  for(int i = 0; i < this->n_in_; i++) {
    for(int j = 0; j < this->n_out_; j++) {
      entropy += (this->j_matrix[i][j]*log2(1.0f/this->j_matrix[i][j]));
    }
  }
  return entropy;
}

double Channel::GuessingEntropy() const {
  double entropy = 0;
  std::vector<double> v = this->prior_distribution;
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

double Channel::BayesVulnerabilityPrior() const {
  double vulnerability = 0;
  for(int i = 0; i < this->n_in_; i++) {
    vulnerability = std::max(vulnerability, this->prior_distribution[i]);
  }
  return vulnerability;
}
    
double Channel::BayesVulnerabilityOut() const {
  double vulnerability = 0;
  for(int i = 0; i < this->n_out_; i++) {
    vulnerability = std::max(vulnerability, this->out_distribution[i]);
  }
  return vulnerability;
}

double Channel::BayesVulnerabilityPosterior() const {
  double vulnerability = 0;
  for(int i = 0; i < this->n_out_; i++) {
    vulnerability += this->max_poutput[i];
  }
  return vulnerability;
}

double Channel::BayesVulnerabilityReversePosterior() const {
  double vulnerability = 0;
  for(int i = 0; i < this->n_in_; i++) {
    vulnerability += this->max_pinput[i];
  }
  return vulnerability;
}
    

double Channel::BayesLeakageMultPosterior() const {
  return this->BayesVulnerabilityPosterior() / this->BayesVulnerabilityPrior();
}

double Channel::BayesLeakageMultReversePosterior() const {
  return this->BayesVulnerabilityPosterior() / this->BayesVulnerabilityOut();
}

double Channel::BayesLeakageAddPosterior() const {
  return this->BayesVulnerabilityPosterior() - this->BayesVulnerabilityPrior();

}

double Channel::BayesLeakageAddReversePosterior() const {
  return this->BayesVulnerabilityPosterior() - this->BayesVulnerabilityOut();
}
