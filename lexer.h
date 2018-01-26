// lexer.h
#ifndef lexer_h
#define lexer_h
#include <iostream>
#include <string>
#include <algorithm>
#include <queue>

struct tok
{
  int type;
  //0 = OP
  //1 = VAR
  //2 = OPEN
  //3 = CLOSE
  //4 = NODE
  int op_type;
  //0
  //1
  //2
  int v_label;
};

queue<struct tok> tokenize( char * in );

#endif
