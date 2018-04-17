#include <iostream>
#include "../../channel/channel.h"
#include "../distribution.h"
int main() {
  base::Distribution d1(5); 
  std::cout << d1.GuessingEntropy() << std::endl;
  return 0;
}
