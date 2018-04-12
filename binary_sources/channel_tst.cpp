#include <iostream>
#include <cassert>
#include "../channel.h"
using namespace std;

int main()
{
  Channel c(3, 3), c2;
  c2.ParseInput(c.to_string());
  
  assert(c.to_string() == c2.to_string());
  
  cout << c.to_string() << endl;
  cout << c2.to_string() << endl;
  
  return 0;
}
