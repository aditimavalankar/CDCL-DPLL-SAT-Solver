#ifndef CLAUSE_H
#define CLAUSE_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
using namespace std;

class Clause
{
    public:
        unordered_map<int, bool> polarityMap;
        unordered_set<int> watched;
        Clause(vector<int>);
        ~Clause();
};

#endif
