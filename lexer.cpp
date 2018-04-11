#include "lexer.h"
#include "define.h"
using namespace std;

list<struct tok> * tokenize( char * in ) 
{
  char * s = in;
  string str;
  // rm whitespace
  while( *s != '\0' ) {
    if( *s != ' ' )
      str.push_back(*s);
    s++;
  }
  cout << str << endl;
  list<struct tok> * l = new list<struct tok>;
  for( int i=0; i<str.length(); i++ ) {
    struct tok t;
    if( str[i] == '(' ) {
      t.type = T_OPEN; 
    }
    else if( str[i] == ')' ) {
      t.type = T_CLOSE;
    }
    else if( str[i] == 'O' && str[i+1] == 'P' ) {
      t.type = T_OP; 
      t.op_type = ((int)str[i+2]) - 48;
      i+=2;
    }
    else if( str[i] == 'v' ) {
      t.type = T_VAR;
      t.v_label = ((int)str[i+1]) - 48;
      i++;
    }
    else {
      cout << "Lexer ERROR" << endl;
      exit(0);
    }
    l->push_back(t);
  }
  return l;
}
// cada tok de operador aponta para 2 tok de variavel
// return list de tokiens