/*#include "expressions.cpp"
#include <stdio.h>
using namespace std;

int main() {
    string str; //= "78/(5+5.5)*2"; // 5+(-3)
    std::cout << "Input field: " << "\n";
    cin >> str;
    Arithmetic arif;
    queue <Lexema> lex_res = arif.lex(str);
    queue <Lexema> postfix = arif.toPostfix(lex_res);
    cout << "result: " <<"\n";
    cout << arif.calculate(postfix) << endl;
    return 0;
}
*/






