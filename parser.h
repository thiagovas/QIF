// parser.h
// struct implementation
#ifndef parser_h
#define parser_h
#include <list>
#include "lexer.h"
#include "define.h"
struct node {
  int type;
  // OP
  int op_type;
  node * left;
  node * right;
  // VAR
  int var_label;
};
struct parse_tree {
  node * root;
};
struct parse_tree * build_parse_tree( std::list<struct tok> l );
#endif
