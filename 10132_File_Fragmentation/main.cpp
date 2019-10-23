/* 24077894	10132	File Fragmentation	Accepted	C++11	0.020	2019-10-20 16:41:35 */
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Fragment
{
    string frag;
    bool paired;
    Fragment(string &frag) : frag(frag), paired(false){};
    Fragment(const Fragment &other) : frag(other.frag), paired(other.paired){};
};

void unpairAll(vector<Fragment> &input)
{
    for (unsigned int i = 0; i < input.size(); ++i)
    {
        input[i].paired = false;
    }
}

bool allPaired(vector<Fragment> &input)
{
    for (auto frag : input)
    {
        if (!frag.paired)
        {
            return false;
        }
    }
    return true;
}

string solve(vector<Fragment> &input)
{
    //Keep a paired counter, that will be the way to know if all have been paired.
    /* If one of the strings in the pair already paired, ignore */
    vector<pair<Fragment *, Fragment *>> pairs;
    vector<string> tested;
    //Make all pairs
    pair<Fragment *, Fragment *> *mPair;
    for (unsigned int i = 0; i < input.size() - 1; ++i)
    {
        for (unsigned int j = i + 1; j < input.size(); ++j)
        {
            mPair = new pair<Fragment *, Fragment *>(&input[i], &input[j]);
            pairs.reserve(pairs.size() + 1);
            pairs.push_back(move(*mPair));
        }
    }
    string combination;
    for (unsigned int i = 0; i < pairs.size(); ++i)
    {
        for (unsigned int j = 0; j < 2; ++j)
        {
            unpairAll(input);
            if (j == 0)
                combination = pairs[i].first->frag + pairs[i].second->frag;
            else
                combination = pairs[i].second->frag + pairs[i].first->frag;
            pairs[i].first->paired = true;
            pairs[i].second->paired = true;
            //Go through all the other pairs. Can they make this combination?
            for (unsigned int k = 0; k < pairs.size(); ++k)
            {
                if (k == i)
                    continue; //Don't test with self
                if (pairs[k].first->paired || pairs[k].second->paired)
                {
                    continue;
                }
                if ((pairs[k].first->frag + pairs[k].second->frag) == combination || (pairs[k].second->frag + pairs[k].first->frag) == combination)
                {
                    pairs[k].first->paired = true;
                    pairs[k].second->paired = true;
                }
            }
            if (allPaired(input))
            {
                return combination;
            }
        }
    }
    return string("");
}

int main()
{
    int testCases;
    cin >> testCases;
    cin.get();
    cin.get();
    vector<Fragment> input;
    string s;
    Fragment *frag;
    for (int i = 0; i < testCases; ++i)
    {
        input.clear();
        while (cin.peek() != '\n' && !cin.eof())
        {
            getline(cin, s);
            frag = new Fragment(s);
            input.reserve(input.size() + 1);
            input.push_back(move(*frag));
        }
        cin.get();
        if (i == (testCases - 1))
            cout << solve(input) << "\n";
        else
            cout << solve(input) << "\n\n";
    }
}