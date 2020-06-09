#include "simplex.h"
#include <QDebug>

Simplex::Simplex()
{

}

void Simplex::init(const QTableWidget *lim, const QTableWidget *fn)
{
    stdForm(lim,fn);
    createMatrixCoeff();
    qDebug()<<"Simplex Initialized"<<endl;
}



void Simplex::stdForm(const QTableWidget *lim, const QTableWidget *fn)
{
    for (int i = 0; i < fn->columnCount() - 1; ++i)
        originalFn.push_back(fn->item(0, i)->text());

    originalFn.push_back(qobject_cast<QComboBox*>(fn->cellWidget(0, fn->columnCount() - 1))->currentText());// (min/max)

    if (originalFn[originalFn.size() - 1] == "min") {
        for (int i = 0; i < originalFn.size() - 1; ++i)
            standardFunction.push_back(-1 * originalFn[i].toInt());
    }
    else {
        for (int i = 0; i < originalFn.size() - 1; ++i)
            standardFunction.push_back(originalFn[i].toInt());
    }


    for (int i = 0; i < lim->rowCount(); ++i) {
        originalLim.push_back(*new QVector<QString>);
        for (int j = 0; j < lim->columnCount() - 2; ++j)
            originalLim[i].push_back(lim->item(i, j)->text());

        originalLim[i].push_back(qobject_cast<QComboBox*>(lim->cellWidget(i, lim->columnCount() - 2))->currentText());
        originalLim[i].push_back(lim->item(i, lim->columnCount() - 1)->text());
    }



    // Reduce the inputs to the standard matrix form
    for (int i = 0; i < lim->rowCount(); ++i) {
        standardLim.push_back(*new QVector<int>);
        if (originalLim[i][originalLim[0].size() - 1][0] == '-') {
            // if in the row 'i' of the last column of the matrix there is an element that starts with minus ('-'), then we consider that this element is negative
            for (int j = 0; j < originalLim[0].size() - 2; ++j)
                standardLim[i].push_back(-1 * originalLim[i][j].toInt());

            standardLim[i].push_back(-1 * originalLim[i][originalLim[0].size() - 1].toInt());
            // add the last element (element, after the equal sign (≥, ≤, =))
            if (originalLim[i][originalLim[0].size() - 2] == " ≥ ") {
                additionalVars.push_back(1);
                ++numAdditionalVars;
            }
            else if (originalLim[i][originalLim[0].size() - 2] == " ≤ ") {
                additionalVars.push_back(-1);
                ++numAdditionalVars;
            }
            else
                additionalVars.push_back(0);
        }
        else {
            for (int j = 0; j < originalLim[0].size() - 2; ++j)
                standardLim[i].push_back(originalLim[i][j].toInt());

            standardLim[i].push_back(originalLim[i][originalLim[0].size() - 1].toInt());
            // add the last element (element, after the equal sign (≥, ≤, =))
            if (originalLim[i][originalLim[0].size() - 2] == " ≥ ") {
                additionalVars.push_back(-1);
                ++numAdditionalVars;
            }
            else if (originalLim[i][originalLim[0].size() - 2] == " ≤ ") {
                additionalVars.push_back(1);
                ++numAdditionalVars;
            }
            else
                additionalVars.push_back(0);
        }
    }
    qDebug()<<"Standard Form"<<endl;
}

void Simplex::createMatrixCoeff()
{
    for (int i = 0; i < standardLim.size(); ++i)
            matrixCoeff.push_back(*new QVector<int>());

        for (int i = 0; i < matrixCoeff.size(); ++i)
            for (int j = 0; j < standardLim[0].size() - 1; ++j)
                matrixCoeff[i].push_back(standardLim[i][j]);

        for (int i = 0; i < standardFunction.size(); ++i)
            allVars.push_back(qMakePair(standardFunction[i], "x" + QString::number(i + 1)));

        for (int i = 0; i < additionalVars.size(); ++i) {
            static int addVarCount = 1;
            if (additionalVars[i] != 0) {
                for (int j = 0; j < matrixCoeff.size(); ++j)
                    matrixCoeff[j].push_back(0);
                matrixCoeff[i][matrixCoeff[0].size() - 1] = additionalVars[i];
                allVars.push_back(qMakePair(0, "u" + QString::number(addVarCount++)));
            }
        }
        //Lamdda function to find basis row
        auto findBasisRow = [=] (const int &row_Where) {
            for (int i = 0; i < standardLim[0].size() - 1; ++i) {
                if (matrixCoeff[row_Where][i] == 1) {
                    for (int j = 0; j < matrixCoeff.size(); ++j) {
                        if (row_Where == j) continue;
                        if (matrixCoeff[j][i] != 0)
                            goto next;
                    }
                    return i;
                }
            next:
                continue;
            }
            return -1;
        };

        for (int i = 0; i < standardLim.size(); ++i) {
            if (findBasisRow(i) == -1 && additionalVars[i] != 1) {
                static int wVarCount = 1;
                for (int j = 0; j < matrixCoeff.size(); ++j)
                    matrixCoeff[j].push_back(0);

                matrixCoeff[i][matrixCoeff[0].size() - 1] = 1;
                allVars.push_back(qMakePair(-1, "W" + QString::number(wVarCount++)));
            }
        }
        artificialBasis = allVars[allVars.size() - 1].second[0] == 'W';
        qDebug()<<"Coefficients Created "<<endl;
}
