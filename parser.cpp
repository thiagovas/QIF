#include "lexer.h"
#include "parser.h"
#include "define.h"
using namespace std;

int abt::getType()
{
  return 0;
}
int op0_node::getType() 
{
  return 1;
}
int op1_node::getType()
{
  return 2;
}
int op2_node::getType()
{
  return 3;
}
int var_node::getType()
{
  return 4;
}

parser::parser( queue<struct tok> q )
{
  int p = 0;
  while( !q.empty() ) {
    if( q.front().type == T_CLOSE ) {
      p++;
    }
  }
};
