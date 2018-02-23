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
int var_node::getType()
{
  return 1;
}
op_node::op_node( int t ) {
  type = t;
}
var_node::var_node( int l, void * c ) {
  channel = c;
  label   = l; 
}
abt * fun( list<struct tok> q ) 
{
  struct tok t = q.front(); q.pop_front(); 
  op_node * node;
  if( t.type == T_VAR ) {
    struct tok op = q.front(); q.pop_front();
    node = new op_node(op.type);
    var_node * v = new var_node(t.v_label, NULL);
    node->left = v;
    struct tok next = q.front();
    if( next.type == T_OPEN ) {
      q.pop_front();
      node->right = fun( q );
    }
    else if( next.type == T_VAR ) {
      node->right = new var_node(next.v_label, NULL);
    }
  }
  else if( t.type == T_OPEN ) {

  }
  return node;
}
parser::parser( list<struct tok> q )
{
  if( q.front().type == T_OPEN && q.back().type == T_CLOSE ) {
    q.pop_front(); q.pop_back();
  }
  op_node * r;
  if( q.front().type == T_VAR ) {
    struct tok left_var = q.front(); q.pop_front();
    struct tok op = q.front(); q.pop_front(); 
    struct tok next = q.front();

    r = new op_node( op.op_type );
    var_node * v = new var_node(left_var.v_label, NULL);
    r->left = v;
    // Aponta pra outra OP
    if( next.type == T_OPEN ) {
      q.pop_front();
      r->right = fun( q );
    }
    // Exp completa
    else if( next.type == T_VAR ) {
      r->right = new var_node(next.v_label, NULL);
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
