#include <algorithm>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <random>
#include <sstream>
#define el std::cout << std::endl;

#include "channel.h"

namespace channel {


Channel::Channel(int n_in, int n_out) : n_in_(n_in), n_out_(n_out) {
  this->Reset();

  // Should we call Identity instead?
  this->Randomize();
  this->setup_default_names();
}

Channel::Channel(std::vector<double> prior_distribution, int n_in, int n_out ) 
                  : n_in_(n_in), n_out_(n_out) {
  this->Reset();

  // Should we call Identity instead?
  this->Randomize();
  this->build_channel(this->c_matrix_, prior_distribution);
  this->setup_default_names();
}

Channel::Channel(const std::vector<std::vector<double> > & c_matrix,
                 int base_norm) {
  this->n_in_  = c_matrix.size();
  this->n_out_ = c_matrix[0].size();
  this->base_norm_ = base_norm;
  this->build_channel(c_matrix);
  this->setup_default_names();
}

Channel::Channel(const std::vector<std::vector<double> > & c_matrix,
                std::vector<double> prior_distribution,
                int base_norm) {
  this->n_in_  = c_matrix.size();
  this->n_out_ = c_matrix[0].size();
  this->base_norm_ = base_norm;
  this->build_channel(c_matrix, prior_distribution);
  this->setup_default_names();
}


// This function resets the class to an initial state.
void Channel::Reset() {
  // The prior is a uniform distribution by default.
  this->prior_distribution_.assign(this->n_in_, 1.0f/this->n_in_);
  this->out_distribution_.assign(this->n_out_, 0);

  this->max_pinput_.assign(this->n_in_, 0);

  this->max_poutput_.assign(this->n_out_, 0);

  // Important: The first index of the matrices always represents
  // the x variable.
  this->c_matrix_.assign(this->n_in_, std::vector<double>(this->n_out_, 0));
  this->h_matrix_.assign(this->n_in_, std::vector<double>(this->n_out_, 0));
  this->j_matrix_.assign(this->n_in_, std::vector<double>(this->n_out_, 0));
}


void Channel::ParseFile(std::string fname) {
  std::ifstream f(fname);
  std::string contentFile;
  std::string curr_line;
  while(std::getline(f, curr_line)) {
    contentFile += curr_line; 
    contentFile += '\n';
  }
  ParseInput(contentFile); 
  f.close();
}


// This function returns a string that represents the
// current channel.
std::string Channel::to_string() const {
  std::stringstream ss;

  ss << std::fixed << std::setprecision(8);
  
  ss << this->cname_ << std::endl;
  ss << this->n_in_ << " " << this->n_out_ << std::endl;
  ss << this->base_norm_ << std::endl;
  
  for(std::string s : this->in_names())
    ss << s << " ";
  ss << std::endl;

  for(std::string s : this->out_names())
    ss << s << " ";
  ss << std::endl;

  for(int i = 0; i < this->n_in_; i++) {
    for(int j = 0; j < this->n_out_; j++)
      ss << this->c_matrix_[i][j] << " ";
    ss << std::endl;
  }

  for(int i = 0; i < this->n_in_; i++) {
    ss << this->prior_distribution_[i] << " ";
  }
  ss << std::endl;

  return ss.str();
}


bool Channel::CompatibleChannels(const Channel& c1, const Channel& c2) {
  return (c1.n_in() == c2.n_in() && c1.in_names() == c2.in_names());
}

std::ostream& operator<< (std::ostream& stream, const Channel& channel) {
  stream << channel.to_string();
  return stream;
}

double Channel::ShannonEntropyPrior() const {
  double entropy = 0;
  for(int i = 0; i < this->n_in_; i++) {
    if(this->prior_distribution_[i] != 0)
      entropy += (this->prior_distribution_[i]*log2(1.0f/this->prior_distribution_[i]));
  }
  return entropy;
}


void Channel::build_channel(std::vector<std::vector<double> > c_matrix) {
  this->n_in_  = c_matrix.size();
  this->n_out_ = c_matrix[0].size();
  this->prior_distribution_ = std::vector<double>(this->n_in_,
                                                  1.0f/this->n_in_);
  this->build_channel(c_matrix, this->prior_distribution_);
}



void Channel::build_channel(std::vector<std::vector<double> > c_matrix,
    std::vector<double> prior_distribution) {
  this->n_in_  = c_matrix.size();
  this->n_out_ = c_matrix[0].size();
  this->Reset();
  this->c_matrix_ = c_matrix;
  this->prior_distribution_ = prior_distribution;

  
  // Filling j_matrix
  for(int i = 0; i < this->n_in_; i++) {
    for(int j = 0; j < this->n_out_; j++) {
      this->j_matrix_[i][j] = this->c_matrix_[i][j] * this->prior_distribution_[i];
    }
  }

  // Filling outdistribution, maxpinput and maxpoutput
  for(int i = 0; i < this->n_in_; i++) {
    for(int j = 0; j < this->n_out_; j++) {
      this->out_distribution_[j] += this->j_matrix_[i][j];
      this->max_pinput_[i] = std::max(this->max_pinput_[i],
                                     this->j_matrix_[i][j]);
      this->max_poutput_[j] = std::max(this->max_poutput_[j],
                                       this->j_matrix_[i][j]);
    }
  }
  
  // Filling h_matrix
  for(int i = 0; i < this->n_in_; i++) {
    for(int j = 0; j < this->n_out_; j++) {
      this->h_matrix_[i][j] = this->j_matrix_[i][j]/this->out_distribution_[j];
    }
  }
}


// Parallel Operator
Channel operator||(const Channel & c1, const Channel & c2) {
  if(!Channel::CompatibleChannels(c1,c2)) {
    std::cerr << "Channels not compatible" << std::endl;
    exit(1);
  }
  std::vector<std::string> out_names;
  std::vector<std::vector<double> > c_m(c1.n_in());

  int c3_rows = c1.n_in();
  int c3_cols = c1.n_out() * c2.n_out();

  for(int i=0; i<c3_rows; i++) {
    c_m[i].assign(c3_cols, 0.0);
  }

  std::vector<std::vector<double> > c1_c = c1.c_matrix();
  std::vector<std::vector<double> > c2_c = c2.c_matrix();

  int col_pos = 0;
  for(int i=0; i<c1.n_out(); i++) {
    for(int j=0; j<c2.n_out(); j++) {

      //std::string c1_ = c1.out_names()[i];
      //std::string c2_ = c2.out_names()[j];

      //out_names.push_back(c1_ + '.' + c2_);
      //std::cout << out_names[out_names.size()-1] << std::endl;
      for(int k=0; k<c1.n_in(); k++) {
        //std::cout << i << " " << j << " " << k << std::endl;
        c_m[k][col_pos] = c1_c[k][i] * c2_c[k][j];
      }
      col_pos++;
    }
  }
  Channel c3(c_m);
  c3.set_in_names(c1.in_names());
  //c3.set_out_names(out_names);
  return c3;
}

// Cascade
Channel operator*(const Channel& c1, const Channel& c2) {
  std::vector<std::vector<double> > new_c(c1.n_in());
  std::vector<std::vector<double> > c1_c = c1.c_matrix();
  std::vector<std::vector<double> > c2_c = c2.c_matrix();
  for(int i=0; i<c1.n_in(); i++) {
    new_c[i].resize(c2.n_out());
    for(int j=0; j<c2.n_out(); j++) {
      double sum = 0;
      for(int k=0; k<c1.n_out(); k++) sum += (c1_c[i][k] * c2_c[k][j]);
      new_c[i][j] = sum;
    }
  }
  // Is the prior of c3 always uniform?
  Channel c3(new_c);
  c3.set_in_names(c1.in_names());
  c3.set_out_names(c2.out_names());
  for(int i=0; i<(int)c2.out_names().size(); i++)
    c3.insert_out_index(c3.out_names()[i], i); 
  return c3;
}

Channel Channel::hidden_choice (const Channel& c1, const double prob,
                                       const Channel& c2) {
  std::vector<std::vector<double> > c_m(c1.n_in());
  std::vector<std::vector<double> > c1_m = c1.c_matrix();
  std::vector<std::vector<double> > c2_m = c2.c_matrix();

  std::vector<std::string> c1_n = c1.out_names();
  std::vector<std::string> c2_n = c2.out_names();

  std::vector<std::string> union_out_names;

  for(auto it : c1_n)
    union_out_names.push_back(it);
  for(auto it : c2_n)
    union_out_names.push_back(it);

  // Removing duplicates
  sort(union_out_names.begin(), union_out_names.end());
  union_out_names.erase(unique(union_out_names.begin(), union_out_names.end()), 
                        union_out_names.end());

  int union_size = union_out_names.size();
  for(int i=0; i<c1.n_in(); i++) {
    c_m[i].assign(union_size, 0);
    for(int j=0; j<union_size; j++) {
      int c1_j, c2_j;
      bool f1 = false, f2 = false;
      std::string target_out = union_out_names[j];

      if( find(c1_n.begin(), c1_n.end(), target_out) != c1_n.end() ) 
        f1 = true;
      if( find(c2_n.begin(), c2_n.end(), target_out) != c2_n.end() )
        f2 = true;

      if( f1 && f2 ) {
        c1_j = c1.out_index(union_out_names[j]);
        c2_j = c2.out_index(union_out_names[j]);
        c_m[i][j] = (prob)*(c1_m[i][c1_j]) + (1-prob)*(c2_m[i][c2_j]);
      }
      else if( f1 ) {
        c1_j = c1.out_index(union_out_names[j]);
        c_m[i][j] = (prob)*(c1_m[i][c1_j]);
      }
      else if( f2 ) {
        c2_j = c2.out_index(union_out_names[j]);
        c_m[i][j] = (1-prob)*(c2_m[i][c2_j]);
      }
      else 
        std::cout << "deu ruim no find" << std::endl;
    }
  }
  Channel c3(c_m);
  c3.set_in_names(c1_n);
  c3.set_out_names(union_out_names);
  int pos = 0;
  for(auto it : union_out_names)
    c3.insert_out_index(it, pos++);
  return c3; 
}

// This function parses a channel string.
void Channel::ParseInput(std::string input_str) {
  std::stringstream f;
  f << input_str;

  f >> this->cname_; 
  f >> this->n_in_;
  f >> this->n_out_;

  // Initializing every class property.
  this->Reset();

  std::string s;
  this->in_names_.resize(this->n_in_);
  this->out_names_.resize(this->n_out_);

  for(int i=0; i<this->n_in_; i++)  pos_in_names_[in_names_[i]] = i;
  for(int i=0; i<this->n_out_; i++) pos_out_names_[out_names_[i]] = i;

  for(int i = 0; i < this->n_in_; i++) {
    f >> this->in_names_[i];
    this->pos_in_names_[this->in_names_[i]] = i;
  }
  for(int i = 0; i < this->n_out_; i++) { 
    f >> this->out_names_[i];
    this->pos_out_names_[this->out_names_[i]] = i;
  }
  for(int i = 0; i < this->n_in_; i++)
    for(int j = 0; j < this->n_out_; j++)
      f >> this->c_matrix_[i][j];

  for(int i = 0; i < this->n_in_; i++)
    f >> this->prior_distribution_[i]; 

  this->build_channel(this->c_matrix_, this->prior_distribution_);
}

Channel Channel::visible_choice (const Channel& c1, const double prob, 
                                        const Channel& c2) {

  std::vector<std::vector<double> > c_m(c1.n_in());
  std::vector<std::string> new_output;

  for(auto it : c1.out_names())
    new_output.push_back(it);
  for(auto it : c2.out_names())
    new_output.push_back(it);

  for(int i=0; i<c1.n_in(); i++) {
    c_m[i].assign(new_output.size(), 0);
    int j = 0;
    for(auto it : c1.c_matrix()[i])
      c_m[i][j++] = prob * it;
    for(auto it : c2.c_matrix()[i])
      c_m[i][j++] = (1-prob) * it;
  }
  Channel c3(c_m);
  c3.set_in_names(c1.out_names());
  c3.set_out_names(new_output);
  for(int i=0; i<(int)new_output.size(); i++)
    c3.insert_out_index(new_output[i], i);
  return c3; 
}

// This function computes the result channel from using the
// visible conditional operator (Old visible if then else)
Channel Channel::visible_conditional(const Channel& c1, 
                                     std::vector<std::string> &A, 
                                     const Channel& c2) {

  std::vector<std::vector<double> > c_m(c1.n_in());

  std::vector<std::string> new_output(c1.out_names());
  for(auto it : c2.out_names())
    new_output.push_back(it);

  std::vector<std::string> input_names = c1.in_names();

  for(int i=0; i<c1.n_in(); i++) {
    std::string x_name = input_names[i];
    c_m[i].assign(c1.n_out() + c2.n_out(), 0);
    auto it = std::find(A.begin(), A.end(), x_name);
    if(*it == x_name) 
      for(int j=0; j<c1.n_out(); j++) 
        c_m[i][j] = c1.c_matrix()[i][j];
    else 
      for(int j=0; j<c2.n_out(); j++) 
        c_m[i][c1.n_out() + j] = c2.c_matrix()[i][j];
  }
  Channel c3(c_m);
  c3.set_in_names(input_names);
  c3.set_out_names(new_output);
  for(int i=0; i<(int)new_output.size(); i++)
    c3.insert_out_index(new_output[i], i);
  return c3; 
}

Channel Channel::hidden_conditional (const Channel& c1,
                                    std::vector<std::string> &A,
                                    const Channel& c2) {

  std::vector<std::vector<double> > c_m(c1.n_in());
  std::vector<std::vector<double> > c1_m = c1.c_matrix();
  std::vector<std::vector<double> > c2_m = c2.c_matrix();

  std::vector<std::string> c1_out = c1.out_names();
  std::vector<std::string> c2_out = c2.out_names();

  std::vector<std::string> union_out_names(c1_out);
  union_out_names.insert(union_out_names.end(), c2_out.begin(), c2_out.end());

  // Removing duplicates
  sort(union_out_names.begin(), union_out_names.end());
  union_out_names.erase( unique(union_out_names.begin(), union_out_names.end()), 
                         union_out_names.end());

  std::vector<std::string> input_names = c1.in_names();
  int new_output_size = union_out_names.size();
  for(int i=0; i<input_names.size(); i++) {
    std::string x_name = input_names[i];
    c_m[i].assign(new_output_size, 0);
    auto it = std::find(A.begin(), A.end(), x_name);
    // C1
    if(*it == x_name) {
      int c3_output_pos = 0;
      int c1_output_pos = 0;
      while(c1_output_pos < c1.n_out()) {
        if(c1_out[c1_output_pos] == union_out_names[c3_output_pos]) {
          c_m[i][c3_output_pos] = c1_m[i][c1_output_pos];
          c1_output_pos++;
        }
        c3_output_pos++;
      }
    }
    // C2
    else {
      int c3_output_pos = 0;
      int c2_output_pos = 0;
      while(c2_output_pos < c2.n_out()) {
        if(c2_out[c2_output_pos] == union_out_names[c3_output_pos]) {
          c_m[i][c3_output_pos] = c2_m[i][c2_output_pos];
          c2_output_pos++;
        }
        c3_output_pos++;
      }
    }
  }

  Channel c3(c_m);
  c3.set_in_names(input_names);
  c3.set_out_names(union_out_names);
  for(int i=0; i<(int)union_out_names.size(); i++)
    c3.insert_out_index(union_out_names[i], i);
  return c3; 
}


// This function randomizes the current channel.
// Maintaining the channel dimensions.
void Channel::Randomize() {
  // RNG
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist(0,1000);

  this->base_norm_ = 0;
  this->prior_distribution_.assign(this->n_in_, 0);
  for(int i = 0; i < this->n_in_; i++) {
    for(int j = 0; j < this->n_out_; j++) {
      int neue = dist(rng);
      this->base_norm_ += neue;
      this->c_matrix_[i][j] = neue;
      this->prior_distribution_[i] += neue;
    }
  }

  for(int i = 0; i < this->n_in_; i++) {
    for(int j = 0; j < this->n_out_; j++) {
      if(this->prior_distribution_[i] != 0) {
        this->c_matrix_[i][j] /= this->prior_distribution_[i];
      }
      else {
        this->c_matrix_[i][j] = 0;
      }
    }
    this->prior_distribution_[i] /= this->base_norm_;
  }

  this->build_channel(this->c_matrix_, this->prior_distribution_);
}
////////////////
// We now define some compositional bounds
////////////////
// Upper and Lower bounds
std::pair<double, double> 
  Channel::parallel_vulnerability(const Channel& c1, const Channel& c2,
                                  std::vector<double> prior,
                                  std::vector<std::vector<double>> &g) {

  double lower, upper;
  double upper_c1 = 0.0, upper_c2 = 0.0;

  // Upper Bound first term
  for(int y=0; y<c2.n_out(); y++) {
    double max_ = 0.0;
    for(int x=0; x<c2.n_in(); x++) {
      double check = 0.0;
      for(int w=0; w<g.size(); w++)
        check += g[w][x];
      if(check == 0.0)
        continue;
      max_ = std::max(max_, c2.c_matrix()[x][y]);
    }
    upper_c1 += max_;
  }
  upper_c1 *= c1.PostGVun(prior, g);
  
  // Upper Bound second term
  for(int y=0; y<c1.n_out(); y++) {
    double max_ = 0.0;
    for(int x=0; x<c1.n_in(); x++) {
      double check = 0.0;
      for(int w=0; w<g.size(); w++)
        check += g[w][x];
      if(check == 0.0)
        continue;
      max_ = std::max(max_, c1.c_matrix()[x][y]);
    }
    upper_c2 += max_;
  }
  upper_c2 *= c2.PostGVun(prior, g);

  lower = std::max(c1.PostGVun(prior, g), c2.PostGVun(prior, g));
  upper = std::min(upper_c1, upper_c2);

  return std::pair<double, double>(lower, upper);
}

std::pair<double, double> 
  Channel::hidden_choice_vulnerability(const Channel& c1,
                              const Channel& c2,
                              const double prob,
                              std::vector<std::vector<double>> &g) {
  double lower, upper;
  lower = std::max( prob*c1.PostGVun(g), (1-prob)*c2.PostGVun(g) );
  upper = prob*c1.PostGVun(g) + (1 - prob)*c2.PostGVun(g);
  return std::pair<double, double>(lower, upper);
}
std::pair<double, double>
  Channel::hidden_conditional_vulnerability(const Channel& c1, 
                                            const Channel& c2,
                                            const std::vector<std::string> &A,
                                            const std::vector<double> &prior,
                                            const std::vector<std::vector<double>> &g) {
  double lower = 0.0, upper = 0.0;
  std::set<std::string> not_A;
  for(auto it : c1.in_names())
    not_A.insert(it);
  for(auto it : c2.in_names())
    not_A.insert(it);
  for(auto it : A)
    not_A.erase(it);
  std::vector<std::vector<double>> A_g(g), not_A_g(g);
  for(auto it : A) {
    int pos = c1.in_index(it);
    for(int i=0; i<A_g[pos].size(); i++)
      A_g[pos][i] = 0.0;
  }
  for(auto it : not_A) {
    int pos = c2.in_index(it);
    for(int i=0; i<not_A_g[pos].size(); i++)
      not_A_g[pos][i] = 0.0;
  }
  lower = std::max(c1.PostGVun(prior, A_g), c2.PostGVun(prior, not_A_g)); 
  upper = c1.PostGVun(prior, A_g) + c2.PostGVun(prior, not_A_g);
  return std::pair<double, double>(lower, upper);
}
// Linear Bounds
double 
  Channel::visible_choice_vulnerability(const Channel& c1,
                                        const Channel& c2,
                                        const double prob,
                                        std::vector<std::vector<double>> &g) {
    return ((prob * c1.PostGVun(g)) + ((1.0 - prob) * c2.PostGVun(g)));
}

//double 
//  Channel::visible_conditional (const Channel& c1,
//                                const Channel& c2,
//                                const std::vector<std::string> &A) {
//    
//}

////////////////
// We now define some metrics
////////////////
double Channel::ShannonEntropyOut() const {
  double entropy = 0;
  for(int i = 0; i < this->n_out_; i++) {
    if(this->out_distribution_[i] != 0)
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
      if(this->h_matrix_[i][j] != 0)
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
      if(this->c_matrix_[i][j] != 0)
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
      if(this->j_matrix_[i][j] != 0)
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
  return (this->ShannonEntropyPrior() - this->ConditionalEntropyHyper());
}

double Channel::NormalizedMutualInformation() const {
  return this->MutualInformation() / sqrt(this->ShannonEntropyPrior()*this->ShannonEntropyOut());
}

double Channel::SymmetricUncertainty() const {
  return 2*this->MutualInformation() / (this->ShannonEntropyPrior() + this->ShannonEntropyOut());
}

double Channel::PriorGVun(std::vector<std::vector<double> > g) const {
	double max_ = 0;
	for(int w_i = 0; w_i<(int)g.size(); w_i++) {
		double max_w = 0;
		for(int x_i = 0; x_i<this->n_in(); x_i++) {
			max_w += this->prior_distribution()[x_i] * g[w_i][x_i];
		}
		max_ = std::max(max_, max_w);
	}
	return max_;
}

double Channel::PostGVun(const std::vector<std::vector<double> > &g) const {
	double sum_ = 0;
	for(int y_i = 0; y_i<this->n_out(); y_i++) {
		double max_w = 0;
		for(int w_i = 0; w_i<(int)g.size(); w_i++) {
			double new_max_w = 0;
			for(int x_i = 0; x_i<this->n_in(); x_i++) {
				new_max_w += this->prior_distribution()[x_i] * 
					this->c_matrix()[x_i][y_i] * g[w_i][x_i];
			}
			max_w = std::max(max_w, new_max_w);
		}
		sum_ += max_w;
	}
	return sum_;
}

double Channel::PostGVun(const std::vector<double> &prior_distribution,
                         const std::vector<std::vector<double> > &g) const {
	double sum_ = 0;
	for(int y_i = 0; y_i<this->n_out(); y_i++) {
		double max_w = 0;
		for(int w_i = 0; w_i<(int)g.size(); w_i++) {
			double new_max_w = 0;
			for(int x_i = 0; x_i<this->n_in(); x_i++) {
				new_max_w += prior_distribution[x_i] * 
					           this->c_matrix()[x_i][y_i] * g[w_i][x_i];
			}
			max_w = std::max(max_w, new_max_w);
		}
		sum_ += max_w;
	}
	return sum_;
}

void Channel::setup_default_names() {
  int input_size = this->n_in(), output_size = this->n_out();
  std::vector<std::string> input, output;

  for(int i=0; i<input_size; i++)
    input.push_back("x" + std::to_string(i));

  for(int i=0; i<output_size; i++) {
    std::string num = std::to_string(i);

    while(num.size() < std::to_string(output_size).size())
      num = "0" + num;

    output.push_back("y" + num);
  }

  this->set_in_names(input);
  this->set_out_names(output);

  this->setup_in_out_map();
}

void Channel::setup_in_out_map() {
  int input_size = this->n_in(), output_size = this->n_out();
  for(int i=0; i<input_size; i++)
    this->insert_in_index(this->in_names()[i], i);
  for(int i=0; i<output_size; i++) 
    this->insert_out_index(this->out_names()[i], i);
}
}  // namespace channel
