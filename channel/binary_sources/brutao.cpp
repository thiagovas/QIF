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
  Channel c1(2, 3), c2(2, 3);
  
  while(true) {
    while(fabs(c1.NormalizedMutualInformation()-c2.NormalizedMutualInformation()) > EPS) {
      c1.Randomize();
      c2.Randomize();
    }
    
    if(b.LeakageMultPosterior(c1) > b.LeakageMultPosterior(c2) &&
       b.LeakageMultReversePosterior(c1) < b.LeakageMultReversePosterior(c2))
    {
      cout << "OIA\n";
      cout << c1.to_string() << endl << endl;
      cout << c2.to_string() << endl;
    }
  }
  
  return 0;
}
