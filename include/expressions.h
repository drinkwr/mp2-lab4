#pragma once
#include<vector>
#include<iostream>
#include<string>
#include<map>

class Expression {
std::string source_str, modified_str;;
     std::string alph_nums = "0123456789",
         alph_letters="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
         alph_operations = "-+/*",
         //alph_opening_brackets ="({[",
         //alph_closing_brackets=")}]",
         alph_separator=".,";
    
    std::string OpenBrackets = "([{";
    std::string CloseBrackets = ")]}";
    std::string SighsOperations = "-+/*=";
    std::vector<std::string> postfix_form,variables_list;
    //std::vector<std::string>alph_constants;// = {
     //"pi","e"
     //};
     std::map <std::string, double> variables;
    std::map <std::string, double> constants;
    //= {
         //{"pi",3.1415926535897932384626433832795},
         //{"e",2.7182818284590452353602874713527}
     //};
    std::map < char, int > prioritet;//= {
        // {'(',0}
         //{'[',0},
         //{'{',0},
         //{'+',1},
         //{'-',1},
         //{'*',2},
         //{'/',2},
    // };
     double res;
     bool is_correct;
    bool expressionIsCorrect();
         //void cut();
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
    //void changeconstants();
         double getResult() { return res; };
         bool isCorrect() { return is_correct; }
         std::string getAcceptableNums() { return alph_nums; }
         std::string getAcceptableOperations() { return alph_operations; }
         //std::string getAcceptableOpeningBrackets() { return alph_opening_brackets; }
         //std::string getAcceptableClosingBrackets() { return alph_closing_brackets; }
         std::string getAcceptableSeparators() { return alph_separator; }
         std::vector<std::string> getPostfixForm() { return postfix_form; }
    double getresult() {return res; }
         Expression& operator=(std::string str);
         Expression& operator=(const Expression& exp);



         friend std::istream& operator>>(std::istream& istr,Expression& exp);
         friend std::ostream& operator<<(std::ostream& ostr,const Expression& exp);

};
