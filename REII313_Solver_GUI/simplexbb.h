#ifndef SIMPLEXBB_H
#define SIMPLEXBB_H
using namespace std;
#include <iostream>
#include "vector"
#include <bits/stdc++.h>

class SimplexBB
{



public:
    SimplexBB();

    int rows;
    int cols;

    vector <vector<float>> A;
    vector <float> B;
    vector <float> C;
    vector <float> BasicSolution;
    vector <float> Solution;
    vector <float> BaseIndex;

    float ObjectifFunction = 0;
    string problem ;

    bool towPhase = false;
    bool realisable = true;

    vector<string> Inegalite;

    vector <float> W;
    float ObjW ;

    SimplexBB(vector <vector<float>>a, vector <float>b, vector <float>c, vector<string> ing, string P);

    void Solve();

    void StandarForme();

    bool PhaseOne();

    void PivotageSimplex();

    bool ChekOptimalSimplex(vector <float> T);



    int* GetPivot(vector <float> G);

    int IndexMinPositive(vector <float> V);

    int IndexMinPositiveStrinct(vector <float> V);

    void print();

    void printff();

    void printtff();
};

#endif // SIMPLEXBB_H
