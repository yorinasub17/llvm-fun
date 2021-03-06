#ifndef PARSER_H_
#define PARSER_H_


#include <iostream>
#include <memory>
#include <deque>

#include "ast.h"
#include "libkaleidoscope_lexer/lexer.h"


class Parser
{
    std::istream &input;
    std::deque<Token> buffer = std::deque<Token>();

  public:
    // Constructors
    Parser(std::istream &input) : input(input) {}

    // API
    std::unique_ptr<ExprAST> ParseExpression();
    std::unique_ptr<ExprAST> ParseExpression(Token token);
    std::unique_ptr<PrototypeAST> ParsePrototype();

    // Test function
    void Driver();

  private:
    // Helper
    Token get_next_token();
    void return_token(Token token);

    // Expression parsing methods
    std::unique_ptr<ExprAST> ParsePrimaryExpr(Token current_token);
    std::unique_ptr<ExprAST> ParseBinOpRHS(int expression_precedence, Token current_token,
                                           std::unique_ptr<ExprAST> LHS);
    std::unique_ptr<ExprAST> ParseNumberExpr(Token token);
    std::unique_ptr<ExprAST> ParseParenExpr(Token token, Token next_token);
    std::unique_ptr<ExprAST> ParseIdentifierExpr(Token current_token);
    std::unique_ptr<ExprAST> ParseCallExpr(Token current_token, Token next_token);

    // Protoype parsing methods

    std::unique_ptr<PrototypeAST> ParsePrototype(Token current_token);
    std::unique_ptr<FunctionAST> ParseDefinition(Token current_token);
    std::unique_ptr<PrototypeAST> ParseExtern(Token current_token);
    std::unique_ptr<FunctionAST> ParseTopLevelExpr(Token current_token);
};


int GetOperatorPrecedence(Token token);


#endif  // PARSER_H_
