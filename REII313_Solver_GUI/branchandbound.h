#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H
#include "simplexbb.h"

class BranchandBound
{
public:

    int rows;
    int cols;

    int Max ;
    vector<float> SolInteger;
    SimplexBB PL ;//= new Simplex();

    vector <vector<float>> A;
    vector <float> B;
    vector <float> C;
    vector <string> Inegalite;

    string problem;
    BranchandBound(){}

    BranchandBound(vector <vector<float>>a, vector <float>b, vector <float>c, vector<string> ing, string P);


   bool ChekIntegerSolution(vector<float>S);

   void CalculBranchAndBound(vector <vector<float>>a, vector <float>b, vector <float>c, vector<string> ing, string p);


   void print();

};

#endif // BRANCHANDBOUND_H
