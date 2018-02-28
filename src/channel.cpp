#include "channel.h"
#include <random>
#include <iostream>
int main() {
  Channel c(2,3);
  std::cout << c << std::endl;
}

Channel::Channel(int n_in, int n_out) : n_in_(n_in), n_out_(n_out) {
  Randomize();
}

// This function parses a channel string.
void Channel::ParseInput(std::string input_str) {}

// This function returns a string that represents the
// current channel.
// std::string Channel::to_string() {}

// This function transposes the current channel.
// The input becomes the output, and the output becomes the
// input; matrix-wise we'll have p(x|y) instead of p(y|x).
void Channel::Transpose() {
//  if( this->h_matrix.capacity == 0  ) {
//    std::vector<double> p_y;
//    this->h_matrix = this->c_matrix;
//    for( int i=0; i<this->n_in_; i++ ) {
//      for( int j=0; j<this->n_out_; j++ ) {
//        this->h_matrix[i][j] = this->h_matrix[i][j] * this->prior_distribution[i];
//      }
//    }
//
//    for( int i=0; i<this->n_out_; i++ ) {
//      p_y.push_back(0.0);
//      for( int j=0; j<this->n_in_; j++ ) {
//        p_y[i] += this->h_matrix[j][i]; 
//      }
//      for( int j=0; j<this->n_in_; j++ ) {
//        this->h_matrix[j][i] /= p_y[i];
//      }
//    } 
//    
//  }
}

// This function randomizes the current channel.
// Maintaining the channel dimensions.
void Channel::Randomize() {
  // RNG
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist(0,100);
  // //
  int norm = 0;
  for( int i=0; i<this->n_in_; i++ ) {
    std::vector<double> row;
    for( int j=0; j<this->n_out_; j++ ) {
      int n = dist(rng);
      norm += n;
      row.push_back(n);
    }
    this->c_matrix.push_back(row);
  }  
  for( int i=0; i<this->n_in_; i++ ) {
    for( int j=0; j<this->n_out_; j++ ) {
      this->c_matrix[i][j] /= norm;
    }
  }  
}

std::ostream& operator<< (std::ostream& stream, const Channel& c) {
  for( int i=0; i<c.n_in_; i++ ) {
    for( int j=0; j<c.n_out_; j++ ) {
      stream << c.c_matrix[i][j] << " ";
    } 
    if( (i+1) != c.n_in_ ) 
      stream << std::endl;
  }
  return stream;
}
