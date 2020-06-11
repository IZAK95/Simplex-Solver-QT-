#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <QVector>
#include <QString>
#include <QPair>
#include <QTableWidget>
#include <QComboBox>
#include "fraction.h"
#include <QDebug>
class Simplex
{
    /* Designation:
         * x - variables for the objective function.
         * u are additional variables.
         * W are artificial variables.
        */
    QVector<QString> originalFunction;
    QVector<QVector<QString>> originalLimitations;
    QVector<int> canonicalFunction;
    QVector<QVector<int>> canonicalLimitations;
    QVector<int> testVector;

    QVector<QVector<Fraction>> matrix;
    QVector<QPair<int, QString>> basis;
    QVector<QPair<int, QString>> unbasis;
    QVector<QPair<int, QString>> allVariables;
    QVector<QVector<int>> matrixCoefficients;
    QVector<int> u_variables;
    int countOfExtraVariables = 0;
    bool artificialBasis = false;
    int guidingRow;
    int guiding_column;
    bool shouldRemoveColumns = false;
    // Flag that signals the need to remove columns with artificial variables.

    void leadToCanonical(const QTableWidget *limitations, const QTableWidget *function);//We reduce the constraints and the objective function to the canonical form
    void createMatrixCoeff();
    void fillMatrixByBasisAndUnbasis();
    void calculateMainBasis();
    void calculateSlackBasis();
    int  findGuidingRow(const int &guidingColumn) const;
    int  findGuidingColumn() const;
    bool isOptimal() const;
    bool isExtraProblem() const;
    void deleteColumn();
    void recalculationOfTable(const int &guiding_row, const int &guiding_column);
    void prepare();

public:
    Simplex();
    enum  class SolveResult { UNSOLVED, SOLVED, UNLIMITED, NO_SOLUTIONS };
    void initialize(const QTableWidget *limitations, const QTableWidget *function);
    SolveResult createNextSimplexMatrix();

    SolveResult                  getCurrentSolveStatus() const;
    QString                      generateString() const;
    QVector<QPair<int, QString>> getBasis() const;
    QVector<QPair<int, QString>> getUnbasis() const;
    QVector<QVector<int>>        getMatrixCoefficients() const;
    QVector<QPair<int, QString>> getAllVars() const;
    QVector<QVector<Fraction>>   getMatrix() const;
    int                          getCurrentGuidingRow() const;
    int                          getCurrentGuidingColumn() const;
    QString                      getAnswer() const;
private:
    SolveResult solveStatus = SolveResult::UNSOLVED;
    SolveResult solve();

};
#endif // SIMPLEX_H
