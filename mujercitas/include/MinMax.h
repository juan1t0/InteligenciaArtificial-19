#ifndef MINMAX_H
#define MINMAX_H
#include <vector>
#include <utility>
#include <iostream>
using namespace std;

typedef vector<vector<char> > Tablero;

class MinMax
{
    int depth, maxDepth, val;
    bool Max;
    Tablero key;
    vector<MinMax*> sons;
public:
    MinMax(Tablero b, int d, int md, bool m);
    void evaluate();
    Tablero choose();
    virtual ~MinMax();
};

#endif // MINMAX_H
