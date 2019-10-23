/* #		Problem	            Verdict	    Language	Run Time	Submission Date
24092834	10150	Doublets	Accepted	C++11	    2.830	    2019-10-23 12:48:42 */
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <queue>
#include <algorithm>
using namespace std;

struct Vertex
{
    string name;
    vector<Vertex *> adj;
    bool visited;
    Vertex *parent;
    Vertex(const string &s) : name(s), visited(false), parent(nullptr){};
    void addToAdj(Vertex *vertex)
    {
        adj.reserve(adj.size() + 1);
        adj.push_back(vertex);
    }
};

/* Do two strings differ by only 1 letter? */
bool diffByOne(const string &first, const string &second)
{
    unsigned int count = 0;
    if (first.length() != second.length())
    {
        return false;
    }

    auto secondIter = second.begin();
    for (auto firstIter = first.begin(); firstIter != first.end(); ++firstIter)
    {
        if ((*firstIter != *secondIter))
        {
            if (count > 1)
            {
                return false;
            }
            else
            {
                ++count;
            }
        }
        ++secondIter;
    }
    if (count == 1)
    {
        return true;
    }
    return false;
}

void reset(unordered_map<string, Vertex *> &input)
{
    for (auto iter : input)
    {
        iter.second->parent = nullptr;
        iter.second->visited = false;
    }
}
vector<string> solve(unordered_map<string, Vertex *> &input, pair<Vertex *, Vertex *> &wordPair)
{
    static int resetFlag = 0;
    if (resetFlag > 0)
        reset(input);
    ++resetFlag;
    queue<Vertex *> que;
    que.push(wordPair.first);
    wordPair.first->visited = true;
    Vertex *vertex;
    while (!que.empty())
    {
        vertex = que.front();
        que.pop();
        for (unsigned int i = 0; i < vertex->adj.size(); ++i)
        {
            if (vertex->adj[i]->name == wordPair.second->name)
            {
                vector<string> result;
                result.reserve(result.size() + 1);
                result.push_back(wordPair.second->name);
                vertex->adj[i]->parent = vertex;
                Vertex *parent = vertex->adj[i]->parent;
                while (parent != nullptr)
                {
                    result.reserve(result.size() + 1);
                    result.push_back(parent->name);
                    parent = parent->parent;
                }
                return result;
            }
            else if (!vertex->adj[i]->visited)
            {
                vertex->adj[i]->parent = vertex;
                vertex->adj[i]->visited = true;
                que.push(vertex->adj[i]);
            }
        }
    }
    return vector<string>();
}

int main()
{
    /* Read in the dictionary and construct vertices. */
    unordered_map<string, Vertex *> *input = new unordered_map<string, Vertex *>();
    input->reserve(10000);
    string s;
    bool flag = true;
    Vertex *vertex;
    while (cin.peek() != '\n')
    {
        getline(cin, s);
        if (flag && input->size() >= 10000)
        {
            input->reserve(26000);
            flag = false;
        }
        vertex = new Vertex(s);
        input->insert({s, vertex});
    }
    cin.get(); //Consume the trailing \n
    /* Construct adjacency lists */
    string current;
    for (auto inputIter = input->begin(); inputIter != input->end(); ++inputIter)
    {
        current = inputIter->first;
        bool checkerFlag = true;
        for (auto checkerIter = inputIter; checkerIter != input->end(); ++checkerIter)
        {
            auto hypotheticalIter = checkerIter; //HypotheticalIter is used to check if this is last element
            ++hypotheticalIter;
            if (checkerFlag && (hypotheticalIter != input->end()))
            {
                ++checkerIter;
                checkerFlag = false;
            }
            if (diffByOne(current, checkerIter->first))
            {
                inputIter->second->addToAdj(checkerIter->second);
                checkerIter->second->addToAdj(inputIter->second);
            }
        }
    }
    /* Read the pairs and solve the problem */
    pair<string, string> wordPair;
    pair<Vertex *, Vertex *> vertexPair;
    vector<string> result;
    string inputPair;
    bool firstResultFlag = true;
    unsigned int separatorPos;
    while (getline(cin, inputPair))
    {
        separatorPos = inputPair.find(" ");
        wordPair.first = inputPair.substr(0, separatorPos);
        wordPair.second = inputPair.substr(separatorPos + 1, inputPair.size());
        if (firstResultFlag)
        {
            firstResultFlag = false;
        }
        else
        {
            cout << "\n";
        }
        vertexPair.first = input->find(wordPair.first)->second;
        vertexPair.second = input->find(wordPair.second)->second;
        result = solve(*input, vertexPair);
        if (result.size() == 0)
        {
            cout << "No solution.\n";
        }
        else
        {
            auto iter = result.end();
            while (iter != result.begin())
            {
                --iter;
                cout << *iter << "\n";
            }
        }
    }
    return 0;
}