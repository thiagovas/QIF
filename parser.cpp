#include "parser.h"
using namespace std;
#define N_VAR 0
#define N_OP  1
struct node * setup_var( int l ) {
  struct node * n = new node;
  n->type = N_VAR;
  n->var_label = l;
  return n;
}
struct node * setup_op( int op_t ) {
  struct node * n = new node;
  n->type = N_OP;
  n->op_type = op_t;
  return n;
}
struct node * rec_build( list<struct tok> l ) 
{
  struct node * n;
  if( l.front().type == T_VAR ) {
    struct tok t_left = l.front(); l.pop_front();
    struct tok t_op   = l.front(); l.pop_front();
    struct tok t_next = l.front(); l.pop_front();
    n = setup_op( t_op.type );
    n->left = setup_var( t_left.v_label );
    if( t_next.type == T_VAR ) {
      n->right = setup_var( t_next.v_label );
    }
    else if ( t_next.type == T_OPEN ) {
      n->right = rec_build( l );
    }
  }
  else if( l.front().type == T_OPEN ) {
    
  }
  else {
    cout << "UNDEFINED BEHAVIOUR: rec_build" << endl;
  }
  return n;
}
struct parse_tree * build_parse_tree( list<struct tok> l )
{
  struct parse_tree * p = new struct parse_tree; 
  if( l.front().type == T_VAR ) {
    struct tok t_left = l.front(); l.pop_front();
    struct tok t_op   = l.front(); l.pop_front();
    struct tok t_next = l.front(); l.pop_front();
    p->root = setup_op( t_op.type );
    p->root->left = setup_var( t_left.v_label );
    if( t_next.type == T_VAR ) {
      p->root->right = setup_var( t_next.v_label);
    }
    else if( t_next.type == T_OPEN ) {
      // TODO
      p->root->right = rec_build( l );
    }
  }
  else if( l.front().type == T_OPEN ) {
    // Comeca com uma exp
  }
  else {
    cout << "UNDEFINED BEHAVIOUR: build_parse_tree" << endl;
  }
  return p;
}
