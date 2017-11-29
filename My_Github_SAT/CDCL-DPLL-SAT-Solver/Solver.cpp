#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <utility>
#include <deque>
#include <queue>
#include "Solver.h"
#include "Node.h"
using namespace std;

Solver :: Solver(int numVariables, vector<Clause> cl)
{
    nvar = numVariables;
    clauses = cl;
    bool temp;
    return;
}

void Solver :: printSatisfiedAssignment()
{
    for(int i = 1; i <= nvar; i++)
    {
        if(graph[i].value)
            cout << i << " ";
        else
            cout << -1 * i << " ";
    }
    cout << endl;
}

void Solver :: findSingletons()
{
    int n = clauses.size();
    for(int i = 0; i < n; i++)
    {
        if(clauses[i].polarityMap.size() == 1)
        {
            interestingClauses.push_back(i);
            //cout << "adding " << i << " to interesting clauses" << endl;
        }
    }
}

pair<int, bool> Solver :: makeDecision()
{
    int nextAssigned;
    bool truthValue;
    while(true)                                                                                                                                                              
    {
        nextAssigned = rand() % nvar + 1;
        if(graph.find(nextAssigned) != graph.end())
            continue;
        truthValue = (rand() % 2 == 0 ? false : true);
        break;
    }
    return make_pair(nextAssigned, truthValue);
}

pair<int, bool> Solver :: unitRule(Clause c)
{
    for(auto it = c.polarityMap.begin(); it != c.polarityMap.end(); it++)
    {
        if(graph.find(it->first) == graph.end())
        {
            return make_pair(it->first, it->second);
        }
    }
}

void Solver :: moveWatch(int c)
{
    for(auto it = clauses[c].polarityMap.begin(); it != clauses[c].polarityMap.end(); it++)
    {
        if(clauses[c].watched.find(it->first) != clauses[c].watched.end())
            continue;
        if(graph.find(it->first) == graph.end())
        {
            clauses[c].watched.insert(it->first);
            return;
        }
        if(graph[it->first].value == it->second)
        {
            clauses[c].watched.insert(it->first);
            return;
        }
    }
}

int Solver :: pickClause(int d)
{
    int c;
    bool gotoNext;
    int w0, w1;
    unordered_set<int>::iterator it;
    while(!interestingClauses.empty())
    {
        c = *(interestingClauses.begin());
        interestingClauses.pop_front();
        if(clauses[c].watched.size() == 0)
            moveWatch(c);
        if(clauses[c].watched.size() == 1)
            moveWatch(c);
        if(clauses[c].watched.size() == 0)
        {
            graph.insert(make_pair(-1, Node(true, d)));
            return c;
        }
        if(clauses[c].watched.size() == 1)
        {
            if(graph.find(*(clauses[c].watched.begin())) == graph.end())
                return c;
            if(graph[*(clauses[c].watched.begin())].value == clauses[c].polarityMap[*(clauses[c].watched.begin())])
                continue;
            graph.insert(make_pair(-1, Node(true, d)));
            return c;
        }
        gotoNext = false;
        for(auto it = clauses[c].watched.begin(); it != clauses[c].watched.end(); it++)
            if(graph.find(*it) != graph.end() && graph[*it].value == clauses[c].polarityMap[*it])
                gotoNext = true;
        if(gotoNext)
            continue;
        it = clauses[c].watched.begin();
        w0 = *it;
        it++;
        w1 = *it;
        if(graph.find(w0) != graph.end() && graph[w0].value == clauses[c].polarityMap[w0])
            continue;
        if(graph.find(w1) != graph.end() && graph[w1].value == clauses[c].polarityMap[w1])
            continue;
        if(graph.find(w0) == graph.end())
        {
            if(graph.find(w1) == graph.end())
                continue;
            clauses[c].watched.erase(w1);
            moveWatch(c);
            if(clauses[c].watched.size() == 2)
                continue;
            return c;
        }
        else
        {
            clauses[c].watched.erase(w0);
            moveWatch(c);
            if(clauses[c].watched.size() == 2)
            {
                interestingClauses.push_back(c);
                continue;
            }
            if(graph.find(w1) == graph.end())
                return c;
            interestingClauses.clear();
            graph.insert(make_pair(-1, Node(true, d)));
            return c;
        }
    }
    return -2;
}

void Solver :: unitPropagate(int d)
{
    pair<int, bool> p;
    int clauseNum;
    while(true)
    {
        clauseNum = pickClause(d);
        if(clauseNum == -2)
        {
            return;
        }
        if(graph.find(-1) != graph.end())
        {
            for(auto it = clauses[clauseNum].polarityMap.begin(); it != clauses[clauseNum].polarityMap.end(); it++)
            {
                graph[-1].parents.insert(it->first);
            }
            return;
        }
        p = unitRule(clauses[clauseNum]);
        for(int i = 0; i < clauses.size(); i++)
            if(i != clauseNum && clauses[i].polarityMap.find(p.first) != clauses[i].polarityMap.end())
                interestingClauses.push_back(i);
        graph.insert(make_pair(p.first, Node(p.second, d)));
        for(auto it = clauses[clauseNum].polarityMap.begin(); it != clauses[clauseNum].polarityMap.end(); it++)
        {
            if(it->first == p.first)
                continue;
            if(graph.find(it->first) != graph.end())
            {
                graph[it->first].children.insert(p.first);
                graph[p.first].parents.insert(it->first);
            }
        }
    }
    return;
}

int Solver :: analyzeAndLearn(int d)
{
    queue<int> bfs, temp;
    int nodesAtHighestLevel = 0;
    int current;
    int secondHighestLevel = 0;
    vector<int> literals;
    for(auto it = graph[-1].parents.begin(); it != graph[-1].parents.end(); it++)
    {
        bfs.push(*it);
        if(graph[*it].d == d)
            nodesAtHighestLevel++;
    }
    while(nodesAtHighestLevel > 1)
    {
        current = bfs.front();
        bfs.pop();
        if(graph[current].d == d)
            nodesAtHighestLevel--;
        for(auto it = graph[current].parents.begin(); it != graph[current].parents.end(); it++)
        {
            bfs.push(*it);
            if(graph[*it].d == d)
                nodesAtHighestLevel++;
            else if(graph[*it].d > secondHighestLevel)
                secondHighestLevel = graph[*it].d;
        }
    }
    while(!bfs.empty())
    {
        current = bfs.front();
        bfs.pop();
        if(find(literals.begin(), literals.end(), current) != literals.end() || find(literals.begin(), literals.end(), -1 * current) != literals.end())
            continue;
        if(graph[current].value)
            literals.push_back(-1 * current);
        else
            literals.push_back(current);
    }
    //cout << "Added clause: ";
    //for(int i = 0; i < literals.size(); i++)
    //    cout << literals[i] << " ";
    //cout << endl;
    clauses.push_back(Clause(literals));
    if(secondHighestLevel == 0)
    {
        if(d == 0)
            return -1;
        return 0;
    }
    return secondHighestLevel;
}

void Solver :: backtrack(int d)
{
    unordered_set<int> deleteNodes;
    for(auto it = graph.begin(); it != graph.end(); it++)
    {
        if(graph[it->first].d > d)
        {
            deleteNodes.insert(it->first);
        }
    }
    for(auto it = deleteNodes.begin(); it != deleteNodes.end(); it++)
    {
        graph.erase(*it);
    }
    for(auto it = graph.begin(); it != graph.end(); it++)
    {
        for(auto iter = deleteNodes.begin(); iter != deleteNodes.end(); iter++)
        {
            if(graph[it->first].parents.find(*iter) != graph[it->first].parents.end())
            {
                graph[it->first].parents.erase(*iter);
            }
            else if(graph[it->first].children.find(*iter) != graph[it->first].children.end())
            {
                graph[it->first].children.erase(*iter);
            }

        }
    }
    return;
}

Solver :: ~Solver()
{
}
