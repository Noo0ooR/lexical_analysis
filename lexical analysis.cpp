#include <iostream>
#include <vector>
using namespace std;

const int NONE = 0;
const int IDENTITY = 1;
const int KEY_FOR = 2;
const int KEY_WHILE = 3;
const int KEY_IF = 4;
const int KEY_RETURN = 5;
const int KEY_INT = 6;
const int KEY_FLOAT = 7;
const int KEY_DOUBLE = 8;
const int KEY_STRING = 9;
const int OPER_ADD = 10;
const int OPER_SUB = 11;
const int OPER_MULT = 12;
const int OPER_DIV = 13;
const int OPER_EQU = 14;
const int OPER_BIG = 15;
const int OPER_SMALL = 16;
const int S_BRA_L = 17;
const int S_BRA_R = 18;
const int B_BRA_L = 19;
const int B_BRA_R = 20;
const int END = 21;
const int MAX = 22;

const string type_list[] = {"none","identity","for","while","if","return","int","float","double","string","add","sub","mult","div","equal","big","small","S_BRA_L","S_BRA_R","B_BRA_L","B_BRA_R","end"};
const string name_list[] = {"NONE","IDENTITY","KEY_FOR","KEY_WHILE","KEY_IF","KEY_RETURN","KEY_INT","KEY_FLOAT","KEY_DOUBLE","KEY_STRING","OPER_ADD","OPER_SUB","OPER_MULT","OPER_DIV","OPER_EQUAL","OPER_BIG","OPER_SMALL","OPER_BRA","OPER_BRA","OPER_BRA","OPER_BRA","END"};
const string oper_table = "+-*/=><(){}";

const int type_begin = 6;
const int type_end = 10;
const int key_begin = 2;
const int key_end = 20;
const int oper_begin = 10;
const int oper_end = 17;

void debug(int n)
{
    cout << "here is " << n << endl;
}

string getName(int n)
{
    return name_list[n];
}

struct token{
    token(string s = "" , int type = NONE ){
        elemt = s;
        _typeid = type;
    }
    void clean(){
        elemt = "";
        _typeid = NONE;
    }
    string elemt;
    int _typeid;
};

bool is_number(char c)
{
    return (c >= '0' && c <= '9');
}

bool is_charactor(char c)
{
    if(c >= 'a' && c <= 'z') return true;
    if(c >= 'A' && c <= 'Z') return true;
    if(c == '_') return true;
    return false;
}

bool is_operator(char c)
{
    string::const_iterator iter;
    for(iter = oper_table.begin() ; iter != oper_table.end() ; ++iter)
    {
        if(c == *iter) return true;
    }
    return false;
}

bool is_blank(char c)
{
    if(c == ' ') return true;
    if(c == '\n') return true;
    if(c == '\t') return true;
    return false;
}

bool is_end(char c)
{
    return (c == ';');
}

int do_identity(std::string &str , std::string::iterator i_iter , token &output)
{
    output.clean();
    if(is_number(*i_iter)) return -1;
    bool flag = false;
    string::iterator iter;
    for(iter = i_iter ; iter != str.end() ; ++iter)
    {
        if(false == is_number(*iter) && false == is_charactor(*iter))
        {
            break;
        }else{
            output.elemt += *iter;
            flag = true;
        }
    }
    if(flag){
        output._typeid = IDENTITY;
        return iter - i_iter;
    }else{
        return -1;
    }
}

int do_keyword(std::string &str , std::string::iterator i_iter , token &output)
{
    output.clean();
    bool flag = false;
    string::iterator iter;
    for(iter = i_iter ; iter != str.end() ; ++iter)
    {
        if(false == is_charactor(*iter) && false == is_number(*iter))
        {
            break;
        }else{
            output.elemt += *iter;
            flag =true;
        }
    }
    if(flag){
        for(int i = key_begin ; i != key_end ; ++i)
        {
            if(type_list[i] == output.elemt)
            {
                output._typeid = i;
                return iter - i_iter;
                break;
            }
        }
        return -1;
    }else{
        return -1;
    }
}

int do_number(std::string &str , std::string::iterator i_iter , token &output)
{
    output.clean();
    bool flag = false;
    bool is_point = false;
    bool is_fail = false;
    string::iterator iter;
    for(iter = i_iter ; iter != str.end() ; ++iter)
    {
        if('.' == *iter){
            if(is_point == false){
                is_point = true;
                continue;
            }else{
                is_fail = true;
                break;
            }
        }
        if(false == is_number(*iter)){
            break;
        }else{
            output.elemt += *iter;
            flag = true;
        }
    }
    if(is_fail == false && flag == true){
        if(is_point == false){
            output._typeid = KEY_INT;
        }else{
            if(output.elemt.length() > 8){
                output._typeid = KEY_DOUBLE;
            }else{
                output._typeid = KEY_FLOAT;
            }
        }
        return iter - i_iter;
    }else{
        return -1;
    }
}

int do_string(std::string &str , std::string::iterator i_iter , token &output)
{
    output.clean();
    if(*i_iter != '"') return -1;
    string::iterator iter;
    for(iter = i_iter ; iter != str.end() ; ++iter)
    {
        if(*iter == '\\' && (iter + 1) != str.end())
        {
            output.elemt += *iter;
            output.elemt += *(iter + 1);
            ++iter;
        }
        if(*iter == '"'){
            output._typeid = KEY_STRING;
            return iter - i_iter;
        }

    }
    output._typeid = NONE;
    return -1;
}

int do_operator(std::string &str , std::string::iterator i_iter , token &output)
{
    output.clean();
    string::iterator iter = i_iter;
    output.elemt += *iter;
    if(is_operator(*iter)){
        string::const_iterator citer;
        for(citer = oper_table.begin() ; citer != oper_table.end() ; ++citer)
        {
            if(*citer == *iter)
            {
                output._typeid = citer - oper_table.begin() + oper_begin;
                return 1;
            }
        }
    }
    return -1;
}

int do_end(std::string &str , std::string::iterator i_iter , token &output)
{
    output.clean();
    if(is_end(*i_iter)){
        output.elemt += *i_iter;
        output._typeid = END;
        return 1;
    }else{
        return -1;
    }
}

int main()
{
    vector <token> out;
    string in;
   // getline(cin, in);
    in = "int main(){ int i = 0; int j = 1; if(i > j) i = j; return 0;}";
    string::iterator it (in.begin());
    token take;
    int t = -1;

    while(it != in.end())
    {
        if(is_blank(*it)){
            ++it;
            continue;
        }

        t = do_end(in, it, take);
        if(t != -1){
            out.push_back(take);
            it += t;
            continue;
        }

        t = do_string(in, it, take);
        if(t != -1){
            out.push_back(take);
            it += t;
            continue;
        }

        t = do_number(in, it, take);
        if(t != -1){
            out.push_back(take);
            it += t;
            continue;
        }

        t = do_keyword(in, it, take);
        if(t != -1){
            out.push_back(take);
            it += t;
            continue;
        }

        t = do_operator(in, it, take);
        if(t != -1){
            out.push_back(take);
            it += t;
            continue;
        }

        t = do_identity(in, it, take);
        if(t != -1){
            out.push_back(take);
            it += t;
            continue;
        }else{
            cout << "compile error" << endl;
            break;
        }
    }

    vector<token>::iterator o_iter;
    for(o_iter = out.begin() ; o_iter != out.end() ; ++o_iter)
    {
        cout << "< " << (*o_iter).elemt << " , " << getName((*o_iter)._typeid) << " >" <<endl;
    }

    return 0;
}
