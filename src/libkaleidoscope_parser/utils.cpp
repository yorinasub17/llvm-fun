#include "parser.h"
#include "libkaleidoscope_lexer/lexer.h"


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


void Parser::Driver()
{
    while (1)
    {
        Token current_token = this->get_next_token();
        switch (current_token.token)
        {
            case tok_eof:
                return;
            case ';':
                break;
            case tok_def:
                this->ParseDefinition(current_token);
                break;
            case tok_extern:
                this->ParseExtern(current_token);
                break;
            default:
                this->ParseTopLevelExpr(current_token);
                break;
        }
    }
}
