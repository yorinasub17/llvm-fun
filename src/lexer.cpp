#include "lexer.h"

#include <limits>
#include <stdio.h>
#include <ctype.h>


// Advances pointer while predicate condition for character is true, returning
// the combined characters as string.
static std::string ConsumeWhileCondition(std::ifstream& file,
                                         int *current_character,
                                         int (*character_predicate)(int))
{
    std::string characters("");

    // start with current value
    characters += *current_character;

    // Keep consuming while predicate is true
    while ((*character_predicate)(*current_character = file.get()))
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
static int isnumberchar(int character)
{
    return isdigit(character) || character == '.';
}


Token GetToken(std::ifstream& file)
{
    Token token;
    std::string token_identifier;
    int *current_character;
    *current_character = file.get();

    // First, trim the leading whitespace
    if (isspace(*current_character))
        ConsumeWhileCondition(file, current_character, isspace);

    // if first character is a letter, consume as identifier
    if (isalpha(*current_character))
    {
        token_identifier = ConsumeWhileCondition(file, current_character, isalnum);
        token.token = LookupToken(token_identifier);
        token.token_identifier = token_identifier;
        token.token_number = std::numeric_limits<double>::quiet_NaN();
        return token;
    }
    // if first character is a number, consume as number
    else if (isdigit(*current_character))
    {
        token_identifier = ConsumeWhileCondition(file, current_character, isnumberchar);
        token.token = tok_number;
        token.token_identifier = token_identifier;
        token.token_number = stod(token_identifier);
        return token;
    }

    // Error: could not identify token
    return token;
}