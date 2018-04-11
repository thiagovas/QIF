#include <iostream>
#include "../channel.h"
using namespace std;

int main()
{
  Channel c(3, 3);
  cout << c.to_string() << endl;
  return 0;
}
