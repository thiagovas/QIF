#include <iostream>
#include <queue>
#include "lexer.h"
#include "parser.h"
using namespace std;
void print_parse_tree( struct parse_tree * p )
{
  queue<struct node *> q;
  q.push(p->root);
  while( !q.empty() ) {

  }
}
int main()
{
  char input[100];
  cin.getline(input,100);
  // Class implementation
  // parser p( tokenize(input) );
  
  // Struct implementation
  struct parse_tree * p = build_parse_tree( tokenize(input) );
  print_parse_tree(p);
  return 0;
}
