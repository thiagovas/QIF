#include <iostream>
#include <string>
#include <list>
using namespace std;

struct tok{
  int type;
  string label;
};
// 00 = OP
// 01 = VAR
// 02 = OPEN
// 03 = CLOSE
list<struct tok> tokenize( string s ) 
{
  list<struct tok> list;
  string :: iterator it = s.begin();


  return list;
}
// cada tok de operador aponta para 2 tok de variavel


// return list de tokiens
