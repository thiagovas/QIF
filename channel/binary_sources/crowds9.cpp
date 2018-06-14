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

  
int main() {
  Channel id, is, pd, ps;
  double p = 0.6;
  double q = 0.34;
  id.ParseFile ("crowds_9/id"); 
  is.ParseFile ("crowds_9/is");
  pd.ParseFile ("crowds_9/pd");
  ps.ParseFile ("crowds_9/ps");
  //cout << id << is << pd << ps;
  //cout << id << std::endl << is*ps;
  Channel x1 = Channel::hidden_choice( id*pd, is*(ps*ps), q );
  cout << "X1" << endl;
  cout << x1;

  Channel x2 = Channel::hidden_choice( is*ps, x1, p );
  cout << "X2" << endl;
  cout << x2;

  Channel x3 = Channel::hidden_choice( id, x2, q);
  cout << "X3" << endl;
  cout << x3;

  return 0;
}
