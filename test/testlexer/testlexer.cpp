#include <sstream>
#include <cmath>
#include "gtest/gtest.h"

#include "libkaleidoscope_lexer/lexer.h"


namespace
{


// The fixture for testing class GetToken.
class GetTokenTest : public ::testing::Test
{
  protected:
	// set up
    GetTokenTest() {}
  
	// clean up
    virtual ~GetTokenTest() {}
  
	// additional setup code
    virtual void SetUp() {}
  
	// additional cleanup code
    virtual void TearDown() {}
};


TEST(GetTokenTest, GetsDef)
{
    std::istringstream stream("def");
    Token token = GetToken(stream);

    EXPECT_EQ(token.token, tok_def);
    EXPECT_EQ(token.identifier, "def");
    EXPECT_TRUE(std::isnan(token.number));
}


TEST(GetTokenTest, GetsExtern)
{
    std::istringstream stream("extern");
    Token token = GetToken(stream);

    EXPECT_EQ(token.token, tok_extern);
    EXPECT_EQ(token.identifier, "extern");
    EXPECT_TRUE(std::isnan(token.number));
}


TEST(GetTokenTest, GetsIdentifier)
{
    std::istringstream stream("abc");
    Token token = GetToken(stream);

    EXPECT_EQ(token.token, tok_identifier);
    EXPECT_EQ(token.identifier, "abc");
    EXPECT_TRUE(std::isnan(token.number));
}


TEST(GetTokenTest, GetsNumber)
{
    std::istringstream stream("123");
    Token token = GetToken(stream);

    EXPECT_EQ(token.token, tok_number);
    EXPECT_EQ(token.identifier, "123");
    EXPECT_EQ(token.number, 123.);
}


TEST(GetTokenTest, GetsNumberDecimal)
{
    std::istringstream stream("123.5");
    Token token = GetToken(stream);

    EXPECT_EQ(token.token, tok_number);
    EXPECT_EQ(token.identifier, "123.5");
    EXPECT_EQ(token.number, 123.5);
}


TEST(GetTokenTest, GetsEOF)
{
    std::istringstream stream(EOF);
    Token token = GetToken(stream);

    EXPECT_EQ(token.token, tok_eof);
    EXPECT_EQ(token.identifier, "");
    EXPECT_TRUE(std::isnan(token.number));
}


TEST(GetTokenTest, GetsComment)
{
    std::istringstream stream("# foo 12345\n");
    Token token = GetToken(stream);

    EXPECT_EQ(token.token, tok_eof);
    EXPECT_EQ(token.identifier, "");
    EXPECT_TRUE(std::isnan(token.number));
}


}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
