#include <iostream>
#include "lexer.h"
#include "parser.h"
using namespace std;
int main()
{
  char input[100];
  cin.getline(input,100);
  // Class implementation
  // parser p( tokenize(input) );
  
  // Struct implementation
  struct parse_tree * p = build_parse_tree( tokenize(input) );
  
  return 0;
}
