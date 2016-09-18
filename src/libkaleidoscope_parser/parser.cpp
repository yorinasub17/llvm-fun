#include <stdio.h>
#include <memory>
#include <iostream>
#include <vector>
#include <map>

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


static std::map<char, int> BinaryOpPrecedence = { {'<', 10},
                                                  {'>', 10},
                                                  {'+', 20},
                                                  {'-', 20},
                                                  {'*', 40},
                                                  {'/', 40}
                                                };
int GetOperatorPrecedence(Token token)
{
    // Token must be char token as operator
    if (!isascii(token.token))
        return -1;

    int token_precedence = BinaryOpPrecedence[token.token];
    // Check to make sure token is a known binary operator
    if (token_precedence <= 0)
        return -1;
    return token_precedence;
}


Token Parser::get_next_token()
{
    // Get directly from input if buffer is empty
    if (this->buffer.empty())
    {
        return GetToken(this->input);
    }
    // Otherwise get from buffer and consume it
    else
    {
        auto token = buffer.front();
        this->buffer.pop_front();
        return token;
    }
}


// Returns consumed token back to front of buffer to be consumed
// again.
void Parser::return_token(Token token)
{
    this->buffer.push_front(token);
}


std::unique_ptr<ExprAST> Parser::ParseExpression()
{
    Token token = this->get_next_token();
    return this->ParseExpression(token);
}


std::unique_ptr<ExprAST> Parser::ParseExpression(Token current_token)
{
    std::unique_ptr<ExprAST> LHS = this->ParsePrimaryExpr(current_token);
    Token next_token = this->get_next_token();
    return ParseBinOpRHS(0, next_token, std::move(LHS));
}


std::unique_ptr<ExprAST> Parser::ParsePrimaryExpr(Token current_token)
{
    std::unique_ptr<ExprAST> LHS = nullptr;
    Token next_token = this->get_next_token();
    if (is_simple_identifier(current_token, next_token))
    {
        // does not consume the next token, so return it in the pipe
        LHS = this->ParseIdentifierExpr(current_token);
        this->return_token(next_token);
    }
    else if (current_token.token == tok_identifier)
    {
        // call expression will consume the next token
        LHS = this->ParseCallExpr(current_token, next_token);
    }
    else if (current_token.token == tok_number)
    {
        // does not consume the next token, so return it in the pipe
        LHS = this->ParseNumberExpr(current_token);
        this->return_token(next_token);
    }
    else if (current_token.token == '(')
    {
        // paren expr will consume the next token
        LHS = this->ParseParenExpr(current_token, next_token);
    }
    else
    {
        return log_error("unknown token when expecting a primary expression!");
    }

    if (!LHS)
        return nullptr;

    return LHS;
}


std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int expression_precedence,
                                               Token current_token,
                                               std::unique_ptr<ExprAST> LHS)
{
    while (1)
    {
        int current_token_precedence = GetOperatorPrecedence(current_token);
        int next_token_precedence = 0;

        // We are done if the current token precedence is less than the current
        // operator precedence.
        if (current_token_precedence < expression_precedence)
        {
            // Since we consumed the token, return it back into the stream for
            // later parsing.
            this->return_token(current_token);
            return LHS;
        }

        // The current token is a binary operator with precedence, so parse it
        // with the next primary expr
        int binary_operator = current_token.token;
        Token next_token = this->get_next_token();
        auto RHS = this->ParsePrimaryExpr(next_token);
        if (!RHS)
            return nullptr;

        // Peek into the next token to determine operator precedence
        next_token = this->get_next_token();
        next_token_precedence = GetOperatorPrecedence(next_token);

        // If the next operator has precedence, parse up the right side
        if (current_token_precedence < next_token_precedence)
        {
            RHS = this->ParseBinOpRHS(current_token_precedence+1, next_token, std::move(RHS));
            if (!RHS)
                return nullptr;
            // Don't forget to update the token
            current_token = this->get_next_token();
        }
        // Otherwise, advance the token
        else
        {
            current_token = next_token;
        }
        LHS = std::make_unique<BinaryExprAST>(binary_operator,
                                              std::move(LHS),
                                              std::move(RHS));
    }
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

    // parse next token which should be the containing
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
        if (auto arg = this->ParseExpression(next_token))
            args.push_back(std::move(arg));
        else
            return nullptr;

        next_token = this->get_next_token();
        if (next_token.token == ',')
            // consume next arg
            next_token = this->get_next_token();
        else if (next_token.token != ')')
            return log_error("cannot parse function call: expected ')' or ',' in args list!");

    }

    return std::make_unique<CallExprAST>(current_token.identifier,
                                         std::move(args));
}
