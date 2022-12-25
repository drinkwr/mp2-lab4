#pragma once
#include<vector>
#include<iostream>
#include<string>
#include<map>

class Expression {
    std::string source_str, modified_str;;
    std::string alph_separator=".,";
    std::string OpenBrackets = "([{";
    std::string CloseBrackets = ")]}";
    std::string SighsOperations = "-+/*=";
    std::vector<std::string> postfix_form, variables_list;
    std::map <std::string, double> variables;
    std::map <std::string, double> constants;
    std::map < char, int > prioritet;
    double res;
    bool is_correct;
    bool expressionIsCorrect();
    void create_postfix();
    void calculate();
    void clear();
    bool isVariable(std::string str);
    
public:
    bool bracketsIsCorrect();
    
    Expression() : is_correct(false),res(0) {};
    Expression(std::string str);
    
    std::string getSourceString() {
        return source_str;
    }
    void createBinaryMinus();
    void changebrackets();
    double getResult() { return res; };
    bool isCorrect() { return is_correct; }
    std::string getAcceptableSeparators() { return alph_separator; }
    std::vector<std::string> getPostfixForm() { return postfix_form; }
    double getresult() {return res; }
    Expression& operator=(std::string str);
    Expression& operator=(const Expression& exp);
    friend std::istream& operator>>(std::istream& istr,Expression& exp);
    friend std::ostream& operator<<(std::ostream& ostr,const Expression& exp);
    
};
