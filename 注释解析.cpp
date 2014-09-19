#include <iostream>
#include <vector>
using namespace std;

static const int NORMAL = 0;
static const int NOTE = 1;
static const int NOTE_LINE = 2;
int STATE_NOW = NORMAL;

int main()
{
    vector <char> out;
    string first = "/*";
    string finish = "*/";
    string in;
    cin >> in;
    //in = "int main() /* test */ {} \nhello \n//hello1 \nhello2";
    string::iterator iter;
    for(iter = in.begin() ; iter != in.end() ; ++iter)
    {
        if( STATE_NOW == NORMAL)
        {
            if(iter + 1 != in.end())
            {
                if(*iter == '/')
                {
                    if(*(iter + 1) == '*')
                    {
                       STATE_NOW = NOTE;
                    }else if(*(iter + 1) == '/'){
                        STATE_NOW = NOTE_LINE;
                    }else{
                        out.push_back(*iter);
                    }
                }else{
                    out.push_back(*iter);
                }
            }else{
                out.push_back(*iter);
            }
        }else if(STATE_NOW == NOTE){
            if(iter + 1 != in.end())
            {
                if(*iter == '*' && *(iter + 1) == '/')
                {
                    STATE_NOW = NORMAL;
                    iter++;
                }
            }
        }else if(STATE_NOW == NOTE_LINE){
            if(*iter == '\n')
            {
                STATE_NOW = NORMAL;
            }
        }
    }
    string result(out.begin() , out.end());
    cout << result << endl;
    return 0;
}

