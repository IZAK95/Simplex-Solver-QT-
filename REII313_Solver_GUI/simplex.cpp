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

bool Simplex::isOptimal() const
{
    for (int i = 0; i < matrix[0].size() - 1; ++i)
        if (matrix[matrix.size() - 1][i] < 0)
            return false;
    return true;
}

bool Simplex::isExtraProblem() const
{
    bool isOneFound = false;
    for (int i = 0; i < matrix[0].size() - 1; ++i) {
        if (matrix[matrix.size() - 1][i] == 1) {
            isOneFound = true;
            break;
        }
    }
    if (!isOneFound || matrix[matrix.size() - 1][matrix[0].size() - 1] != 0)
        return false;
    return true;
}

int Simplex::getCurrentGuidingRow() const
{
    return guiding_row;
}

int Simplex::getCurrentGuidingColumn() const
{
    return guiding_column;
}

int Simplex::findGuidingRow(const int &guidingColumn) const
{
    Fraction min(INT_MAX);
    int INDEX = -1;
    for (int i = 0; i < matrix.size() - 1; ++i) {
        if (matrix[i][guidingColumn].getNumerator() > 0) {
            if (matrix[i][matrix[0].size() - 1] / matrix[i][guidingColumn] < min) {
                min = matrix[i][matrix[0].size() - 1] / matrix[i][guidingColumn];
                INDEX = i;
            }
        }
    }
    return INDEX;
}

int Simplex::findGuidingColumn() const
{
    Fraction min(-1, 10000000000);
    int INDEX = -1;
    for (int i = 0; i < matrix[0].size() - 1; ++i) {
        if (matrix[matrix.size() - 1][i] < min) {
            min = matrix[matrix.size() - 1][i];
            INDEX = i;
        }
    }
    return  INDEX;
}

QString Simplex::getAnswer() const
{
    switch (solveStatus) {
    case SolveResult::SOLVED:
        return "The problem is solved.";
    case SolveResult::UNLIMITED:
        return "The function is not limited to the set of feasible solutions; it is impossible to find the optimal plan.";
    case SolveResult::NO_SOLUTIONS:
        return " The value of the function is positive and there are artificial variables in the basis.\n"
               "The original problem is unsolvable "
               "because of the inconsistency of the system of restrictions..";
    case SolveResult::UNSOLVED:
        return "The problem is not solved";
    }
}

void Simplex::prepare()
{
    if (isOptimal()) {
        guiding_row = -1;
        guiding_column = -1;
        if (artificialBasis) {
            if (isExtraProblem()) {
                shouldRemoveColumns = true;
                solveStatus = SolveResult::UNSOLVED;
                artificialBasis = false;
                return;
            }
            else {
                solveStatus = SolveResult::NO_SOLUTIONS;
                return;
            }
        }
        solveStatus = SolveResult::SOLVED;
        return;
    }
    guiding_column = findGuidingColumn();
    guiding_row = findGuidingRow(guiding_column);
    if (guiding_row == -1)
        solveStatus = SolveResult::UNLIMITED;
    else
        solveStatus = SolveResult::UNSOLVED;
}

Simplex::SolveResult Simplex::generateNextSimplexMatrix()
{
    if (solveStatus != Simplex::SolveResult::UNSOLVED)
        return solveStatus;

    if (shouldRemoveColumns) {
        removeColumn();
        calculateMainBasis();
        shouldRemoveColumns = false;
    }
    else {
        qSwap(unbasis[guiding_column], basis[guiding_row]);
        recalculationOfTable(guiding_row, guiding_column);
    }
    prepare();
    return Simplex::SolveResult::UNSOLVED;
}

Simplex::SolveResult Simplex::getCurrentSolveStatus() const
{
    return solveStatus;
}

QVector<QVector<int> > Simplex::getMatrixCoeff() const
{
    return matrixCoeff;
}

QVector<QPair<int, QString> > Simplex::getAllVars() const
{
    return allVars;
}

QString Simplex::genStdLimAsString() const
{
    auto getSign = [](const QString &source) {
            if (source[0] == '-')
                return " - " + source.right(source.length() - 1);
           return " + " + source;
        };
        QString s;
        int uCounter = 0;
        s += QString::number(standardFunction[0]) + "x<sub>1</sub>";
        for (int i = 1; i < standardFunction.size(); ++i)
            s += getSign(QString::number(standardFunction[i])) + "x<sub>" + QString::number(i + 1) + "</sub>";

        s += " —> max<br>";
        for (int i = 0; i < standardLim.size(); ++i) {
            s += QString::number(standardLim[i][0]) + "x<sub>1</sub>";
            for (int j = 1; j < standardLim[0].size() - 1; ++j)
                s += getSign(QString::number(standardLim[i][j])) + "x<sub>" + QString::number(j + 1) + "</sub>";

            if (additionalVars[i] != 0) {
                if (additionalVars[i] == 1)
                    s += " + u<sub>" + QString::number(++uCounter) + "</sub>";
                else s += " - u<sub>" + QString::number(++uCounter) + "</sub>";
            }
            s += " = " + QString::number(standardLim[i][standardLim[0].size() - 1]) + "<br>";
        }
        return s;
}

void Simplex::removeColumn()
{
    for (int i = 0; i < matrix[0].size() - 1; ++i) {
        if (matrix[matrix.size() - 1][i].getNumerator() == 1) {
            for (int j = 0; j < matrix.size(); ++j)
                matrix[j].erase(matrix[j].begin() + i);

            unbasis.erase(unbasis.begin() + i);
             --i;
        }
    }
}

void Simplex::calculateMainBasis()
{
    {
        for (int i = 0; i < matrix[0].size(); ++i) {
            Fraction value(0);
            for (int j = 0; j < basis.size(); ++j)
                value += basis[j].first * matrix[j][i];

            value -= unbasis[i].first;
            matrix[matrix.size() - 1][i] = value;
        }
}

}
QVector<QPair<int, QString> > Simplex::getBasis() const
{
    if (artificialBasis || shouldRemoveColumns) {
        QVector<QPair<int, QString>> tbasis = basis;
        for (auto &i : tbasis) {
            if (i.second[0] != 'W')
                i.first = 0;
        }
        return tbasis;
    }
    return basis;
}

QVector<QPair<int, QString> > Simplex::getUnbasis() const
{
    if (artificialBasis || shouldRemoveColumns) {
        QVector<QPair<int, QString>> tunbasis = unbasis;
        for (auto &i : tunbasis) {
            if (i.second[0] != 'W')
                i.first = 0;
        }
        return tunbasis;
    }
    return  unbasis;
}

QVector<QVector<Fraction> > Simplex::getMatrix() const
{
    return matrix;
}

void Simplex::recalculationOfTable(const int &guiding_row, const int &guiding_column)
{
    QVector<QVector<Fraction>> temp;
    for (int i = 0; i < matrix.size(); ++i)
        temp.push_back(*new QVector<Fraction>(matrix[0].size()));

    Fraction resElem = matrix[guiding_row][guiding_column];
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            if (i == guiding_row && j == guiding_column) {
                temp[i][j] = 1 / resElem;
                continue;
            }
            if (i == guiding_row) {
                temp[i][j] = matrix[i][j] / resElem;
                continue;
            }
            if (j == guiding_column) {
                temp[i][j] = -1 * matrix[i][j] / resElem;
                continue;
            }
            temp[i][j] = ((resElem * matrix[i][j]) - (matrix[i][guiding_column] * matrix[guiding_row][j])) / resElem;
        }
    }
    for (int i = 0; i < matrix.size(); ++i)
        for (int j = 0; j < matrix[0].size(); ++j)
            matrix[i][j] = temp[i][j];
}
