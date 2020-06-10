#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <QVector>
#include <QTableWidget>
#include <QComboBox>
#include "fraction.h"

class Simplex
{


public:
    Simplex();
    void init(const QTableWidget *lim,const QTableWidget *fn);
    void stdForm(const QTableWidget *lim, const QTableWidget *fn);
    void createMatrixCoeff();
    bool shouldRemoveColumns = false;
    bool isOptimal() const;
    bool isExtraProblem() const;
    int getCurrentGuidingRow() const;
    int getCurrentGuidingColumn() const;
    int guiding_row;                                 //Reference line.
    int guiding_column;     //Reference column.

    int  findGuidingRow(const int &guidingColumn) const;
    int  findGuidingColumn() const;

    QString getAnswer() const;
    void prepare();

    enum  class SolveResult { UNSOLVED, SOLVED, UNLIMITED, NO_SOLUTIONS };

    SolveResult generateNextSimplexMatrix();

    SolveResult getCurrentSolveStatus() const;

    QVector<QString> originalFn;
    QVector<int> standardFunction;

    QVector<QVector<QString>> originalLim;
    QVector<QVector<int>> standardLim;

    QVector<int> additionalVars;
    int numAdditionalVars = 0;

    QVector<QVector<int>> matrixCoeff;

    QVector<QVector<int>>getMatrixCoeff() const;

    QVector<QPair<int, QString>> allVars;
    bool artificialBasis = false;
    QVector<QPair<int, QString>> getAllVars() const;

    QString genStdLimAsString() const;
    QVector<QPair<int, QString>> basis;              //Base variables: (Variable value, variable name).
    QVector<QPair<int, QString>> unbasis;            //Non-basic variables: (Variable value, variable name).

    void removeColumn();

    void calculateMainBasis();
    QVector<QPair<int, QString>> getBasis() const;
    QVector<QPair<int, QString>> getUnbasis() const;
    QVector<QVector<Fraction>>   getMatrix() const;
    QVector<QVector<Fraction>> matrix;
    void recalculationOfTable(const int &guiding_row, const int &guiding_column);

private:

    SolveResult solveStatus = SolveResult::UNSOLVED;
    SolveResult solve();

};



#endif // SIMPLEX_H
