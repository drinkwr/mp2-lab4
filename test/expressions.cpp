#include "expressions.h"
#include<iostream>
#include<string>
#include<map>
#include <queue>

using namespace std;

enum TypeElement {
    Operation,
    Value,
    None
};

class Lexema  {
    string str;
    TypeElement type;
    int start;
public:
    Lexema(string _str = "", TypeElement _type = None, int _start = -1) : str(_str), type(_type), start(_start) {};
    string getStr() { return str; }
    TypeElement getType() { return type; }
    
    friend istream& operator >> (istream& in,Lexema& exp) {
        std::string tmp;
        std::getline(in, tmp,'\n');
        exp = tmp;
        return in;
    }
    friend ostream& operator << (ostream& out, Lexema& p) {
        out << "{" << p.str << ", ";
        if (p.type == Operation) out << "operation";
        else if (p.type == Value) out << "value";
        else out << "none";
        out << ", " << p.start << "}";
        return out;
    }

    int getPriority() {
        if (type == Operation) {
            if (str == "(") return 0;
            if (str == ")") return 1;
            if (str == "+" || str == "-") return 2;
            if (str == "*" || str == "/") return 3;
        }
        if (type == None) return -1;
        return -2;
    }
};

class IExeption{
public:
    virtual void show() = 0;
};

class LexEx: public IExeption{
    Lexema lex;
    string mess;
public:
    LexEx(Lexema l, string m = "Lexema"): lex(l), mess(m) {
        show();
    }
    void show() override{
        cout << mess << " at " << lex << endl;
    }
};

class BracketEx: public IExeption{
    Lexema lex;
    string mess;
public:
    BracketEx(Lexema l, string m = "Brackets"): lex(l), mess(m) {
        show();
    }
    void show() override{
        cout << mess << " at " << lex << endl;
    }
};

class OperationEx: public IExeption{
    Lexema lex;
    string mess;
public:
    OperationEx(Lexema l, string m = "operation"): lex(l), mess(m) {
        show();
    }
    void show() override{
        cout << mess << " at " << lex << endl;
    }
};

class ArgsEx: public IExeption{
    Lexema lex;
    string mess;
public:
    ArgsEx(Lexema l, string m = "Arguments"): lex(l), mess(m) {
        show();
    }
    void show() override{
        cout << mess << " at " << lex << endl;
    }
};

class Arithmetic {
public:
    string OpenBrackets = "(";
    string CreateBinMinus(string input)
    {
        string tmp;
        char this_symb = ' ';
        if (input[0] == '-')
            tmp += '0';
        tmp += input[0];
        for (int i = 1; i < input.size(); i++) {
            this_symb = input[i];
            if(OpenBrackets.find(input[i - 1]) != string::npos && this_symb == '-' )
                tmp += '0';
            tmp += this_symb;
        }
        return tmp;
    }
    
    queue<Lexema> lex(string input) {
        queue<Lexema>res;
        input += ' ';
        input = CreateBinMinus(input);
        string tmp = "";
        int state = 0;
        string op = "+-*//*()";
        string sep = " \n\t";
        Lexema P (tmp, None);
        
        for (int i = 0; i < input.size(); i++) {
            char c = input[i];
            switch (state)
            {
            case 0:
                if (c >= '0' && c <= '9') {
                    tmp = c;
                    state = 1;
                    break;
                }
                if (op.find(c) != string::npos) {
                    tmp = c;
                    Lexema l(tmp, Operation);
                    res.push(l);
                    state = 0;
                    break;
                }
                if (sep.find(c) != string::npos) {
                    tmp = "";
                    break;
                }
                    throw LexEx(P, "Unknown element");
                    //throw "Unknown element";
                    
            case 1:
                    if ((c >= '0' && c <= '9') || c == '.') {
                    tmp += c;
                    break;
                }
                if (op.find(c) != string::npos) {
                    Lexema l1(tmp, Value);
                    res.push(l1);
                    tmp = c;
                    Lexema l2(tmp, Operation);
                    res.push(l2);
                    state = 0;
                    break;
                }
                if (sep.find(c) != string::npos) {
                    Lexema l(tmp, Value);
                    res.push(l);
                    state = 0;
                    break;
                }
                    throw LexEx(P, "Unknown element");
                    //throw "Unknown element";
            default:
                break;
            }
        }
        return res;
    }

    void Validate(queue<Lexema> lex) {
        int open = 0;
        int close = 0;
        Lexema current_lex;
        Lexema prev;
        while (!lex.empty()) {
            current_lex = lex.front();
            if (current_lex.getStr() == "(") open++;
            else if (current_lex.getStr() == ")") close++;

            if (close > open) throw BracketEx(current_lex, "Extra )");

            if (prev.getPriority() > 1 && current_lex.getPriority() > 1 && prev.getType() == Operation && current_lex.getType() == Operation) throw OperationEx(current_lex, "Operation conflict");
            prev = current_lex;
            lex.pop();
        }
        if (open > close) throw BracketEx(current_lex, "Unclosed (");
    }


    queue<Lexema> toPostfix(queue<Lexema> lex_res) {
        cout << endl;
        Stack<Lexema> operStack;
        queue<Lexema> res;
        while (!lex_res.empty()) {
            Lexema tmp = lex_res.front();

            if (tmp.getType() == Operation) {
                if (tmp.getStr() != "(") {
                    while (!operStack.empty()) {
                        Lexema prevOper = operStack.top();
                        if (tmp.getPriority() <= prevOper.getPriority()) {
                            res.push(prevOper);
                        }
                        else {
                            if (tmp.getStr() != ")" && prevOper.getStr() != "(") {
                                operStack.push(prevOper);
                            }
                            break;
                        }
                    }
                }
                if (tmp.getStr() != ")") {
                    operStack.push(tmp);
                }
            }
            else {
                res.push(tmp);
            }
            lex_res.pop();
        }
        while (!operStack.empty()) {
            Lexema prevOper = operStack.top();
            res.push(prevOper);
        }
        return res;
    }

    double calculate(queue<Lexema> post) {
        Stack<double> digitStack;
        Lexema lex;
        while (!post.empty()) {
            lex = post.front();

            if (lex.getType() == Operation) {
                if (digitStack.Size() < 2) throw ArgsEx(lex, "Missing argument");
                double a, b;
                a = digitStack.top();
                b = digitStack.top();
                switch (lex.getStr()[0]) {
                case '+':
                    digitStack.push(a + b);
                    break;
                case '-':
                    digitStack.push(b - a);
                    break;
                case '*':
                    digitStack.push(a * b);
                    break;
                case '/':
                    digitStack.push(b / a);
                    break;
                default:
                    break;
                }
            }
            else {
                digitStack.push(stod(lex.getStr()));
            }
            post.pop();
        }
        return digitStack.top();
    }
};










