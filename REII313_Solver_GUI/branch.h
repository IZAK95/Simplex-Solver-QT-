#ifndef BRANCH_H
#define BRANCH_H
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;
//#include "simplexbb.h"

class Branch
{
public:

    int rows;
    int cols;

    int Max ;
    vector<float> SolInteger;
    //    SimplexBB PL ;//= new Simplex();

    vector <vector<float>> A;
    vector <float> B;
    vector <float> C;
    vector <string> Inegalite;

    string problem;

    Branch(vector <vector<float>>a, vector <float>b, vector <float>c, vector<string> ing, string P);


    bool ChekIntegerSolution(vector<float>S);

    void CalculBranchAndBound(vector <vector<float>>a, vector <float>b, vector <float>c, vector<string> ing, string p);


    void print();
};

#endif // BRANCH_H
