#include <iostream>
using namespace std;
int state_count = 0;
static const char _left = '(';
static const char _right = ')';
int main()
{
    string in;
    cin >> in;
    bool flag = true;
    string::iterator iter;
    for( iter = in.begin() ; iter != in.end() ; ++ iter )
    {
        if( *iter == _left )
        {
            state_count ++;
        }else if( *iter == _right )
        {
            state_count --;
        }
        if( state_count < 0 )
        {
            flag = false;
            break;
        }
    }
    if(flag)
        cout << "match !" << endl;
    else
        cout << "connot match !" << endl;
    return 0;
}
