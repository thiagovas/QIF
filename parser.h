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
    abt left;
    abt right;
    int type;
  private:
    int getType();
    void setupOP(int t);
};
class var_node : public abt
{
  public:
    void * channel; // Placeholder, pointer to channel
    int label;
  private:
    int getType();
    void setupVar(int label, void * channel);
};
class parser
{
  public:
    abt * root;
    list<struct tok> t;
    int b_count;
  private:
    parser( list<struct tok> q );
    abt fun( list<struct tok> q );
};
#endif
