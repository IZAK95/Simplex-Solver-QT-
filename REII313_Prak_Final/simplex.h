#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

class Simplex
{
public:
    Simplex(std::vector <std::vector<float> > matrix,std::vector<float> b ,std::vector<float> c);
    bool simplexAlgorithmCalculation();
    bool checkOptimality();
    void doPivotting(int pivotRow, int pivotColumn);
    void print();
    int findPivotColumn();
    int findPivotRow(int pivotColumn);
    void CalculateSimplex();

private:
    int rows, cols;
    //stores coefficients of all the variables
    std::vector <std::vector<float> > A;
    //stores constants of constraints
    std::vector<float> B;
    //stores the coefficients of the objective function
    std::vector<float> C;
    float maximum;
    bool isUnbounded;
};

#endif // SIMPLEX_H
