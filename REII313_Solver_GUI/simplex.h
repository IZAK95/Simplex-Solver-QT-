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
};

#endif // SIMPLEX_H
