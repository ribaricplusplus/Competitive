/*
#		    Problem     Verdict     Language	Run Time    Submission Date
24103718    848	Fmt     Accepted    C++11	    0.000	    2019-10-25 14:02:51 */
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

void removeAllBreakpoints(string &input)
{
    bool removeFlag = true;
    int next;
    for (int i = 0; i < input.length() - 1; ++i)
    {
        if (input[i] == '\n')
        {
            removeFlag = true;
            next = i+1;
            while (input[next] == '\n' ||
                   input[next] == ' ')
            {
                removeFlag = false;
                ++next;
            }
            --next;
            if (removeFlag)
            {
                input[i] = ' ';
            }
            i = next;
        }
    }
}
/*  Removes spaces around pos and automatically repositions pos.

    option = 0  =>  Remove spaces around pos
    option = 1  =>  Remove spaces before pos
    option = 2  =>  Remove spaces after pos  */
void removeSpaces(string& input, int& pos, int option = 0)
{
    --pos;
    /* Remove spaces before */
    while((pos > 0) && (input[pos] == ' ') && (option != 2))
    {
        input.erase(pos,1);
        --pos;
    }
    ++pos;
    /* Remove spaces after */
    ++pos;
    while((pos < input.length()) && (input[pos] == ' ') && (option != 1))
    {
        input.erase(pos,1);
    }
    --pos;
}

string fmt(string&& segment)
{
    if (segment.length() < 73)
    {
        return segment;
    }
    int currentPos = 72;
    bool spaceFoundFlag = false;
    vector<int> breakpoints;
    /* Place the breakpoints */
    while(currentPos < segment.length())
    {
        /* The assumed breakpoint is at a space */
        if(segment[currentPos] == ' ')
        {
            removeSpaces(segment, currentPos);
            breakpoints.push_back(currentPos);
        }
        /* The assumed breakpoint is at a word */
        else
        {
            while(currentPos > 0 && segment[currentPos] != ' ')--currentPos;
            if(currentPos != 0)
            {
                removeSpaces(segment, currentPos, 1);
                breakpoints.push_back(currentPos);
            }
            else{//Long word, no breakpoint
            }
        }
        /* Position the next breakpoint:
            Default is current + 73
            If default lands on the same word (very long word),
            position at the end of word or nowhere if end of segment reached. */
        spaceFoundFlag = false;
        for(int i = 1; i <= 73; ++i)
        {
            ++currentPos;
            if(currentPos > segment.length()) break; //No more breakpoints
            if (segment[currentPos] == ' ') spaceFoundFlag = true;
        }
        if (!spaceFoundFlag)
        {
            //Very long word happened. Its end is where the next breakpoint consideration will be.
            while(currentPos < segment.length() && segment[currentPos] != ' ')++currentPos;
        }
    }
    /* Form the breakpoints */
    for(unsigned int i = 0; i < breakpoints.size(); ++i)
    {
        segment[breakpoints[i]] = '\n';
    }
    return segment;
}

string solve(string&& input)
{
    stringstream result;
    removeAllBreakpoints(input);
    int prevPos = 0;
    int pos = input.find('\n', prevPos);
    bool firstRunFlag = true;
    while(pos != string::npos)
    {
        removeSpaces(input, pos, 1);
        if(firstRunFlag)
        {
            result << fmt(input.substr(prevPos, pos-prevPos));
            result << "\n";
            firstRunFlag = false;
        }
        else
        {
            result << fmt(input.substr(prevPos+1, pos-prevPos-1));
            result << "\n";
        }
        prevPos = pos; 
        pos = input.find('\n', prevPos+1);  
    }
    /* Don't forget the last segment */
    int endPos = input.length()-1;
    removeSpaces(input, endPos, 1);
    result << fmt(input.substr(prevPos+1, input.length()-prevPos-1));
    return result.str();
}

int main()
{
    stringstream input;
    char ch;
    while(cin.get(ch))
    {
        input << ch;
    }
    string&& result = solve(input.str());
    cout << result;
    return 0;
}