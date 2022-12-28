#include "expressions.cpp"
#include <stdio.h>
using namespace std;

int main() {
    string str; // = "78/(5+5.5)*2"; // 5+(-3) // 56*4+2.5
    std::cout << "Input field: " << "\n";
    cin >> str;
    Arithmetic arif;
    try
    {
        queue <Lexema> lex_res = arif.lex(str);
        try
        {
            queue <Lexema> postfix = arif.toPostfix(lex_res);
            cout << "result: " <<"\n";
            cout << arif.calculate(postfix) << endl;
        }
        catch(string s)
        {
            cout << s << "\n";
        }
    }
    catch(IExeption *ex)
    {
    }
    return 0;
}


//2(2+3) 1(2+3) (2+3)1   (2+3)*7/0

//))) (((


