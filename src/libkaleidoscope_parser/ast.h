#ifndef AST_H_
#define AST_H_


#include <memory>
#include <string>
#include <vector>


class ExprAST
{
  public:
    virtual ~ExprAST() {}
};


class NumberExprAST : public ExprAST
{
    double val;

  public:
    NumberExprAST(double val) : val(val) {}
};


class VariableExprAST : public ExprAST
{
    std::string name;

  public:
    VariableExprAST(const std::string &name) : name(name) {}
};


class BinaryExprAST : public ExprAST
{
    char op;
    std::unique_ptr<ExprAST> left, right;

  public:
    BinaryExprAST(char op,
                  std::unique_ptr<ExprAST> left,
                  std::unique_ptr<ExprAST> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    char get_op();
    ExprAST* get_left();
    ExprAST* get_right();
};


class CallExprAST : public ExprAST
{
    std::string function_name;
    std::vector<std::unique_ptr<ExprAST>> args;

  public:
    CallExprAST(const std::string &function_name,
                 std::vector<std::unique_ptr<ExprAST>> args)
        : function_name(function_name), args(std::move(args)) {}
};


class PrototypeAST
{
    std::string name;
    std::vector<std::string> args;

  public:
    PrototypeAST(const std::string &name,
                 std::vector<std::string> args)
        : name(name), args(std::move(args)) {}
};


class FunctionAST
{
    std::unique_ptr<PrototypeAST> prototype;
    std::unique_ptr<ExprAST> body;

  public:
    FunctionAST(std::unique_ptr<PrototypeAST> prototype,
                std::unique_ptr<ExprAST> body)
        : prototype(std::move(prototype)), body(std::move(body)) {}
};


#endif  // AST_H_
