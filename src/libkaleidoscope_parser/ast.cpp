#include "ast.h"

char BinaryExprAST::get_op()
{
    return this->op;
}


ExprAST* BinaryExprAST::get_left()
{
    return this->left.get();
}


ExprAST* BinaryExprAST::get_right()
{
    return this->right.get();
}
