#include <sstream>
#include <cmath>
#include "gtest/gtest.h"

#include "libkaleidoscope_lexer/lexer.h"
#include "libkaleidoscope_parser/ast.h"
#include "libkaleidoscope_parser/parser.h"


namespace
{


// The fixture for testing class ParserTest.
class ParserTest : public ::testing::Test
{
  protected:
	// set up
    ParserTest() {}
  
	// clean up
    virtual ~ParserTest() {}
  
	// additional setup code
    virtual void SetUp() {}
  
	// additional cleanup code
    virtual void TearDown() {}
};


TEST(ParserTest, GetNextTokenWorks)
{
    std::istringstream stream("100");
    Parser parser = Parser(stream);
    Token token = parser.get_next_token();

    EXPECT_EQ(token.token, tok_number);
    EXPECT_EQ(token.identifier, "100");
    EXPECT_EQ(token.number, 100.0);
}


TEST(ParserTest, ParseNumberExpressionWorks)
{
    std::istringstream stream("100");
    Parser parser = Parser(stream);
    Token token = parser.get_next_token();

    EXPECT_EQ(token.token, tok_number);

    // Check ParseExpression parses a number expression
    auto expr = parser.ParseExpression(token);

    EXPECT_TRUE(dynamic_cast<NumberExprAST*>(expr.get()));
}


TEST(ParserTest, ParseVariableExpressionWorks)
{
    std::istringstream stream("abcd");
    Parser parser = Parser(stream);
    Token token = parser.get_next_token();

    EXPECT_EQ(token.token, tok_identifier);


    // Check ParseExpression parses an identifier expression
    auto expr = parser.ParseExpression(token);
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(expr.get()));
}


TEST(ParserTest, ParseCallExpressionWorks)
{
    std::istringstream stream("abcd(foo)");
    Parser parser = Parser(stream);
    Token token = parser.get_next_token();
    EXPECT_EQ(token.token, tok_identifier);


    // Check ParseExpression parses an identifier expression
    auto expr = parser.ParseExpression(token);
    EXPECT_TRUE(dynamic_cast<CallExprAST*>(expr.get()));
}


}
