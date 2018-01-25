#include <iostream>
#include <string>
#include <algorithm>
#include <list>

struct tok
{
  int type;
  //0 = OP
  //1 = VAR
  //2 = OPEN
  //3 = CLOSE
  int op_type;
  //0
  //1
  //2
  int v_label;
};

list<struct tok> tokenize( char * in );
