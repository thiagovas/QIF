//  parser.h
#ifndef parser_h
#define parser_h
#include <queue>
#include "lexer.h"
class abt 
{
  private:
    virtual int getType();
    virtual void createNode(abt left, abt right);
};
class op0_node : public abt
{
  public: 
    abt * left;
    abt * right;
  private:
    int getType();
};
class op1_node : public abt
{
  public:
    abt * left;
    abt * right;
  private:
    int getType();
};
class op2_node : public abt
{
  public:
    abt * left;
    abt * right;
  private:
    int getType();
};
class var_node : public abt
{
  pubic:
    void * channel; // Placeholder, pointer to channel
    int label;
  private:
    int getType();
};
class parser
{
  abt root;
  parser( queue<struct tok> q );
};
#endif
