#include <iostream>
#include "lexer.h"
#include "parser.h"
using namespace std;
int main()
{
  char input[100];
  parser p( tokenize(input) );
  
  return 0;
}
