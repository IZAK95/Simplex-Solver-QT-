#include "simplex.h"
///Public------------------------------------------------------------------------------------------------------------------------------------
Simplex::Simplex()
{

}

// initialization function. It is this method that calls the functions of creating a matrix of coefficients, etc
void Simplex::initialize(const QTableWidget *limitations, const QTableWidget *function)
{
    leadToCanonical(limitations, function);
    createMatrixCoeff();
    fillMatrixByBasisAndUnbasis();
    if (artificialBasis)
        calculateSlackBasis();
    else
        calculateMainBasis();
    prepare();
}

// Generates the following simplex matrix.
Simplex::SolveResult Simplex::createNextSimplexMatrix()
{
    if (solveStatus != Simplex::SolveResult::UNSOLVED)
        return solveStatus;

    if (shouldRemoveColumns) {
        deleteColumn();
        calculateMainBasis();
        shouldRemoveColumns = false;
    }
    else {
        qSwap(unbasis[guiding_column], basis[guidingRow]);
        recalculationOfTable(guidingRow, guiding_column);
    }
    prepare();
    return Simplex::SolveResult::UNSOLVED;
}
//*********************************************************************************************************************************************************************
Simplex::SolveResult Simplex::getCurrentSolveStatus() const
{
    return solveStatus;
}
//*********************************************************************************************************************************************************************
QVector<QPair<int, QString>> Simplex::getBasis() const
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
//*********************************************************************************************************************************************************************
QVector<QPair<int, QString>> Simplex::getUnbasis() const
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
//*********************************************************************************************************************************************************************
QVector<QVector<int>> Simplex::getMatrixCoefficients() const
{
    return matrixCoefficients;
}
//*********************************************************************************************************************************************************************
QVector<QPair<int, QString>> Simplex::getAllVars() const
{
    return allVariables;
}
//*********************************************************************************************************************************************************************
QVector<QVector<Fraction>> Simplex::getMatrix() const
{
    return matrix;
}
//*********************************************************************************************************************************************************************
int Simplex::getCurrentGuidingRow() const
{
    return guidingRow;
}
//*********************************************************************************************************************************************************************
int Simplex::getCurrentGuidingColumn() const
{
    return guiding_column;
}
//*********************************************************************************************************************************************************************
QString Simplex::getAnswer() const
{
    switch (solveStatus) {
    case SolveResult::SOLVED:
        return "The problem is solved. And the optimal solution is displayed. Please note that the results are displayed in the Table Above.\n "
               "If the user clicks on the last column, it will highlight the corresponding variable. \nPlease note that the answer is given in terms of S (Slack variable) and it will correspond"
               "to an x value which can be determined by the user";
    case SolveResult::UNLIMITED:
        return "The function is not limited to the set of feasible solutions";
    case SolveResult::NO_SOLUTIONS:
        return " The value of the function is positive and there are artificial variables in the basis.\n"
               "The original problem is unsolvable "
               "because of the inconsistency of the system of restrictions..";
    case SolveResult::UNSOLVED:
        return "The problem is not solved";
    }
}
//*********************************************************************************************************************************************************************
QString Simplex::generateString() const
{
    auto getSign = [](const QString &source) {
        if (source[0] == '-')
            return " - " + source.right(source.length() - 1);
        return " + " + source;
    };
    QString s;
    int uCounter = 0;
    s += QString::number(canonicalFunction[0]) + "x<sub>1</sub>";
    for (int i = 1; i < canonicalFunction.size(); ++i)
        s += getSign(QString::number(canonicalFunction[i])) + "x<sub>" + QString::number(i + 1) + "</sub>";

    s += " —> max<br>";
    for (int i = 0; i < canonicalLimitations.size(); ++i)
    {
        s += QString::number(canonicalLimitations[i][0]) + "x<sub>1</sub>";
        for (int j = 1; j < canonicalLimitations[0].size() - 1; ++j)
            s += getSign(QString::number(canonicalLimitations[i][j])) + "x<sub>" + QString::number(j + 1) + "</sub>";

        if (u_variables[i] != 0) {
            if (u_variables[i] == 1)
                s += " + S<sub>" + QString::number(++uCounter) + "</sub>";
            else s += " - S<sb>" + QString::number(++uCounter) + "</sub>";
        }
        s += " = " + QString::number(canonicalLimitations[i][canonicalLimitations[0].size() - 1]) + "<br>";
        qDebug()<<"Canonical Lims"<<canonicalLimitations[i]<<endl;
        canonicalLimitations[i].value(1);

    }
    return s;
}
//*********************************************************************************************************************************************************************
/// Private------------------------------------------------------------------------------------------------------------------------------------

//Initializng the simplex table,running checks and searching referance rows and columns.
void Simplex::prepare()
{
    if (isOptimal()) {
        guidingRow = -1;
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
    guiding_column = findGuidingColumn();               //Reference Column
    guidingRow = findGuidingRow(guiding_column);        //Referance Row
    if (guidingRow == -1)
        solveStatus = SolveResult::UNLIMITED;
    else
        solveStatus = SolveResult::UNSOLVED;
}

//*********************************************************************************************************************************************************************
// Reduction to canonical/standard from form.
void Simplex::leadToCanonical(const QTableWidget *limitations, const QTableWidget *function)
{
    for (int i = 0; i < function->columnCount() - 1; ++i)
        originalFunction.push_back(function->item(0, i)->text());

    originalFunction.push_back(qobject_cast<QComboBox*>(function->cellWidget(0, function->columnCount() - 1))->currentText());// Decide to use min/max

    if (originalFunction[originalFunction.size() - 1] == "min") {
        for (int i = 0; i < originalFunction.size() - 1; ++i)
            canonicalFunction.push_back(-1 * originalFunction[i].toInt());
    }
    else {
        for (int i = 0; i < originalFunction.size() - 1; ++i)
            canonicalFunction.push_back(originalFunction[i].toInt());
    }
    qDebug()<<"Original Fn "<<originalFunction<<endl;
    qDebug()<<"Canonical Fn "<<canonicalFunction<<endl;

    for (int i = 0; i < limitations->rowCount(); ++i) {
        originalLimitations.push_back(*new QVector<QString>);
        for (int j = 0; j < limitations->columnCount() - 2; ++j)
            originalLimitations[i].push_back(limitations->item(i, j)->text());

        originalLimitations[i].push_back(qobject_cast<QComboBox*>(limitations->cellWidget(i, limitations->columnCount() - 2))->currentText());
        originalLimitations[i].push_back(limitations->item(i, limitations->columnCount() - 1)->text());

        qDebug()<<"Original Lim "<<originalLimitations<<endl;

        //    qDebug()<<"Original Test "<<endl;
    }

    //****************************************************************************************************************************************************************

    // Reduce to the canonical/standard form.
    for (int i = 0; i < limitations->rowCount(); ++i)
    {
        canonicalLimitations.push_back(*new QVector<int>);
        if (originalLimitations[i][originalLimitations[0].size() - 1][0] == '-')
        {
            // Check the last column's element responding to iteration to see if its negative
            for (int j = 0; j < originalLimitations[0].size() - 2; ++j)
                canonicalLimitations[i].push_back(-1 * originalLimitations[i][j].toInt());

            canonicalLimitations[i].push_back(-1 * originalLimitations[i][originalLimitations[0].size() - 1].toInt());
            // add the last element (element, after the equal sign (≥, ≤, =))
            if (originalLimitations[i][originalLimitations[0].size() - 2] == " ≥ ")
            {
                u_variables.push_back(1);
                ++countOfExtraVariables;
            }
            else if (originalLimitations[i][originalLimitations[0].size() - 2] == " ≤ ")
            {
                u_variables.push_back(-1);
                ++countOfExtraVariables;
            }
            else
                u_variables.push_back(0);
        }
        else
        {
            for (int j = 0; j < originalLimitations[0].size() - 2; ++j)
                canonicalLimitations[i].push_back(originalLimitations[i][j].toInt());

            canonicalLimitations[i].push_back(originalLimitations[i][originalLimitations[0].size() - 1].toInt());
            // add  element after the operator (≥, ≤, =))
            if (originalLimitations[i][originalLimitations[0].size() - 2] == " ≥ ")
            {
                u_variables.push_back(-1);
                ++countOfExtraVariables;
            }
            else if (originalLimitations[i][originalLimitations[0].size() - 2] == " ≤ ")
            {
                u_variables.push_back(1);
                ++countOfExtraVariables;
            }
            else
                u_variables.push_back(0);
        }
    }
    qDebug()<<"Reduced to Standard Form"<<endl;
}
//**************************************************************************************************************************************

// Create a matrix of coefficients that will be implemented later on again.

void Simplex::createMatrixCoeff()
{
    qDebug()<<"Creating Coefficients"<<endl;
    for (int i = 0; i < canonicalLimitations.size(); ++i)
        matrixCoefficients.push_back(*new QVector<int>());

    for (int i = 0; i < matrixCoefficients.size(); ++i)
        for (int j = 0; j < canonicalLimitations[0].size() - 1; ++j)
            matrixCoefficients[i].push_back(canonicalLimitations[i][j]);

    for (int i = 0; i < canonicalFunction.size(); ++i)
        allVariables.push_back(qMakePair(canonicalFunction[i], "x" + QString::number(i + 1)));

    for (int i = 0; i < u_variables.size(); ++i) {
        static int uVarCount = 1;
        if (u_variables[i] != 0) {
            for (int j = 0; j < matrixCoefficients.size(); ++j)
                matrixCoefficients[j].push_back(0);
            matrixCoefficients[i][matrixCoefficients[0].size() - 1] = u_variables[i];
            allVariables.push_back(qMakePair(0, "S" + QString::number(uVarCount++)));
        }
    }

    auto findBasisRow = [=] (const int &row_Where) {
        for (int i = 0; i < canonicalLimitations[0].size() - 1; ++i) {
            if (matrixCoefficients[row_Where][i] == 1) {
                for (int j = 0; j < matrixCoefficients.size(); ++j) {
                    if (row_Where == j) continue;
                    if (matrixCoefficients[j][i] != 0)
                        goto next;
                }
                return i;
            }
next:
            continue;
        }
        return -1;
    };

    for (int i = 0; i < canonicalLimitations.size(); ++i) {
        if (findBasisRow(i) == -1 && u_variables[i] != 1) {
            static int wVarCount = 1;
            for (int j = 0; j < matrixCoefficients.size(); ++j)
                matrixCoefficients[j].push_back(0);

            matrixCoefficients[i][matrixCoefficients[0].size() - 1] = 1;
            allVariables.push_back(qMakePair(-1, "W" + QString::number(wVarCount++)));
        }
    }
    artificialBasis = allVariables[allVariables.size() - 1].second[0] == 'W';
}
//****************************************************************************************************************************************************************

// Fill table with basic and non-basic variables
void Simplex::fillMatrixByBasisAndUnbasis()
{
    qDebug()<<"Filling matrix by basis and unbasis"<<endl;
    auto isBasisColumn = [=](const int &columnIndex)
    {
        int c1 = 0;
        int c0 = 0;
        for (int i = 0; i < matrixCoefficients.size(); ++i) {
            if (matrixCoefficients[i][columnIndex] == 1)
                ++c1;
            else
                if (matrixCoefficients[i][columnIndex] == 0)
                    ++c0;
        }
        return (c1 == 1 && c0 == matrixCoefficients.size() - 1);
    };

    for (int i = 0; i < matrixCoefficients.size(); ++i)
        matrix.push_back(*new QVector<Fraction>());

    for (int i = 0; i < matrixCoefficients[0].size(); ++i) {
        if (isBasisColumn(i)) {                             // If the i-th column of the coefficient matrix is ​​basic,
            basis.push_back(allVariables[i]);               // then add it to the basis.
        }
        else {
            // otherwise, it is considered to be a non-base column.
            unbasis.push_back(allVariables[i]);
            // add it to non-base variables.
            for (int k = 0; k < matrixCoefficients.size(); ++k)
                // and write to the main matrix
                matrix[k].push_back(matrixCoefficients[k][i]);
        }
    }
    unbasis.push_back(qMakePair(0, ' '));

    for (int i = 0; i < matrix.size(); ++i)
        matrix[i].push_back(canonicalLimitations[i][canonicalLimitations[0].size() - 1]);

    matrix.push_back(*new QVector<Fraction>());
    matrix[matrix.size() - 1].resize(matrix[0].size());
}

//****************************************************************************************************************************************************************
// Get the initial values ​​of the evaluation string and the objective function.
void Simplex::calculateMainBasis()
{
    qDebug()<<"Calculating Main Basis"<<endl;
    for (int i = 0; i < matrix[0].size(); ++i) {
        Fraction value(0);
        for (int j = 0; j < basis.size(); ++j)
            value += basis[j].first * matrix[j][i];

        value -= unbasis[i].first;
        matrix[matrix.size() - 1][i] = value;
    }
}
//****************************************************************************************************************************************************************
// If an artificial basis is used, reset other variables to zero
void Simplex::calculateSlackBasis()
{   qDebug()<<"Calculating Artificial Variables Basis"<<endl;
    for (int i = 0; i < matrix[0].size(); ++i) {
        int value = 0;
        for (int j = 0; j < basis.size(); ++j) {
            if (basis[j].second[0] == 'W')
                value += basis[j].first * matrix[j][i].getNumerator();
        }
        matrix[matrix.size() - 1][i] = value;
    }
}

//****************************************************************************************************************************************************************
//Find the reference Row.
int Simplex::findGuidingRow(const int &guidingColumn) const
{   qDebug()<<"Searching reference row"<<endl;
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

//****************************************************************************************************************************************************************
// Find the reference column.
int Simplex::findGuidingColumn() const
{   qDebug()<<"Searching Reference Column..."<<endl;
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

//****************************************************************************************************************************************************************
// Check the plan for optimality.
bool Simplex::isOptimal() const
{
    for (int i = 0; i < matrix[0].size() - 1; ++i)
        if (matrix[matrix.size() - 1][i] < 0)
            return false;
    return true;
    qDebug()<<"Optimality Checked"<<endl;
}
//****************************************************************************************************************************************************************

// Verifies if slack variables removed
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
    qDebug()<<"Slack Variables Removed"<<endl;
}
//****************************************************************************************************************************************************************

// Delete columns with artificial variables.
void Simplex::deleteColumn()
{
    for (int i = 0; i < matrix[0].size() - 1; ++i) {
        if (matrix[matrix.size() - 1][i].getNumerator() == 1) {
            for (int j = 0; j < matrix.size(); ++j)
                matrix[j].erase(matrix[j].begin() + i);

            unbasis.erase(unbasis.begin() + i);
            --i;
        }
    }
    qDebug()<<"Delete Column"<<endl;
}
//****************************************************************************************************************************************************************

// Recalculation of the simplex table.
void Simplex::recalculationOfTable(const int &guidingRow, const int &guiding_column)
{
    QVector<QVector<Fraction>> temp;
    for (int i = 0; i < matrix.size(); ++i)
        temp.push_back(*new QVector<Fraction>(matrix[0].size()));

    Fraction resElem = matrix[guidingRow][guiding_column];
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            if (i == guidingRow && j == guiding_column) {
                temp[i][j] = 1 / resElem;
                continue;
            }
            if (i == guidingRow) {
                temp[i][j] = matrix[i][j] / resElem;
                continue;
            }
            if (j == guiding_column) {
                temp[i][j] = -1 * matrix[i][j] / resElem;
                continue;
            }
            temp[i][j] = ((resElem * matrix[i][j]) - (matrix[i][guiding_column] * matrix[guidingRow][j])) / resElem;
        }
    }
    for (int i = 0; i < matrix.size(); ++i)
        for (int j = 0; j < matrix[0].size(); ++j)
            matrix[i][j] = temp[i][j];
    qDebug()<<"Recalculated Table"<<endl;

}
