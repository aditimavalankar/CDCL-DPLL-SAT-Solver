#include <iostream>
#include "Node.h"

using namespace std;

Node :: Node()
{
}

Node :: Node(bool assignedValue,int depth)
{
    value = assignedValue;
    d = depth;
}

