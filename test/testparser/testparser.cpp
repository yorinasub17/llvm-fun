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


// Test to make sure there is no precedence for non-ascii token
TEST(ParserTest, OperatorPrecedenceTokenTest)
{
    Token def_token;
    def_token.token = tok_def;
    def_token.identifier = "def";
    def_token.number = std::numeric_limits<double>::quiet_NaN();

    EXPECT_EQ(GetOperatorPrecedence(def_token), -1);
}


// Test to make sure there is no precedence for unknown ascii token
TEST(ParserTest, OperatorPrecedenceBadTokenTest)
{
    Token bad_token;
    bad_token.token = 'd';
    bad_token.identifier = "d";
    bad_token.number = std::numeric_limits<double>::quiet_NaN();

    EXPECT_EQ(GetOperatorPrecedence(bad_token), -1);
}


// Test to make sure operator precedence is close to reality
TEST(ParserTest, OperatorPrecedenceRealityTest)
{
    Token mul_token;
    mul_token.token = '*';
    mul_token.identifier = "*";
    mul_token.number = std::numeric_limits<double>::quiet_NaN();

    Token add_token;
    add_token.token = '+';
    add_token.identifier = "+";
    add_token.number = std::numeric_limits<double>::quiet_NaN();

    EXPECT_GT(GetOperatorPrecedence(mul_token), GetOperatorPrecedence(add_token));
}


TEST(ParserTest, ParseNumberExpressionWorks)
{
    std::istringstream stream("100");
    Parser parser = Parser(stream);

    // Check ParseExpression parses a number expression
    auto expr = parser.ParseExpression();
    EXPECT_TRUE(dynamic_cast<NumberExprAST*>(expr.get()));
}


TEST(ParserTest, ParseVariableExpressionWorks)
{
    std::istringstream stream("abcd");
    Parser parser = Parser(stream);

    // Check ParseExpression parses an identifier expression
    auto expr = parser.ParseExpression();
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(expr.get()));
}


TEST(ParserTest, ParseCallExpressionWorks)
{
    std::istringstream stream("abcd(foo)");
    Parser parser = Parser(stream);

    // Check ParseExpression parses a call expression
    auto expr = parser.ParseExpression();
    EXPECT_TRUE(dynamic_cast<CallExprAST*>(expr.get()));
}


TEST(ParserTest, ParseParenExpressionWorks)
{
    std::istringstream stream("(foo)");
    Parser parser = Parser(stream);

    // Check ParseExpression parses a call expression
    auto expr = parser.ParseExpression();
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(expr.get()));
}


// Test to make sure single binary operator is parsed
TEST(ParserTest, ParseSingleBinOp)
{
    std::istringstream stream("a+b");
    Parser parser = Parser(stream);

    // Check ParseExpression parses a binary expression
    auto expr = parser.ParseExpression();
    auto binary_expr = dynamic_cast<BinaryExprAST*>(expr.get());
    EXPECT_TRUE(binary_expr);

    // Check to make sure operator is '+'
    EXPECT_EQ(binary_expr->get_op(), '+');
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(binary_expr->get_left()));
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(binary_expr->get_right()));
}


// Test to make sure right precedent binary operator is parsed
TEST(ParserTest, ParseRightPrecedentBinOp)
{
    std::istringstream stream("a+b*c");
    Parser parser = Parser(stream);

    // Check ParseExpression parses a binary expression
    auto expr = parser.ParseExpression();
    auto binary_expr = dynamic_cast<BinaryExprAST*>(expr.get());
    EXPECT_TRUE(binary_expr);

    // Check to make sure operator is '+'
    EXPECT_EQ(binary_expr->get_op(), '+');
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(binary_expr->get_left()));
    auto right_expr = dynamic_cast<BinaryExprAST*>(binary_expr->get_right());
    EXPECT_TRUE(right_expr);

    // Check to make sure right side is b*c
    EXPECT_EQ(right_expr->get_op(), '*');
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(right_expr->get_left()));
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(right_expr->get_right()));
}


// Test to make sure left precedent binary operator is parsed
TEST(ParserTest, ParseLeftPrecedentBinOp)
{
    std::istringstream stream("a*b+c");
    Parser parser = Parser(stream);

    // Check ParseExpression parses a binary expression
    auto expr = parser.ParseExpression();
    auto binary_expr = dynamic_cast<BinaryExprAST*>(expr.get());
    EXPECT_TRUE(binary_expr);

    // Check to make sure operator is '+'
    EXPECT_EQ(binary_expr->get_op(), '+');
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(binary_expr->get_right()));
    auto left_expr = dynamic_cast<BinaryExprAST*>(binary_expr->get_left());
    EXPECT_TRUE(left_expr);

    // Check to make sure left side is a*b
    EXPECT_EQ(left_expr->get_op(), '*');
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(left_expr->get_left()));
    EXPECT_TRUE(dynamic_cast<VariableExprAST*>(left_expr->get_right()));
}


}
