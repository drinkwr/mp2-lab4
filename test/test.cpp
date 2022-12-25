#include "expressions.h"
#include<iostream>
#include <gtest.h>
TEST(Expression, create_empty_expression)
{
    ASSERT_NO_THROW(Expression exp);
}
TEST(Expression, expression_with_empty_str)
{
    ASSERT_NO_THROW(Expression e(""));
}
TEST(Expression, expression_with_a_single_number)
{
    ASSERT_NO_THROW(Expression e("1"));
}
TEST(Expression, expression_with_a_single_variable)
{
    ASSERT_NO_THROW(Expression e("variable"));
}
TEST(Expression, expression_with_pair_numbers_and_one_operation)
{
    ASSERT_NO_THROW(Expression e("1+2"));
}
TEST(Expression, expression_with_pair_variables_and_one_operation)
{
    ASSERT_NO_THROW(Expression e("tmp1+tmp2"));
}

TEST(Expression, expression_with_one_number_and_one_variable_and_one_operation)
{
    ASSERT_NO_THROW(Expression e("5+tmp2"));
}

TEST(Expression, expression_with_pair_numbers_and_one_operation_and_brackets)
{
    ASSERT_NO_THROW(Expression e("{(1+2)}"));
}
TEST(Expression, can_create_incorrect_expression)
{
    ASSERT_NO_THROW(Expression e("{1+2)"));
}



TEST(Expression, calculate_expression_with_plus_correctly) {
    Expression exp("3+2");
    EXPECT_EQ(5,exp.getResult());
}
TEST(Expression, calculate_expression_with_minus_correctly) {
    Expression exp("3-2");
    EXPECT_EQ(1, exp.getResult());
}
TEST(Expression, calculate_expression_with_multiplie_correctly) {
    Expression exp("3*2");
    EXPECT_EQ(6, exp.getResult());
}
TEST(Expression, calculate_expression_with_division_correctly) {
    Expression exp("3/2");
    EXPECT_EQ(1.5, exp.getResult());
}
TEST(Expression, can_detect_division_by_zero) {
    Expression exp("1/0");
    EXPECT_EQ(0, exp.isCorrect());
}

//can_detect_incorrectness_of_expression

TEST(Expression, INCORRECT_NUM) {
    Expression exp("3.1.1");
    EXPECT_EQ(0, exp.isCorrect());
}
TEST(Expression, INCORRECT_DOUBLE_SEPORATOR) {
    Expression exp("1..0");
    EXPECT_EQ(0, exp.isCorrect());
}

TEST(Expression, INCORRECT_VARIABLE_STARTED_WITH_NUM) {
    Expression exp("1abc");
    EXPECT_EQ(0, exp.isCorrect());
}
TEST(Expression, INCORRECT_VARIABLE_WITH_DOT) {
    Expression exp("tmp.1");
    EXPECT_EQ(0, exp.isCorrect());
}
TEST(Expression, INCORRECT_SEQUENCE_OF_OPERATIONS) {
    Expression exp("1+-2");
    EXPECT_EQ(0, exp.isCorrect());
}
TEST(Expression, INCORRECT_DOUBLE_UNARY_MINUS) {
    Expression exp("--1");
    EXPECT_EQ(0, exp.isCorrect());
}
TEST(Expression, INCORRECT_SINGLES_BRACKETS) {
    Expression exp("{1)");
    EXPECT_EQ(0, exp.isCorrect());
}
TEST(Expression, INCORRECT_BRACKETS) {
    Expression exp("({1)}");
    EXPECT_EQ(0, exp.isCorrect());
}
TEST(Expression, INCORRECT_SINGLES_BRACKETS_WITHOUT_VALUE) {
    Expression exp("()");
    EXPECT_EQ(0, exp.isCorrect());
}
TEST(Expression, DIFFERENT_NUM_OF_BRACKETS) {
    Expression exp("(1))");
    EXPECT_EQ(0, exp.isCorrect());
}
TEST(Expression, INCORRECT_LAST_SYMBOL_OPERATION) {
    Expression exp("1+2-");
    EXPECT_EQ(0, exp.isCorrect());
}
TEST(Expression, INCORRECT_LAST_SYMBOL_DOT) {
    Expression exp("1+2.");
    EXPECT_EQ(0, exp.isCorrect());
}

//can_correctly_calculate_expression

TEST(Expression, after_change_value_of_variable) {
    Expression exp;
    exp = std::string("a=2");
    std::istringstream input("1+a");
    input >> exp;
    EXPECT_EQ(3, exp.getResult());
}
TEST(Expression, after_change_values_of_variables) {
    Expression exp;
    exp = std::string("a=2");
    std::istringstream input1("b=1");
    //std::istringstream input1("a=2");
    input1 >> exp;
    std::istringstream input2("b+a");
    input2 >> exp;
    EXPECT_EQ(3, exp.getResult());
}
TEST(Expression, can_use_constant_pi) {
    Expression exp;
    exp = std::string("a=1");
    std::istringstream input("a+pi");
    input >> exp;
    EXPECT_EQ(4.1415926535897931, exp.getResult());
}
TEST(Expression, can_use_constant_e) {
    Expression exp;
    exp = std::string("a=2");
    std::istringstream input("a+e");
    input >> exp;
    EXPECT_EQ(4.718281828459045, exp.getResult());
}
TEST(Expression, after_change_value_of_variable_written_like_expression) {
    Expression exp;
    exp = std::string("a=2*(-1.5+3.5)");
    std::istringstream input("1.5+a");
    input >> exp;
    EXPECT_EQ(5.5, exp.getResult());
}
TEST(Expression, expression_with_different_priority_operations_correctly) {
    Expression exp("3+2*5");
    EXPECT_EQ(13, exp.getResult());
}
TEST(Expression, expression_with_brackets_correctly) {
    Expression exp("3*(2+5)");
    EXPECT_EQ(21, exp.getResult());
}

TEST(Expression, can_detect_unary_minus) {
    Expression exp("-3");
    EXPECT_EQ(-3, exp.getResult());
}
TEST(Expression, can_detect_unary_minuses) {
    Expression exp("-(-3)");
    EXPECT_EQ(3, exp.getResult());
}
TEST(Expression, can_calculate_expression_with_unary_minuse_correctly) {
    Expression exp("-(3*2)+(-(-1-1))");
    EXPECT_EQ(-4, exp.getResult());
}
TEST(Expression, can_calculate_different_expressions) {
    Expression exp;
    for (int i = 0; i < 5; i++) {
        exp=std::to_string(i)+"+"+std::to_string(i+1);
        EXPECT_EQ(i+i+1, exp.getResult());
    }
}

TEST(Expression, can_calculate_expression_with_different_values_of_variable) {
    for (int i = 0; i < 5; i++) {
        Expression exp;
        exp = std::string("a="+std::to_string(i));
        std::istringstream input("1+a");
        input >> exp;
        EXPECT_EQ(1+i, exp.getResult());
    }
    ASSERT_NO_THROW(Expression exp);
    
}
