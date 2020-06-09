#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <QVector>
#include <QTableWidget>
#include <QComboBox>


class Simplex
{


public:
    Simplex();
    void init(const QTableWidget *lim,const QTableWidget *fn);
    void stdForm(const QTableWidget *lim, const QTableWidget *fn);
    QVector<QString> originalFn;
    QVector<int> standardFunction;
    QVector<QVector<QString>> originalLim;
    QVector<QVector<int>> standardLim;
    QVector<int> additionalVars;
    int numAdditionalVars = 0;
    QVector<QVector<int>> matrixCoeff;
    void createMatrixCoeff();
    QVector<QPair<int, QString>> allVars;
    bool artificialBasis = false;
};

#endif // SIMPLEX_H
