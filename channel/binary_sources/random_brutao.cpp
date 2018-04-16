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

using namespace std;
using namespace channel;
using namespace channel::vulnerability;

#define EPS 1e-4

int main()
{
  Bayes b;

  for(int i = 2; i < 10; i++) {
    for(int j = 2; j < 10; j++) {
      Channel c1(i, j), c2(i, j);
      Channel best_c1, best_c2;
      double best_diffa = 0, best_diffb = 0;
      
      int ntries = 100000;
      while((ntries--) > 0 || best_diffa < EPS || best_diffb < EPS) {
        while(fabs(c1.NormalizedMutualInformation() -
                    c2.NormalizedMutualInformation()) > EPS) {
          c1.Randomize();
          c2.Randomize();
        }
        
        double bLeakagePosterior_c1 = b.LeakageMultPosterior(c1);
        double bLeakagePosterior_c2 = b.LeakageMultPosterior(c2);
        
        double bLeakageRevPosterior_c1 = b.LeakageMultReversePosterior(c1);
        double bLeakageRevPosterior_c2 = b.LeakageMultReversePosterior(c2);
        
        double diff1 = bLeakagePosterior_c1 - bLeakagePosterior_c2;
        double diff2 = bLeakageRevPosterior_c2 - bLeakageRevPosterior_c1;
        
        if(bLeakagePosterior_c1 > bLeakagePosterior_c2 && 
            bLeakageRevPosterior_c1 < bLeakageRevPosterior_c2) {
          if(diff1 > best_diffa && diff2 > best_diffb) {
            best_c1.ParseInput(c1.to_string());
            best_c2.ParseInput(c2.to_string());
            best_diffa = diff1;
            best_diffb = diff2;
          }
        }
        c1.Randomize();
        c2.Randomize();
      }

      if(best_diffa > 0 && best_diffb > 0) {
        cout << "OIA: " << i << " X " << j << endl;
        cout << best_c1.to_string() << endl;
        cout << best_c2.to_string() << endl;
        cout << b.LeakageMultPosterior(best_c1) << " " << b.LeakageMultPosterior(best_c2) << endl;
        cout << b.LeakageMultReversePosterior(best_c1) << " " << b.LeakageMultReversePosterior(best_c2) << endl;

        cout << endl << endl;
      }
    }
  }
  return 0;
}
