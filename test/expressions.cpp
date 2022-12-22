#include"expressions.h"
#include<stack>
#include<cctype>
//can be used
//int getPriority(char elem,std::string str) {//priority is based on index in string
//    int index = 0;
//    for(index;elem!=str[index]||index<str.size();index++)
//    return (index>=str.size())?-1:index;
//}

/*bool contains(char symb, std::string set) {
    for (int i = 0; i < set.size(); i++) {
        if (symb == set[i]) return true;
    }
    return false;
}*/

Expression::Expression(std::string str) {
    source_str = str;
    
    is_correct=this->expressionIsCorrect();
    if(is_correct)
    {
        this->changebrackets();
       // this->changeconstants();
        this->create_postfix();
        //this->cut();
        this->calculate();
    }
    prioritet['(']=0;
    prioritet['[']=0;
    prioritet['{']=0;
    prioritet['+']=1;
    prioritet['-']=1;
    prioritet['*']=2;
    prioritet['/']=2;
    
}
Expression& Expression::operator=(const Expression& exp) {
     if (this == &exp) return *this;
     variables = exp.variables;
     postfix_form = exp.postfix_form;
     is_correct = exp.is_correct;
     res = exp.res;
     return *this;
 }

/*void Expression::changeconstants()
{
    for(int i = 0; i < source_str.size(); i++)
    {
        if(source_str[i] == 'e' || source_str[i] == '{')
            source_str[i] = '(';
}
*/
void Expression::changebrackets()
{
    for(int i = 0; i < source_str.size(); i++)
    {
        if(source_str[i] == '[' || source_str[i] == '{')
            source_str[i] = '(';
        if(source_str[i] == ']' || source_str[i] == '}')
            source_str[i] = ')';
    }
}

void Expression::clear() {
     Expression tmp;
     *this = tmp;
 }

bool in(std::string str, std::vector<std::string> arr) {
     for (int i = 0; i < arr.size();i++) {
         if (str == arr[i]) return true;
     }
     return false;
 }

bool Expression:: bracketsIsCorrect()
{
    std::stack<char> st;
    std::string str = this->getSourceString(); //строка
    
if(str.size() == 0)
    return true;
    for (unsigned int i = 0; i < str.size()-1; i++)
    {
        size_t pos1 = OpenBrackets.find(str[i]);
        size_t pos2 = CloseBrackets.find(str[i+1]);
        if (pos1 != std::string::npos && pos2 != std::string::npos)
        {
            return false;
        }
    }
    for (unsigned int i = 0; i < str.size(); i++)
    {
        size_t pos = OpenBrackets.find(str[i]);
        if(pos != std::string::npos)
            st.push(CloseBrackets[pos]);
        
        if (CloseBrackets.find(str[i]) != std::string::npos && !st.empty() && str[i] == st.top())
        {
            st.pop();
        }
        else
        {
            if (CloseBrackets.find(str[i]) != std::string::npos && st.empty() )
                return false;
            if (CloseBrackets.find(str[i]) != std::string::npos && str[i] != st.top() )
                return false;
        }
    }

    return st.empty();
}


bool Expression::isVariable(std::string str) {
     //if(contains(str[0],alph_nums)) return false;
    if(isdigit(str[0])) return false;
     //std::cout << str;
     for (int i = 1; i < str.size(); i++)
         //if (!(contains(str[i], alph_letters) || contains(str[i], alph_nums))) return false;
         if(!(isalpha(str[i]) || isdigit(str[0]))) return false;
     return true;
 }

enum states {
     WAIT_LET_NUM_MIN_OPENBR,
     WAIT_NUM_DOT_OPER_CLOSEBR,
     WAIT_NUM,
     WAIT_NUM_OPER_CLOSEBR,
     WAIT_OPER_CLOSEBR,
     WAIT_LET_NUM_OPENBR,
     WAIT_LET_NUM_OPER_CLOSEBR,

     ERROR
 };
bool Expression::expressionIsCorrect() {
     if (this->bracketsIsCorrect() == 0) return false;
     states a = states::WAIT_LET_NUM_MIN_OPENBR;
     char symb=' ';

     for (int i = 0; i < source_str.size(); i++) {
         symb = source_str[i];

         switch (a) {
         case(states::WAIT_LET_NUM_MIN_OPENBR):
             //if (contains(symb, alph_nums)) {
             if (isdigit(symb)){
                 a = states::WAIT_NUM_DOT_OPER_CLOSEBR;
             }
             //else if (contains(symb, alph_letters)) {
             else if (isalpha(symb)){
                 a = states::WAIT_LET_NUM_OPER_CLOSEBR;
             }
             else if (symb == '-') {
                 a = states::WAIT_LET_NUM_OPENBR;
             }
             //else if (contains(symb, alph_opening_brackets)) {
             else if(OpenBrackets.find(symb) != std::string::npos){
                 a = a;//cостояние не меняется
             }
             else a = states::ERROR;

             break;
         case(states::WAIT_NUM_DOT_OPER_CLOSEBR):
             //if (contains(symb, alph_nums)) {
                 if(isdigit(symb)){
                 a = a;
             }
             //else if (contains(symb, alph_separator)) {
                 else if (symb == '.') {
                 a = states::WAIT_NUM;
             }
             //else if (contains(symb, alph_operations)) {
                 else if(SighsOperations.find(symb) != std::string::npos){
                 a = states::WAIT_LET_NUM_OPENBR;
             }
             //else if (contains(symb, alph_closing_brackets)) {
                 else if(CloseBrackets.find(symb) != std::string::npos){
                 a = states::WAIT_OPER_CLOSEBR;
             }
             else a = states::ERROR;

             break;
         case(states::WAIT_NUM):
             //if (contains(symb, alph_nums)) {
                 if(isdigit(symb)){
                 a = states::WAIT_NUM_OPER_CLOSEBR;
             }
             else a = states::ERROR;

             break;
         case(states::WAIT_NUM_OPER_CLOSEBR) :
             //if (contains(symb, alph_nums)) {
                 if(isdigit(symb)){
                 a = a;
             }
             //else if (contains(symb, alph_operations)) {
                 else if(SighsOperations.find(symb) != std::string::npos){
                 a = states::WAIT_LET_NUM_OPENBR;
             }
             //else if (contains(symb, alph_closing_brackets)) {
                 else if(CloseBrackets.find(symb) != std::string::npos){
                 a = states::WAIT_OPER_CLOSEBR;
             }
             else a = states::ERROR;

             break;
         case(states::WAIT_OPER_CLOSEBR):
             //if (contains(symb, alph_operations)) {
                 if(SighsOperations.find(symb) != std::string::npos){
                 a = states::WAIT_LET_NUM_OPENBR;
             }
             //else if (contains(symb, alph_closing_brackets)) {
                 else if(CloseBrackets.find(symb) != std::string::npos){
                 a = a;
             }
             else a = states::ERROR;

                 break;
         case(states::WAIT_LET_NUM_OPENBR):
             //if (contains(symb, alph_letters)) {
                 if (isalpha(symb)){
                 a = states::WAIT_LET_NUM_OPER_CLOSEBR;
             }
             //else if (contains(symb, alph_nums)) {
                 else if(isdigit(symb)){
                 a = states::WAIT_NUM_DOT_OPER_CLOSEBR;
             }
             //else if (contains(symb, alph_opening_brackets)) {
                 else if(OpenBrackets.find(symb) != std::string::npos){
                 a = states::WAIT_LET_NUM_MIN_OPENBR;
             }
             else {
                 a = states::ERROR;
             }

             break;
         case(states::WAIT_LET_NUM_OPER_CLOSEBR): {
            // if (contains(symb, alph_letters) || contains(symb, alph_nums)) {
             if(isalpha(symb) || isdigit(symb)){
                 a = a;
             }
             //else if (contains(symb, alph_operations)) {
             else if(SighsOperations.find(symb) != std::string::npos){
                 a = states::WAIT_LET_NUM_OPENBR;
             }
             //else if (contains(symb, alph_closing_brackets)) {
             else if(CloseBrackets.find(symb) != std::string::npos){
                 a=states::WAIT_OPER_CLOSEBR; //------------------------------
             }
             else {
                 a = states::ERROR;
             }
             break;
         }
         case(states::ERROR) :
                 return false;
                 break;
         }
     }
     //if (contains(symb, alph_nums) || contains(symb, alph_letters) || contains(symb, alph_closing_brackets)) {
    if(isdigit(symb) || isalpha(symb) || CloseBrackets.find(symb) != std::string::npos){
         return true;
     }
     return false;
 }
void Expression::createBinaryMinus()
 {
     char this_symb = ' ';
     std::string tmp;
     if (source_str[0] == '-')
         tmp += '0';
     tmp += source_str[0];

     for (int i = 1; i < source_str.size(); i++) {
         this_symb = source_str[i];
         //if (contains(source_str[i - 1], alph_opening_brackets) && cur_symb == '-')
         if(OpenBrackets.find(source_str[i - 1]) != std::string::npos && this_symb == '-' )
             tmp += '0';
         tmp += this_symb;
     }
     modified_str = tmp;
 }
/*enum class states {
    WAIT_NUMBER__OR_MINUS_OR_OPENING_BRACKET,
    WAIT_NUMBER_OR_OPENING_BRACKET,
    WAIT_NUMBER,
    WAIT_NUMBER_OR_OPERATION_OR_DOT_OR_CLOSING_BRACKET,
    WAIT_NUMBER_OR_OPERATION_OR_CLOSING_BRACKET,
    WAIT_OPERATION_OR_CLOSING_BRACKET,
    ERROR
};
*/

/*bool Expression::expressionIsCorrect() {
    if (this->bracketsIsCorrect() == 0) return false;
    states a = states::WAIT_NUMBER__OR_MINUS_OR_OPENING_BRACKET;
    char symb;


    for (int i = 0; i < source_str.size(); i++) {
        symb = source_str[i];

        switch (a) {
        //
        case(states::WAIT_NUMBER__OR_MINUS_OR_OPENING_BRACKET):

            //std::cout << "WAIT_NUMBER__OR_MINUS_OR_OPENING_BRACKET\n";
                if(!isdigit(symb) &&
                   SighsOperations.find(symb) == std::string::npos &&
                   OpenBrackets.find(symb) == std::string::npos)
                {
                    a = states::ERROR;
                }
            else if (contains(symb, alph_nums)) {
                a = states::WAIT_NUMBER_OR_OPERATION_OR_DOT_OR_CLOSING_BRACKET;
            }
            else if (symb == '-') {
                a = states::WAIT_NUMBER_OR_OPENING_BRACKET;
            }
            else if (contains(symb, alph_opening_brackets)) {
                
                a = a;
            }
            break;
        //
        case(states::WAIT_NUMBER_OR_OPENING_BRACKET):

            //std::cout << "WAIT_NUMBER_OR_OPENING_BRACKET\n";

            if (!contains(symb,alph_nums) && !contains(symb,alph_opening_brackets)) {
                a = states::ERROR;
            }
            else if (contains(symb, alph_nums)) {
                a = states::WAIT_NUMBER_OR_OPERATION_OR_DOT_OR_CLOSING_BRACKET;
            }
            else if (contains(symb, alph_opening_brackets)) {
                a = states::WAIT_NUMBER__OR_MINUS_OR_OPENING_BRACKET;
            }
            break;
        //
        case(states::WAIT_NUMBER):

            //std::cout << "WAIT_NUMBER\n";

            if (!contains(symb, alph_nums)) {
                a = states::ERROR;

            }
            else {
                a = states::WAIT_NUMBER_OR_OPERATION_OR_CLOSING_BRACKET;
            }
            break;
        //
        case(states::WAIT_NUMBER_OR_OPERATION_OR_CLOSING_BRACKET):

            //std::cout << "WAIT_NUMBER_OR_OPERATION_OR_CLOSING_BRACKET\n";

            if (!contains(symb, alph_nums)&& !contains(symb, SighsOperations)&&!contains(symb,alph_closing_brackets))
                a = states::ERROR;
            else if (contains(symb, SighsOperations))
                a = states::WAIT_NUMBER_OR_OPENING_BRACKET;
            else if(contains(symb,alph_closing_brackets))
                a=a;
            else if (contains(symb, alph_nums)) {
                a = a;
            }
            break;
        //
        case(states::WAIT_OPERATION_OR_CLOSING_BRACKET):

            //std::cout << "WAIT_OPERATION_OR_CLOSING_BRACKET\n";

            if (!contains(symb, SighsOperations) && !contains(symb, alph_closing_brackets)) {
                a = states::ERROR;
                
            }
            else if (contains(symb, SighsOperations))
                a = states::WAIT_NUMBER_OR_OPENING_BRACKET;
            else if (contains(symb, alph_closing_brackets))
                a = a;
            break;
        //
        case(states::WAIT_NUMBER_OR_OPERATION_OR_DOT_OR_CLOSING_BRACKET):

            //std::cout << "WAIT_NUMBER_OR_OPERATION_OR_DOT_OR_CLOSING_BRACKET\n";

            if (!contains(symb, alph_nums)&& !contains(symb,SighsOperations) && !contains(symb,alph_separator) && !contains(symb,alph_closing_brackets) ) {
                a = states::ERROR;
            }
            else if (contains(symb, alph_nums)) {
                a = a;
            }
            else if (contains(symb, SighsOperations)) {
                a = states::WAIT_NUMBER_OR_OPENING_BRACKET;
            }
            else if (contains(symb, alph_separator)) {
                a = states::WAIT_NUMBER;
            }
            else if (contains(symb, alph_closing_brackets)) {
                a = states::WAIT_OPERATION_OR_CLOSING_BRACKET;
            }
            break;

        case(states::ERROR):
            //std::cout <<"ERROR\n";
            return false;
        }
        if (!contains(source_str[source_str.size() - 1], alph_nums) && !contains(source_str[source_str.size() - 1], alph_closing_brackets)) return false;
    }
    return true;
}
*/

/*void Expression::cut() {

    std::stack<char> operations_stack;
    
    if (is_correct == false) return;
    char cur_symb;
    std::string token;

    for (int i = 0; i < source_str.size(); i++) {
        cur_symb = source_str[i];
        if (contains(cur_symb, sim_operations) || contains(cur_symb, alph_opening_brackets)) {

            if (token != "") {
                postfix_form.push_back(token);
                operands.insert({ token, (contains(token[0],alph_just_letters)) ? 0.0 : std::stod(token) });

                token = "";
            }
            if(contains(cur_symb, alph_opening_brackets)==false)
            while (operations_stack.empty() == false && (prioritet[cur_symb]<=prioritet[operations_stack.top()])) {
                if(contains(operations_stack.top(), alph_opening_brackets) == false)
                postfix_form.push_back(std::string(1,operations_stack.top()));
                operations_stack.pop();
            }
            operations_stack.push(cur_symb);


        }
        else if (contains(cur_symb, alph_closing_brackets)) {
            if (token != "") {
                postfix_form.push_back(token);
                operands.insert({ token, (contains(token[0],alph_just_letters)) ? 0.0 : std::stod(token) });

                token = "";
            }

            while (operations_stack.empty() == false) {
                if (contains(operations_stack.top(), alph_opening_brackets) == false)
                    postfix_form.push_back(std::string(1, operations_stack.top()));
                    operations_stack.pop();
                
            }

        }
        else {
            token += cur_symb;
        }
    }
    if (token != "") {
        postfix_form.push_back(token);
        operands.insert({ token, (contains(token[0],alph_just_letters)) ? 0.0: std::stod(token) });
    }
    while (operations_stack.empty() == false) {
        if (contains(operations_stack.top(), alph_opening_brackets) == false)
            postfix_form.push_back(std::string(1, operations_stack.top()));
            operations_stack.pop();

    }

}
*/
void Expression::create_postfix()
{
    prioritet['(']=0;
    prioritet['[']=0;
    prioritet['{']=0;
    prioritet['+']=1;
    prioritet['-']=1;
    prioritet['*']=2;
    prioritet['/']=2;
    
    variables["pi"]=3.1415926535897931;
    variables["e"]=2.718281828459045;
    
    std::stack<char> operations;
    std::string token = "";
    if (is_correct == false)
        return;
    
    createBinaryMinus();
    source_str = modified_str;
    
    for (int i = 0; i < source_str.size(); i++)
    {
        if(isdigit(source_str[i]) || source_str[i]=='.' || isalpha(source_str[i]))
            token += source_str[i];
        else
        {
            if(token != "")
            {
                
                if(SighsOperations.find(token) == std::string::npos)
                {
                    if(isalpha(token[0]))
                    {
                        double val = variables[token];
                        postfix_form.push_back(std::to_string(val));
                    }
                    else
                    {
                        postfix_form.push_back(token);
                        variables[token] = std::stod(token);
                    }
                }
                token = "";
            }
            
            if (SighsOperations.find(source_str[i]) != std::string::npos)
            {
                if(!operations.empty())
                {
                    while(!operations.empty() && operations.top() != '(' &&
                          prioritet[operations.top()] >= prioritet[source_str[i]])
                    {
                        std::string a(1,operations.top());
                        postfix_form.push_back(a);
                        operations.pop();
                    }
                }
                
                operations.push(source_str[i]);
            }
            else
            {
                if(source_str[i] == '(')
                    operations.push(source_str[i]);
                else
                {
                    while(operations.top() != '(')
                    {
                        std::string a(1,operations.top());
                        postfix_form.push_back(a);
                        operations.pop();
                    }
                    operations.pop();
                }
            }
        }
    }
    if(token != "")
    {
        if(SighsOperations.find(token) == std::string::npos)
        {
            if(isalpha(token[0]))
            {
                double val = variables[token];
                //postfix_form.push_back(std::to_string(val));
                char str[100];
                std:: sprintf(str, "%.16lf", val);
                postfix_form.push_back(std::string(str));
            }
            else
            {
                postfix_form.push_back(token);
                variables[token] = std::stod(token);
            }
        }
    }
    while(!operations.empty())
    {
        std::string a(1,operations.top());
        postfix_form.push_back(a);
        operations.pop();
    }
}

/*void Expression::calculate() {
    std::stack<double> st;
    std::string element;
    double v1, v2;
    for (std::string el:postfix_form)
    {
        if (el == "+") {
            v2 = st.top(); st.pop();
            v1 = st.top(); st.pop();
            st.push(v1 + v2);
        }
        else if (el == "-") {
            v2 = st.top(); st.pop();
            v1 = st.top(); st.pop();
            st.push(v1 - v2);
        }
        else if (el == "*") {
            v2 = st.top(); st.pop();
            v1 = st.top(); st.pop();
            st.push(v1 * v2);
        }
        else if (el == "/") {
            v2 = st.top(); st.pop();
            v1 = st.top(); st.pop();
            st.push(v1 / v2);
        }
        else st.push(std::stod(el));
        
    }
    res = st.top();
}
*/
void Expression::calculate() {
     std::stack<double> st;
     std::string lexem;
     double v1, v2;
     for (int i = 0; i < postfix_form.size(); i++) {
         lexem = postfix_form[i];
         
         if (lexem == "+") {
             v2 = st.top(); st.pop();

             v1 = st.top(); st.pop();

             st.push(v1 + v2);
         }
         else if (lexem == "-") {
             v2 = st.top(); st.pop();
             v1 = st.top(); st.pop();

             st.push(v1 - v2);
         }
         else if (lexem == "*") {
             v2 = st.top(); st.pop();
             v1 = st.top(); st.pop();
             st.push(v1 * v2);
         }
         else if (lexem == "/") {
             v2 = st.top(); st.pop();
             v1 = st.top(); st.pop();
             if (v2 == 0) {
                 std::cout << "Division by zero\n"; //throw("Division by zero");
             is_correct = false;
             break;
             }
             st.push(v1 / v2);
         }
         else
         {
             if (isalpha(lexem[0]))
                st.push(variables[lexem]);
             else
                 st.push(std::stod(lexem));
         }
             
     }
     if(st.empty()==false)
     res = st.top();
 }


void delete_spaces(std::string& str) {
     std::string tmp;
     for (int i = 0; i < str.size(); i++)
         if (str[i] != ' ')
             tmp += str[i];
     str = tmp;
 }

Expression& Expression::operator=(std::string str) {
     delete_spaces(str);

     std::string token1, token2;
     bool flag = true;
     for (int i = 0; i < str.size(); i++) {
         if (str[i] == '=') {
             flag = false;
             continue;
         }
         if (flag)
             token1 += str[i];
         else token2 += str[i];
     }
     if (token2 == "") {
         source_str = str;
         is_correct = expressionIsCorrect();
         if (is_correct) {
             create_postfix();
             calculate();
         }
         else source_str = "";
     }
     else if (source_str=="" && isVariable(token1)) {
         Expression tmpexp2;
         tmpexp2.variables = variables;
         tmpexp2 = token2;
         if (!tmpexp2.is_correct) throw("Expression is not correct");
         variables[token1] = tmpexp2.res;
         //source_str = str;
     }
     else if(in(token1,variables_list)) {
         Expression tmpexp2;
         tmpexp2.variables = variables;
         tmpexp2 = token2;
         if (!tmpexp2.expressionIsCorrect()) throw("Expression is not correct");

         variables[token1] = tmpexp2.res;
         calculate();
     }
     else {
         Expression tmpexp1(token1), tmpexp2(token2);
         if (tmpexp1.is_correct && tmpexp2.is_correct) {
             if (tmpexp1.res == tmpexp2.res) {
                 source_str = token1;
                 res = tmpexp1.res;
                 is_correct = true;
                 create_postfix();
                 variables_list = tmpexp1.variables_list;
                 variables = tmpexp1.variables;
             }
             else is_correct = false;

         }
         if(!is_correct)
         throw("Expressions is not correct");
     }
    return *this;
     }

/*std::istream& operator>>(std::istream& istream,Expression& exp) {
    std::string tmp,token1,token2;
    bool flag=false;
    if (exp.source_str == "") {
        istream >> exp.source_str;
        exp.is_correct = exp.expressionIsCorrect();
        exp.create_postfix();
        //exp.cut();
    }
    else {  //TODO if's
        istream >> tmp;
        for (auto symb:tmp) {
            if (symb == '=') {
                flag = true;
                continue;
            }
            if (flag == false)
                token1 += symb;
            else token2 += symb;
        }
        exp.variables[token1] = std::stod(token2);// ({ token1,std::stod(token2) });
    }
    exp.calculate();

    return istream;
}*/
std::istream& operator>>(std::istream& istream,Expression& exp) {
     std::string tmp;
     std::getline(istream, tmp,'\n');
     exp = tmp;
     /*
     std::string tmp,token1,token2;
     bool flag=false;
     istream >> tmp;
     if (!contains('=',tmp)) {
         exp.clear();
         exp.source_str = tmp;
         exp.is_correct = exp.expressionIsCorrect();

         if (exp.is_correct)
             exp.cut();
         else exp.source_str = "";
     }
     else {
         for (auto symb : tmp) {
             if (symb == '=') {
                 flag = true;
                 continue;
             }
             if (flag == false)
                 token1 += symb;
             else token2 += symb;
         }
         if (in(token1, exp.alph_constants) || contains(token1[0], exp.alph_nums)) {
             std::cout << "Can't change constant\n";
             exp.is_correct = false;
         }
         else if(token2!="") {
             Expression tmpexp;
             tmpexp.operands = exp.operands;
             tmpexp = token2;
             exp.operands[token1] = tmpexp.res;
         }
         else if (token2 == "") {
             std::cout << '=' << exp.operands[token1];
         }
     }
     exp.is_correct = exp.expressionIsCorrect();
     if (exp.is_correct) {
         exp.calculate();
     }
     */
     return istream;
 }
std::ostream& operator<<(std::ostream& ostream, const Expression& exp) {
    if (exp.is_correct)
        ostream << exp.source_str<<"="<<exp.res;
    else ostream << "Expression is not correct";
    return ostream;
}



