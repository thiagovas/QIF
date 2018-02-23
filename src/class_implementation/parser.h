//  parser.h
#ifndef parser_h
#define parser_h
#include <list>
#include "lexer.h"
class abt 
{
  private:
    virtual int getType();
};
class op_node : public abt
{
  public:
    abt * left;
    abt * right;
    int type;
    op_node(int t);
  private:
    int getType();
};
class var_node : public abt
{
  public:
    void * channel; // Placeholder, pointer to channel
    int label;
    var_node(int label, void * channel);
  private:
    int getType();
};
abt * fun( std::list<struct tok> q );
class parser
{
  public:
    parser( std::list<struct tok> q );
    abt * root;
    std::list<struct tok> t;
    int b_count;
};
#endif
