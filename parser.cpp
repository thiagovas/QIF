#include "lexer.h"
#include "parser.h"
#include "define.h"
using namespace std;

int abt::getType()
{
  return 0;
}
int op_node::getType() 
{
  return type;
}
void op_node::setupVar(int t)
{
  type = t;
}
int var_node::getType()
{
  return 1;
}
abt fun::parser( list<struct tok> q ) 
{
   
}
void parser::parser( list<struct tok> q )
{
  int p = 0;
  op_node r;
  b_count = 0;
  if( q.front().type == T_VAR ) {
    struct tok left_var = q.front(); q.pop_front();
    struct tok op       = q.front(); q.pop_front();
    r.left = new var_node;

    r.setupOP( op.op_type );
    r.left.setupVar(left_var.v_label,NULL);

    struct tok next = q.front();
    // Aponta pra outra OP
    if( next.type == T_OPEN ) {
      r.right = fun( q );
    }
    // Exp completa
    else if( next.type == T_VAR ) {
      r.right = new var_node;
    }
  }
  else if( q.front().type == T_OPEN ) {
    // TODO
  }
};

// Toda expressao comeca com um T_OPEN ou T_VAR
// Vai ser raiz se comecar por T_VAR
// Se comeca com T_OPEN a primeira op nao é a raiz

// parser::parser define sempre a raiz
// implementar funcoes que permitem essa abordagem
