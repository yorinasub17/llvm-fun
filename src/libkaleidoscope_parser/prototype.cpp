#include <stdio.h>
#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include "ast.h"
#include "parser.h"
#include "libkaleidoscope_lexer/lexer.h"


static std::unique_ptr<PrototypeAST> log_error_prototype(const char *str)
{
    fprintf(stderr, "ERROR: %s\n", str);
    return nullptr;
}


static std::unique_ptr<FunctionAST> log_error_function(const char *str)
{
    fprintf(stderr, "ERROR: %s\n", str);
    return nullptr;
}


std::unique_ptr<PrototypeAST> Parser::ParsePrototype(Token current_token)
{
    if (current_token.token != tok_identifier)
        return log_error_prototype("Expected function name in prototype");

    std::string function_name = current_token.identifier;

    // Make sure next token is a paren
    Token next_token = this->get_next_token();
    if (next_token.token != '(')
        return log_error_prototype("Expected '(' after function name in prototype"); 

    // Parse arg list
    std::vector<std::string> arg_names;
    next_token = this->get_next_token();
    while (next_token.token != ')')
    {
        if (next_token.token == tok_identifier)
            arg_names.push_back(next_token.identifier);
        else
            return log_error_prototype("Expected an identifier, ',', or ')' in arg list for prototype definition");

        next_token = this->get_next_token();
        // Skip comma
        if (next_token.token == ',')
            next_token = this->get_next_token();
    }

    return std::make_unique<PrototypeAST>(function_name, std::move(arg_names));
}


std::unique_ptr<FunctionAST> Parser::ParseDefinition(Token current_token)
{
    if (current_token.token != tok_def)
        return log_error_function("Function definition must start with def");

    current_token = this->get_next_token();
    auto prototype = this->ParsePrototype(current_token);
    if (!prototype)
        return nullptr;

    current_token = this->get_next_token();
    auto expression = this->ParseExpression(current_token);
    if (!expression)
        return nullptr;

    return std::make_unique<FunctionAST>(std::move(prototype),
                                         std::move(expression));
}


std::unique_ptr<PrototypeAST> Parser::ParseExtern(Token current_token)
{
    if (current_token.token != tok_extern)
        return log_error_prototype("Extern definition must start with extern");

    current_token = this->get_next_token();
    return this->ParsePrototype(current_token);
}


std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr(Token current_token)
{
    auto expression = this->ParseExpression(current_token);
    if (!expression)
        return nullptr;

    auto prototype = std::make_unique<PrototypeAST>("", std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(prototype), 
                                         std::move(expression));
}
