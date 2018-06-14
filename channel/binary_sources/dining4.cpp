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
  Channel coin1, coin2, coin3, coin4, Id;
  Channel crypto1, crypto2, crypto3, crypto4;
  string prefix = "dc_4/";
  coin1.ParseFile(prefix + "coin1");
  coin2.ParseFile(prefix + "coin2");
  coin3.ParseFile(prefix + "coin3");
  coin4.ParseFile(prefix + "coin4");
  Id.ParseFile(prefix + "id");
  crypto1.ParseFile(prefix + "crypto1");
  crypto2.ParseFile(prefix + "crypto2");
  crypto3.ParseFile(prefix + "crypto3");
  crypto4.ParseFile(prefix + "crypto4");

  Channel coins = coin1 || coin2 || coin3 || coin4 || Id;
  Channel announce = crypto1 || crypto2 || crypto3 || crypto4;

  //cout << coins;
  //cout << announce;

  cout << coins * announce;
  return 0;
}
