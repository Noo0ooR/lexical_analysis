#include <iostream>
#include <vector>
using namespace std;

const int NONE = 0;
const int ID = 1;
const int TYPE_NUM = 2;
const int TYPE_REAL = 3;
const int TYPE_STR = 4;
const int KEY_IF = 5;
const int KEY_WHILE = 6;
const int KEY_FOR = 7;
const int KEY_RETURN = 8;
const int KEY_INT = 9;
const int KEY_DOUBLE = 10;
const int KEY_STRING = 11;
const int OPER_NOTEQ = 12;
const int OPER_ADD = 13;
const int OPER_SUB = 14;
const int OPER_MULT = 15;
const int OPER_DIV = 16;
const int OPER_EQU = 17;
const int OPER_LPAR = 18;
const int OPER_RPAR = 19;
const int END = 20;
const int MAX = 21;

const string match_key[] = {"NONE","ID","TYPE_NUM","TYPE_REAL","TYPE_STR","KEY_IF","KEY_WHILE","KEY_FOR","KEY_RETURN","KEY_INT","KEY_DOUBLE","KEY_STRING","OPERNOTEQ",\
"OPER_ADD","OPER_SUB","OPER_MULT","OPER_DIV","OPER_EQU","OPER_LPAR","OPER_RPAR","END"};
const string type_list[] = {"if","while","for","return","int","double","string"};
const int type_begin = KEY_IF;
const string oper_list = "+-*/=<>!(){}";

bool is_charactor(char c)
{
    if(c >= 'a' && c <= 'z') return true;
    if(c >= 'A' && c <= 'Z') return true;
    if(c == '_') return true;
    return false;
}

bool is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

bool is_blank(char c)
{
    if(c == ' ') return true;
    if(c == '\n') return true;
    if(c == '\t') return true;
    return false;
}

bool is_operator(char c)
{
    string::const_iterator iter ;
    for(iter = oper_list.begin() ; iter != oper_list.end() ; ++iter)
    {
        if(*iter == c) return true;
    }
    return false;
}

bool is_end(char c)
{
    return c ==';';
}

struct token{
    token(string str = "none", int type = NONE):elemt(str),_typeid(type){
    }
    void clean(){
        elemt = match_key[NONE];
        _typeid = NONE;
    }
    string elemt;
    int _typeid;
};

int parse_identity(string &str, string::iterator i_iter, token& output)
{
    output.clean();
    const int s_input = 0;
    const int s_exit = 1;
    bool flag = false;
    bool is_error = false;
    vector<char> o;
    string::iterator iter = i_iter;
    int state = s_input;
    if(is_digit(*iter) == true) state = s_exit;
    while(flag == false)
    {
        switch(state)
        {
            case s_input:
                if(iter == str.end()){
                    state = s_exit;
                }if(is_charactor(*iter) == false && is_digit(*iter) == false)
                {
                    state = s_exit;
                }else{
                    o.push_back(*iter++);
                }
                break;
            case s_exit:
                if(iter == i_iter) is_error = true;
                flag = true;
                break;
        }
    }
    if(is_error){
        return -1;
    }else{
        output.elemt = string(o.begin(), o.end());
        output._typeid = ID;
        return iter - i_iter;
    }
}

int parse_digit(string &str, string::iterator i_iter, token& output)
{
    output.clean();
    const int s_input = 0;
    const int s_exit = 1;
    const int s_point = 2;
    bool flag = false;
    bool is_error = false;
    bool is_point = false;
    vector<char> o;
    string::iterator iter = i_iter;
    int state = s_input;
    while(flag == false)
    {
        switch(state)
        {
        case s_input:
            if(iter == str.end()){
                state = s_exit;
            }else if(*iter == '.'){
                state = s_point;
            }else if(is_digit(*iter) == false){
                state = s_exit;
            }else{
                o.push_back(*iter++);
            }
            break;
        case s_exit:
            flag = true;
            if(iter == i_iter) is_error = true;
            break;
        case s_point:
            if(is_point){
                is_error = true;
                state = s_exit;
            }else{
                is_point = true;
                o.push_back(*iter++);
                state = s_input;
            }
            break;
        }
    }
    if(is_error){
        return -1;
    }else{
        output.elemt = string(o.begin(), o.end());
        if(is_point){
            output._typeid = TYPE_REAL;
        }else{
            output._typeid = TYPE_NUM;
        }
        return iter - i_iter;
    }
}

int parse_keyword(string &str, string::iterator i_iter, token &output)
{
    const int s_input = 0;
    const int s_exit = 1;
    bool flag = false;
    bool is_error = false;
    vector <char> o;
    string::iterator iter = i_iter;
    int state = s_input;
    if(is_digit(*iter)) state = s_exit;
    while(flag == false)
    {
        switch(state){
            case s_input:
                if(iter == str.end()){
                    state = s_exit;
                }else if(is_charactor(*iter) == false && is_digit(*iter) == false){
                    state = s_exit;
                }else{
                    o.push_back(*iter++);
                }
                break;
            case s_exit:
                if(iter == i_iter) is_error = true;
                flag = true;
                break;
        }
    }
    if(is_error){
        return -1;
    }else{
        string tmp(o.begin(), o.end());
        int tag = -1;
        for(int i = 0 ; i != 7 ; ++i )
        {
            if(tmp == type_list[i])
            {
                tag = i;
                break;
            }
        }
        if(tag == -1){
             return -1;
        }else {
            output.elemt = tmp;
            output._typeid = type_begin + tag;
            return iter - i_iter;
        }
    }
}

int parse_string(string &str, string::iterator i_iter, token& output)
{
    const int s_input = 0;
    const int s_exit = 1;
    const int s_note = 2;
    output.clean();
    bool flag = false;
    bool is_error = false;
    vector <char> o;
    string::iterator iter = i_iter;
    int state = s_input;

    if(*iter != '"'){
        state = s_exit;
    }else{
        ++iter;
    }
    while(flag == false)
    {
        switch(state)
        {
        case s_input:
            if(iter == str.end()){
                 state = s_exit;
            }else if(*iter == '\\'){
                state = s_note;
            }else if(*iter == '"'){
                state = s_exit;
            }else{
                o.push_back(*iter++);
            }
            break;
        case s_exit:
            if(iter == i_iter) is_error = true;
            flag = true;
            break;
        case s_note:
            if((iter + 1) != str.end() && *(iter + 1) == '"'){
                o.push_back(*(iter + 1 ));
                iter += 2;
            }else if((iter + 1) != str.end() && *(iter + 1) == 'n'){
                o.push_back('\n');
                iter += 2;
            }else if((iter + 1) != str.end() && *(iter + 1) == 't'){
                o.push_back('\t');
                iter += 2;
            }
            state = s_input;
            break;
        }
    }
    if(is_error)
    {
        return -1;
    }else{
        output.elemt = string(o.begin(), o.end());
        output._typeid = TYPE_STR;
        return iter - i_iter;
    }
}

int parse_end(string &str, string::iterator i_iter, token &output)
{
    output.clean();
    if(i_iter != str.end())
    {
        if(is_end(*i_iter)){
            output.elemt = ";";
            output._typeid = END;
            return 1;
        }
    }
    return -1;
}

int parse_operator(string &str, string::iterator i_iter, token &output)
{
    bool is_error = false;
    output.clean();
    if(i_iter != str.end())
    {
        switch(*i_iter)
        {
            case '+': output._typeid = OPER_ADD; break;
            case '-': output._typeid = OPER_SUB; break;
            case '*': output._typeid = OPER_MULT;break;
            case '/': output._typeid = OPER_DIV; break;
            case '=': output._typeid = OPER_EQU; break;
            case '(': output._typeid = OPER_LPAR;break;
            case '{': output._typeid = OPER_LPAR;break;
            case ')': output._typeid = OPER_RPAR;break;
            case '}': output._typeid = OPER_RPAR;break;
            case '!': if((i_iter+1) != str.end() && *(i_iter + 1) == '=') output._typeid = OPER_NOTEQ;break;
            default: is_error = true; break;
        }
    }else{
        is_error = true;
    }
    if(is_error){
        return -1;
    }else if(output._typeid != OPER_NOTEQ){
        output.elemt = *i_iter;
        return 1;
    }else{
        output.elemt = "!=";
        return 2;
    }
}

int main()
{
    bool success = true;
    token t;
    int tt = -1;
    string in ;
    getline(cin,in);
    string::iterator it = in.begin();
    vector<token>out;
    while(it != in.end())
    {
        if(is_blank(*it)){

            ++it;
            continue;
        }
        tt = parse_end(in, it, t);
        if(tt != -1){
            out.push_back(t);
            it += tt;
            continue;
        }
        tt = parse_digit(in, it, t);
        if(tt != -1){
            out.push_back(t);
            it += tt;
            continue;
        }
        tt = parse_string(in, it, t);
        if(tt != -1){
            out.push_back(t);
            it += tt;
            continue;
        }
        tt = parse_operator(in, it, t);
        if(tt != -1){
            out.push_back(t);
            it += tt;
            continue;
        }
        tt = parse_keyword(in, it, t);
        if(tt != -1){
            out.push_back(t);
            it += tt;
            continue;
        }
        tt = parse_identity(in, it, t);
        if(tt != -1){
            out.push_back(t);
            it += tt;
        }else{
            success = false;
            break;
        }
    }
    if(success)
    {
        vector<token>::iterator iter;
        for(iter = out.begin(); iter != out.end(); ++iter)
        {
            cout << "< " << (*iter).elemt << " , " << (*iter)._typeid << " >" << endl;
        }
    }else{
        cout << "." << *it << "." << "compile error!" << endl;
    }

    return 0;
}
