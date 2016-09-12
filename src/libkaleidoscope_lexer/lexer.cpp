#include <limits>
#include <stdio.h>
#include <ctype.h>

#include "lexer.h"


// Advances pointer while predicate condition for character is true, returning
// the combined characters as string.
static std::string ConsumeWhileCondition(std::istream& input,
                                         int *current_character,
                                         int (*character_predicate)(int))
{
    std::string characters("");

    // start with current value
    characters += *current_character;

    // Keep consuming while predicate is true
    while ((*character_predicate)(*current_character = input.get()))
        characters += *current_character;

    return characters;
}


// Lookup token value based on content
static int LookupToken(std::string token)
{
    if (token == "def")
        return tok_def;
    else if (token == "extern")
        return tok_extern;
    else if (token == "if")
        return tok_if;
    else if (token == "then")
        return tok_then;
    else if (token == "else")
        return tok_else;
    else
        return tok_identifier;
}


// Whether or not the character is for a number
static int is_number_char(int character)
{
    return isdigit(character) || character == '.';
}


// Whether or not the character is an end of line
static int is_not_endofline(int character)
{
    int is_end_of_line = character == EOF || character == '\n' || character == '\r';
    return !is_end_of_line;
}


Token GetToken(std::istream& input)
{
    Token token;
    std::string token_identifier;
    int current_character = input.get();

    // First, trim the leading whitespace
    if (isspace(current_character))
        ConsumeWhileCondition(input, &current_character, isspace);

    // if first character is a letter, consume as identifier
    if (isalpha(current_character))
    {
        token_identifier = ConsumeWhileCondition(input, &current_character, isalnum);
        token.token = LookupToken(token_identifier);
        token.identifier = token_identifier;
        token.number = std::numeric_limits<double>::quiet_NaN();
        return token;
    }
    // if first character is a number, consume as number
    else if (isdigit(current_character))
    {
        token_identifier = ConsumeWhileCondition(input, &current_character, is_number_char);
        token.token = tok_number;
        token.identifier = token_identifier;
        token.number = stod(token_identifier);
        return token;
    }
    // if first character starts a comment, consume until end of line
    else if (current_character == '#')
    {
        ConsumeWhileCondition(input, &current_character, is_not_endofline);
        return GetToken(input);
    }
    // check for eof
    else if (current_character == EOF)
    {
        token.token = tok_eof;
        token.identifier = "";
        token.number = std::numeric_limits<double>::quiet_NaN();
        return token;
    }

    // return current character as is
    token.token = current_character;
    token.identifier = "";
    token.number = std::numeric_limits<double>::quiet_NaN();
    return token;
}
