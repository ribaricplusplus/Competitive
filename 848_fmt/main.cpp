#include <iostream>
#include <string>
#include <sstream>
#include <exception>

using namespace std;
#define MIN(x, y) ((x < y) ? (x) : (y))
#define MAX(x, y) ((x > x) ? (x) : (y))
struct Breakpoint
{
    char ch;
    int pos;
    Breakpoint(char ch, int pos) : ch(ch), pos(pos) {}
    Breakpoint()
    {
        ch = '\0';
        pos = 0;
    }
};

void removeLineBreaks(string &input)
{
    unsigned int next;
    bool removeFlag = true;
    for (unsigned int i = 0; i < input.length(); ++i)
    {
        removeFlag = true;
        if (input[i] == '\n')
        {
            next = i + 1;
            while (next < input.length() &&
                   (input[next] == '\n' ||
                    input[next] == ' '))
            {
                removeFlag = false;
                ++next;
            }
            if (removeFlag)
            {
                input[i] = ' ';
            }
            --next;
            i = next;
        }
    }
}

string fmtSegment(string &&segment)
{
    /* Consider the special case */
    if (segment.length() <= 72U)
    {
        return segment;
    }
    unsigned int n = (segment.length() / 73);
    Breakpoint *breakpoints = new Breakpoint[n];
    /* Place breakpoints */
    {
        unsigned int pos = 72;
        unsigned int i = 0;
        while (pos < segment.length())
        {
            breakpoints[i] = Breakpoint(segment[pos], pos);
            pos += 73;
            ++i;
        }
    }
    /* Consider each breakpoint and do the appropriate thing with it */
    unsigned int currentPos;
    unsigned int nextPos;
    unsigned int breakpointsReached;
    for (int i = n - 1; i >= 0; --i)
    {
        currentPos = breakpoints[i].pos;
        if (segment[currentPos] == ' ' &&
            (segment[currentPos - 1] == ' ' ||
             segment[currentPos + 1] == ' '))
        {
            //Breakpoint with spaces around it.
            /* Remove spaces before breakpoint */
            nextPos = ((i - 1) < 0) ? -2 : breakpoints[i - 1].pos;
            breakpointsReached = 0;
            while (--currentPos > 0 && segment[currentPos] == ' ')
            {
                /* What if a different breakpoint is reached along the way? */
                if (currentPos == nextPos)
                {
                    breakpoints[i - breakpointsReached].pos = -1;
                    ++breakpointsReached;
                    if ((i - (breakpointsReached + 1)) < 0)
                    {
                        nextPos = -2; //There is no next breakpoint.
                    }
                    else
                    {
                        nextPos = breakpoints[i - (breakpointsReached + 1)].pos;
                    }
                }
                segment.erase(currentPos, 1);
            }
            ++currentPos; //All spaces preceding the breakpoint deleted, move back
                          //to breakpoint
            breakpoints[i - breakpointsReached].pos = currentPos;

            /* Remove spaces after breakpoint */
            breakpointsReached = 0;
            nextPos = ((i + 1) > (n - 1)) ? -2 : breakpoints[i + 1].pos;
            while (++currentPos < segment.length() && segment[currentPos] == ' ')
            {
                if (currentPos == nextPos)
                {
                    breakpoints[i - breakpointsReached].pos = -1;
                    ++breakpointsReached;
                    if ((i + (breakpointsReached + 1)) > (n - 1))
                    {
                        nextPos = -2; //There is no next breakpoint.
                    }
                    else
                    {
                        nextPos = breakpoints[i - (breakpointsReached + 1)].pos;
                    }
                }
                segment.erase(currentPos, 1);
            }
            --currentPos; //All spaces following the breakpoint deleted,
                          //Move back to breakpoint.
            breakpoints[i - breakpointsReached].pos = currentPos;
            i = i - breakpointsReached;
        }
        /* If the breakpoint is a word, move it to the start of the word,
        and delete all the spaces in front.
        Beware of the edges of the segment and other breakpoints.
        It should be impossible for spaces that are in front of the word
        to need removal.*/
        breakpointsReached = 0;
        nextPos = (i - 1 < 0) ? -2 : breakpoints[i - 1].pos;
        if (segment[currentPos] != ' ')
        {
            while (--currentPos > 0 && segment[currentPos] != ' ')
            {
                if (currentPos == nextPos)
                {
                    breakpoints[i - breakpointsReached].pos = -1; //Kill the current breakpoint
                    ++breakpointsReached;                         //A new breakpoint was reached
                    nextPos = (i - (breakpointsReached + 1)) < 0 ? -2 : breakpoints[i - (breakpointsReached + 1)].pos;
                }
            }
            if (segment[currentPos] == ' ')
            {
                //Remove all the preceding spaces.
                while (--currentPos > 0 && segment[currentPos] == ' ')
                {
                    if (currentPos == nextPos)
                    {
                        breakpoints[i - breakpointsReached].pos = -1;
                        ++breakpointsReached;
                        nextPos = (i - (breakpointsReached + 1) < 0) ? -2 : breakpoints[i - (breakpointsReached + 1)].pos;
                    }
                    segment.erase(currentPos, 1);
                }
                ++currentPos;
                breakpoints[i - breakpointsReached].pos = currentPos;
            }
            else if (currentPos == 0)
            {
                //Beginning of the segment reached. Remove breakpoint.
                breakpoints[i - breakpointsReached].pos = -1;
            }
            else
            {
                throw logic_error("Something weird happened");
            }
            i = i - breakpointsReached;
        }
    }
    /* Breakpoints should now all be spaces. Convert the valid ones to \n */
    for (unsigned int i = 0; i < n; ++i)
    {
        if (breakpoints[i].pos != -1)
        {
            segment[breakpoints[i].pos] = '\n';
        }
    }
    return segment;
}

string solve(string &input)
{
    removeLineBreaks(input);
    stringstream result;
    unsigned int segStartPosPrev = 0;
    unsigned int segStartPos = input.find('\n', segStartPosPrev);
    bool firstFlag = true;
    while (segStartPos != string::npos)
    {
        if (input[MIN(segStartPos - 1, 0)] == ' ')
        {
            while (--segStartPos > 0 && input[segStartPos] == ' ')
            {
                input.erase(segStartPos);
            }
            ++segStartPos;
        }
        if (firstFlag)
        {
            result << fmtSegment(input.substr(segStartPosPrev, segStartPos - segStartPosPrev));
            firstFlag = false;
        }
        else
        {
            result << fmtSegment(input.substr(segStartPosPrev + 1, segStartPos - segStartPosPrev - 1));
        }

        result << '\n';
        segStartPosPrev = segStartPos;
        segStartPos = input.find('\n', segStartPosPrev + 1);
    }
    /* Don't forget the last segment*/
    if (input[MIN(segStartPosPrev - 1, 0)] == ' ')
    {
        while (--segStartPosPrev > 0 && input[segStartPosPrev] == ' ')
        {
            input.erase(segStartPosPrev);
        }
        ++segStartPosPrev;
    }
    result << fmtSegment(input.substr(segStartPosPrev + 1, input.length()));
    return result.str();
}

int main()
{
    char ch;
    stringstream inpstream;
    while (cin.get(ch))
    {
        inpstream << ch;
    }
    string input = inpstream.str();
    cout << solve(input);
    return 0;
}