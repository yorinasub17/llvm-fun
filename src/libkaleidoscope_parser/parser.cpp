#include <stdio.h>
#include <memory>
#include <iostream>
#include <vector>

#include "ast.h"
#include "parser.h"
#include "libkaleidoscope_lexer/lexer.h"


static std::unique_ptr<ExprAST> log_error(const char *str)
{
    fprintf(stderr, "ERROR: %s\n", str);
    return nullptr;
}


static std::unique_ptr<PrototypeAST> log_error_prototype(const char *str)
{
    log_error(str);
    return nullptr;
}


static int is_simple_identifier(Token current_token, Token next_token)
{
    return current_token.token == tok_identifier && next_token.token != '(';
}


Token Parser::get_next_token()
{
    return GetToken(this->input);
}


std::unique_ptr<ExprAST> Parser::ParseExpression(Token token)
{
    std::unique_ptr<ExprAST> LHS = nullptr;
    Token next_token = this->get_next_token();
    if (is_simple_identifier(token, next_token))
    {
        LHS = this->ParseIdentifierExpr(token);
    }
    else if (token.token == tok_identifier)
    {
        // call expr will consume the next token, so make sure to get
        // the next token again before continuing
        LHS = this->ParseCallExpr(token, next_token);
        Token next_token = this->get_next_token();
    }
    else if (token.token == tok_number)
    {
        LHS = this->ParseNumberExpr(token);
    }
    else if (token.token == '(')
    {
        // paren expr will consume the next token, so make sure to get
        // the next token again before continuing
        LHS = this->ParseParenExpr(token, next_token);
        Token next_token = this->get_next_token();
    }
    else
    {
        return log_error("unknown token when expecting a primary expression!");
    }

    // TODO
    return LHS;
}


std::unique_ptr<ExprAST> Parser::ParseNumberExpr(Token token)
{
    if (token.token != tok_number)
        return log_error("cannot parse number: current token is not a number literal!");

    auto result = std::make_unique<NumberExprAST>(token.number);
    return std::move(result);
}


std::unique_ptr<ExprAST> Parser::ParseParenExpr(Token token, Token next_token)
{
    if (token.token != '(')
        return log_error("cannot parse parentheses expression: not an open paren!");

    // parse next token, which should be the containing
    // expression within the parentheses.
    auto value = this->ParseExpression(next_token);
    if (!value)
        return nullptr;

    // consume next token, which should be closing paren
    next_token = this->get_next_token();
    if (next_token.token != ')')
        return log_error("cannot parse parentheses expression: no closing paren!");

    return value;
}


std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr(Token current_token)
{
    if (current_token.token != tok_identifier)
        return log_error("cannot parse identifier: not an identifier!");

    return std::make_unique<VariableExprAST>(current_token.identifier);
}


std::unique_ptr<ExprAST> Parser::ParseCallExpr(Token current_token, Token next_token)
{

    if (current_token.token != tok_identifier)
        return log_error("cannot parse call expr: first token not an identifier!");
    if (next_token.token != '(')
        return log_error("cannot parse call expr: second token not an open paren!");

    // parse call args list
    std::vector<std::unique_ptr<ExprAST>> args;
    next_token = this->get_next_token();
    while (next_token.token != ')')
    {
        // FIXME!!!: Bug where ParseExpression eats up close paren
        if (auto arg = this->ParseExpression(next_token))
            args.push_back(std::move(arg));
        else
            return nullptr;

        next_token = this->get_next_token();
        if (next_token.token == ',')
            // consume next arg
            next_token = this->get_next_token();
        else
            return log_error("cannot parse function call: expected ')' or ',' in args list!");

    }

    return std::make_unique<CallExprAST>(current_token.identifier,
                                         std::move(args));
}
