#include <gtest.h>
#include <string>
#include <queue>
#include "expressions.h"
#include "expressions.cpp"
using namespace std;
string queue_to_string(queue <Lexema> t){
    string res = "";
    while (!t.empty()) {
        res += t.front().getStr();
        res += "|";
        t.pop();
    }
    return res;
}

TEST(Arithmetic, lexema_check)
{
    string str = "( 123 + 23 *    8) / 5\n";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    int check_arr[3] = {0, 0, 0}; // 0-Oper  1-Value
    while (!lex_res.empty()) {
        Lexema lex = lex_res.front();
        if(lex.getType() == Operation) check_arr[0]++;
        if(lex.getType() == Value) check_arr[1]++;
        lex_res.pop();
    }
    EXPECT_EQ(5, check_arr[0]);
    EXPECT_EQ(4, check_arr[1]);
}

TEST(Parser, lexema_check_unknow_element)
{
    string str = "( 123 + 23 *    sin(8)) / 5\n";
    Arithmetic arif;
    EXPECT_ANY_THROW(arif.lex(str));
}

TEST(Parser, validation_check_ok)
{
    string str = "( 123 + 23 *    8) / 5\n";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    EXPECT_NO_THROW(arif.Validate(lex_res));
}

TEST(Parser, validation_check_brackets_error)
{
    string str = "( 123 + 23 * 8)) / 5\n";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    EXPECT_ANY_THROW(arif.Validate(lex_res));
}

TEST(Parser, validation_check_operation_error)
{
    string str = "( 123 ++ 23 * 8) / 5\n";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    EXPECT_ANY_THROW(arif.Validate(lex_res));
}

TEST(Parser, postfix_1)
{
    string str = "( 123 + 23 * 8) / 5\n";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    queue <Lexema> postfix = arif.toPostfix(lex_res);
    EXPECT_EQ("123|23|8|*|+|5|/|",queue_to_string(postfix));
}

TEST(Parser, postfix_2)
{
    string str = "( 123 +10 * 2-1)/ 20\n";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    queue <Lexema> postfix = arif.toPostfix(lex_res);
    EXPECT_EQ("123|10|2|*|+|1|-|20|/|",queue_to_string(postfix));
}

TEST(Parser, calculate1)
{
    string str = "( 123 +10 * 2-1)/ 20\n";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    queue <Lexema> postfix = arif.toPostfix(lex_res);
    EXPECT_EQ(7.1, arif.calculate(postfix));
}

TEST(Parser, calculate2_with_dot)
{
    string str = "( 123.5 +10.4 * 2-1)/ 20\n";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    queue <Lexema> postfix = arif.toPostfix(lex_res);
    EXPECT_GT(0.00000001, abs(7.165 - arif.calculate(postfix)));
}

TEST(Parser, calculate3)
{
    string str = "124+56*( (2-8) / 4)";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    queue <Lexema> postfix = arif.toPostfix(lex_res);
    EXPECT_EQ(40, arif.calculate(postfix));
}

TEST(Parser, calculate4)
{
    string str = "78/5+5*2-4";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    queue <Lexema> postfix = arif.toPostfix(lex_res);
    EXPECT_EQ(21.6, arif.calculate(postfix));
}

TEST(Parser, calculate_error)
{
    string str = "78/5+5*2-";
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    queue <Lexema> postfix = arif.toPostfix(lex_res);
    EXPECT_ANY_THROW(arif.calculate(postfix));
}














