#include <iostream>
#include <stack>
#include <vector>
#include <assert.h>
using namespace std;

bool is_num(char c)
{
    if( c >= '0' && c <= '9' ) return true;
    return false;
}

bool is_operator(char c)
{
    char *oper = "+-*/%";
    for(int i = 0 ; i < 5  ; ++i)
    {
        if( c == oper[i] ) return true;
    }
    return false;
}

int get_order(char c)
{
    if( c == '(' ) return 1;
    if( c == '+' || c == '-' ) return 2;
    if( c == '*' || c == '/' || c == '%') return 3;
}

string conv_exp(string exp)
{
    stack< char > s;
    vector <char>  v;
    string::iterator iter ;

    for( iter = exp.begin() ; iter != exp.end() ; ++ iter )
    {
        if( is_num(*iter) ){
            v.push_back(*iter);
        }else if( *iter == '(' ){
            s.push(*iter);
        }else if( * iter == ')' ){
            while( s.top() != '(' )
            {
                char t = s.top();
                v.push_back(t);
                s.pop();
            }
            s.pop();
        }else if( is_operator(*iter) ){
            while( s.empty() == false && get_order(*iter) <= get_order(s.top()) )
            {
                char t = s.top();
                v.push_back(t);
                s.pop();
            }
            s.push(*iter);
        }else{
            cout << "Input Error!" << endl;
            return string();
        }
    }
    while( !s.empty() )
    {
       char t = s.top();
       v.push_back(t);
        s.pop();
    }

    string ou( v.begin(),v.end() );
    return ou;
}
int calc_exp(string exp)
{
    stack<int> s;
    string::iterator iter;

    for( iter = exp.begin() ; iter != exp.end() ; ++iter )
    {
        if(is_num(*iter)){
            s.push( *iter - '0' );
        }else if( is_operator(*iter) ){
            int a = s.top();
            s.pop();
            int b = s.top();
            s.pop();
            int c;
            if( *iter == '+' ){
                c = b + a;
            }else if( *iter == '-' ){
                c = b - a;
            }else if( *iter == '*' ){
                c = b * a;
            }else if( *iter == '/' ){
                if( b != 0 ){
                    c = b / a;
                }else{
                    cout << "The dividend is 0! " << endl;
                    c = 0xffffffff;
                }
            }else if( *iter == '%' ){
                c = b % a;
            }else{
                cout << "operator error! " << endl;
                c = 0xffffffff;
            }
            s.push(c);
        }
    }
    return s.top();
}

int main()
{
    string in;
    cin >> in;
    string conv = conv_exp(in);
    int result = calc_exp(conv);
    cout << in << " convert to " << conv << " calc is " << result << endl;

    return 0;
}
