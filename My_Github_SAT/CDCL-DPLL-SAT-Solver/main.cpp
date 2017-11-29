#include <iostream>
#include <vector>
#include <cstdio>
#include <ctime>
#include <cstring>
#define _USE_MATH_DEFINES
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <fstream>
#include <sstream>
#include "Clause.h"
#include "Solver.h"
#include "Node.h"
using namespace std;

clock_t start;

int main(int argc, char* argv[])
{
    string inputFile;
    ifstream input;
    int nvar;
    long long int nclauses;
    string token;
    vector<int> literals;
    vector<Clause> clauses;
    istringstream iss;
    double timeElapsed = 0.0;
    pair<int, bool> decision;
    int d = 0;

    input.open(argv[1]);
    if(!input.is_open())
    {
        cout << "The file does not exist or could not be opened" << endl;
        return 1;
    }
    for(string line; getline(input, line, '\n');)
    {
        iss.str(line);
        if(line[0] == 'c' || line[0] == ' ')
            continue;
        if(line[0] == 'p')
        {
            iss >> token;
            iss >> token;
            iss >> token;
            nvar = stoi(token);
            iss >> token;
            nclauses = stoll(token);
        }
        else
        {
            while(getline(iss, token, ' '))
            {
                if(token == "0")
                    break;
                literals.push_back(stoi(token));
            }
            clauses.push_back(Clause(literals));
            literals.clear();
        }
        iss.clear();
    }

    Solver s(nvar, clauses);
    //s.eliminatePureLiterals();
    s.findSingletons();

    start = clock();
    while(true)
    {
        s.unitPropagate(d);
        if(s.graph.find(-1) == s.graph.end())
        {
            if(s.graph.size() == s.nvar)
            {
                cout << "SAT" << endl;
                s.printSatisfiedAssignment();
                break;
            }
            decision = s.makeDecision();
            d++;
            for(int i = 0; i < s.clauses.size(); i++)
            {
                if(s.clauses[i].polarityMap.find(decision.first) != s.clauses[i].polarityMap.end())
                    s.interestingClauses.push_back(i);
            }
            s.graph.insert(make_pair(decision.first, Node(decision.second, d)));
        }
        else
        {
            d = s.analyzeAndLearn(d);
            //cout << "Backtracking to level " << d << endl;
            if(d < 0)
            {
                cout << "UNSAT" << endl;
                break;
            }
            s.backtrack(d);
            s.graph.erase(-1);
        }
        timeElapsed = (clock() - start) / (double) CLOCKS_PER_SEC;
        if(timeElapsed > 30)
        {
            cout << "TIMEOUT" << endl;
            break;
        }
    }
    return 0;
}
