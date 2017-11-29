#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
#include <deque>
#include <unordered_map>
#include "Clause.h"
#include "Node.h"
using namespace std;

class Solver
{
    public:
        int nvar;
        vector<Clause> clauses;
        deque<int> interestingClauses;
        unordered_map<int, Node> graph;
        Solver(int, vector<Clause>);
        void printSatisfiedAssignment();
        void findSingletons();
        pair<int, bool> makeDecision();
        void unitPropagate(int);
        void backtrack(int);
        pair<int, bool> unitRule(Clause);
        int pickClause(int);
        void moveWatch(int);
        int analyzeAndLearn(int);
        ~Solver();
};

#endif
