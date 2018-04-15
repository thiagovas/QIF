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

      int ntries = 10000;
      while(ntries--) {
        while(fabs(c1.NormalizedMutualInformation() -
                    c2.NormalizedMutualInformation()) > EPS) {
          c1.Randomize();
          c2.Randomize();
        }

        if(b.LeakageMultPosterior(c1) > b.LeakageMultPosterior(c2) &&
            b.LeakageMultReversePosterior(c1) < b.LeakageMultReversePosterior(c2)) {
          cout << "OIA\n";
          cout << c1.to_string() << endl << endl;
          cout << c2.to_string() << endl;
        }
      }
    }
  }
  return 0;
}
