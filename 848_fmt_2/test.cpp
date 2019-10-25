#include<iostream>
#include<sstream>
#include<string>
using namespace std;
int main()
{
    /* How many characters is tab? */
    string thingy;
    stringstream input;
    char ch;
    while(cin.get(ch))
    {
        input << ch;
    }
    thingy = input.str();
    cout << thingy.length();
    /* By default, tab is counted as 4 characters... Count it as 1? */
    return 0;
}