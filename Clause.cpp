#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "Clause.h"
using namespace std;

Clause :: Clause(vector<int> literals)
{
    int n = literals.size();
    int var;
    bool polarity;
    for(int i = 0; i < n; i++)
    {
        var = abs(literals[i]);
        polarity = (literals[i]/var == 1 ? true : false);
        polarityMap[var] = polarity;
        if(watched.size() < 2)
            watched.insert(var);
    }
}

Clause :: ~Clause()
{
}
