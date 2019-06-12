// In here, you can find a brute-force algorithm that randomly tries to find
// two channels c1 and c2.
//
// The intention is to find channels whose properties hold:
// 1) I_1(X;Y) = I_2(X;Y)
// 2) V_1(X|Y) < V_2(X|Y)
// 3) V_1(Y|X) > V_2(Y|X)

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

#include "../channel.h"
#include "../vulnerability/bayes.h"
#include "../vulnerability/vulnerability.h"

using namespace std;
using namespace channel;
using namespace channel::vulnerability;

#define EPS 1e-4

#define MAX_INPUT 3
#define MAX_OUTPUT 3

vector<double> prior_distribution;
Channel best_c1, best_c2;
vector<vector<int> > best_matrix_c1, best_matrix_c2;
double best_diff1, best_diff2;
int BASE_NORM = 20;


// This function gets the next vector with size vec.size(),
// and sum '\sum vec[i]', considering the lexicographical ordering.
void next_vector(vector<int>& vec) {
  // TODO: There is a way to implement this in O(1).
  //       But since I would need some auxiliary variables, this is
  //       going to be the implementation for now.
  for(int i = vec.size()-1; i >= 1; i--) {
    if(vec[i] != 0) {
      vec[i-1]++;
      int hue = vec[i]-1;
      vec[i]=0;
      vec[vec.size()-1] = hue;
      break;
    }
  }
}

// On the lexicographical ordering of vectors, this function checks
// if vec is the final one.
bool final_vector(const vector<int>& vec) {
  bool non_zero = false;
  for(unsigned i = 1; i < vec.size(); i++)
    if(vec[i] != 0) {
      non_zero = true;
      break;
    }
  return (!non_zero);
}



void evaluate(const vector<vector<int> >& c1_matrix,
               const vector<vector<int> >& c2_matrix) {
  Bayes b;
  int ninput = c1_matrix.size();
  int noutput = c1_matrix[0].size();
  vector<vector<double> > j1_matrix(ninput, vector<double>(noutput, 0));
  vector<vector<double> > j2_matrix(ninput, vector<double>(noutput, 0));
  
  
  for(int i = 0; i < c1_matrix.size(); i++) {
    for(int j = 0; j < c1_matrix[i].size(); j++) {
      j1_matrix[i][j] = ((double)c1_matrix[i][j])/BASE_NORM;
      j1_matrix[i][j] *= prior_distribution[i];
    }
  }
  
  for(int i = 0; i < c2_matrix.size(); i++) {
    for(int j = 0; j < c2_matrix[i].size(); j++) {
      j2_matrix[i][j] = ((double)c2_matrix[i][j])/BASE_NORM;
      j2_matrix[i][j] *= prior_distribution[i];
    }
  }
  
  Channel c1(j1_matrix, prior_distribution, BASE_NORM);
  Channel c2(j2_matrix, prior_distribution, BASE_NORM);
  
  if(fabs(c1.NormalizedMutualInformation() -
        c2.NormalizedMutualInformation()) > EPS)
    return;

  double bLeakagePosterior_c1 = b.LeakageMultPosterior(c1);
  double bLeakagePosterior_c2 = b.LeakageMultPosterior(c2);

  double bLeakageRevPosterior_c1 = b.LeakageMultReversePosterior(c1);
  double bLeakageRevPosterior_c2 = b.LeakageMultReversePosterior(c2);

  double diff1 = bLeakagePosterior_c1 - bLeakagePosterior_c2;
  double diff2 = bLeakageRevPosterior_c2 - bLeakageRevPosterior_c1;
  if(bLeakagePosterior_c1 > bLeakagePosterior_c2 && 
      bLeakageRevPosterior_c1 < bLeakageRevPosterior_c2) {
    if(best_diff1 < diff1 && best_diff2 < diff2) {
      best_c1 = c1;
      best_c2 = c2;
      best_diff1 = diff1;
      best_diff2 = diff2;
      best_matrix_c1 = c1_matrix;
      best_matrix_c2 = c2_matrix;
    }
  }
}

// Invariants:
// * The comparisons made here, are lexicografic comparisons. 
//
// 1) Considering just one matrix, c[i] <= c[i+1]
void brute(vector<vector<int> >& c1_matrix,
            vector<vector<int> >& c2_matrix, int line,
            bool fill2=false) {
  if(fill2 && line == c2_matrix.size()) {
    evaluate(c1_matrix, c2_matrix);
    return;
  }

  if(line == c1_matrix.size()) {
    brute(c1_matrix, c2_matrix, 0, true);
    return;
  }

  vector<vector<int> > *cur_matrix = &c1_matrix;
  if(fill2) cur_matrix = &c2_matrix;

  if(line != 0) {
    (*cur_matrix)[line] = (*cur_matrix)[line-1];
  }
  else if(fill2) {
    c2_matrix[0] = vector<int>(c2_matrix[0].size(), 0);
    c2_matrix[0][c2_matrix[0].size()-1] = BASE_NORM;
  }

  while(!final_vector((*cur_matrix)[line])) {
    brute(c1_matrix, c2_matrix, line+1, fill2);
    next_vector((*cur_matrix)[line]);
  }
  brute(c1_matrix, c2_matrix, line+1, fill2);
}

void print_matrix(const vector<vector<int> >& matrix) {
  for(int i = 0; i < matrix.size(); i++) {
    for(int j = 0; j < matrix[i].size(); j++) {
      cout << matrix[i][j] << " ";
    }
    cout << endl;
  }
}

int main() {
  Bayes b;
  for(int i = 2; i <= MAX_INPUT; i++) {
    for(int j = 2; j <= MAX_OUTPUT; j++) {
      cout << "Brute-forcing " << i << " X " << j << " ...\n" << std::flush;
      BASE_NORM = 3;
      best_diff1 = -1, best_diff2 = -1;
      best_matrix_c1 = vector<vector<int> > (i, vector<int>(j, 0));
      best_matrix_c2 = vector<vector<int> > (i, vector<int>(j, 0));
      while(++BASE_NORM <= 500) {
        vector<vector<int> > c1(i, vector<int>(j, 0));
        vector<vector<int> > c2(i, vector<int>(j, 0));
        prior_distribution = vector<double>(i, 1.0f/i);
        
        //cout << "\r" << BASE_NORM << std::flush;
        c1[0][j-1] = BASE_NORM;
        c2[0][j-1] = BASE_NORM;
        brute(c1, c2, 0);
        if(best_diff1 >= 0 && best_diff2 >= 0) {
          cout << endl;
          cout << "OIA: " << i << " X " << j << endl;
          cout << best_c1.to_string() << endl;
          print_matrix(best_matrix_c1);

          cout << best_c2.to_string() << endl;
          print_matrix(best_matrix_c2);
          
          cout << "NMI C1: " << best_c1.NormalizedMutualInformation() << endl;
          cout << "NMI C2: " << best_c2.NormalizedMutualInformation() << endl;
          cout << b.LeakageMultPosterior(best_c1) << " " <<
            b.LeakageMultPosterior(best_c2) << endl;
          cout << b.LeakageMultReversePosterior(best_c1) << " " <<
            b.LeakageMultReversePosterior(best_c2) << endl;
          cout << endl << endl;
        }
      }
      cout << endl;
    }
  }

  return 0;
}
