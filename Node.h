#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<unordered_set>
using namespace std;

class Node
{
    public:
        bool value;
        int d;
        unordered_set<int> children;
        unordered_set<int> parents;
        Node();
        Node(bool, int);
};

#endif
