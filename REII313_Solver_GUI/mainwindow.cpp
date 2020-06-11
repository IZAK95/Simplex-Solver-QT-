#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "simplex.h"
#include "branch.h"
#include "simplexbb.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("REII313 Practical");
    Setup();

    qDebug()<<"Init Main Window"<<endl;
}
//*********************************************************************************************************************************************************************
MainWindow::~MainWindow()
{
    delete ui;
}
//*********************************************************************************************************************************************************************

void MainWindow::Setup()
{
     //Define Fonts Used
     QFont DefaultFont("Times",15);
     QFont TitleFont("Times", 35, QFont::Bold);

     //Set BackGround Colours

     QPalette labelQPalette;
     labelQPalette.setColor(QPalette::Window,Qt::white);

     //Set Text Colours
     labelQPalette.setColor(QPalette::WindowText,Qt::blue);

    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(widget);
    //Insert Title
    QLabel *labelTitle = new QLabel("REII313 Practical \nLinear Program Solver",this);
    labelTitle->setMinimumWidth(100);
    labelTitle->setMinimumHeight(100);
    labelTitle->setFont(TitleFont);
    layout->addWidget(labelTitle,0,0);

    //Insert Description
    QLabel *labelDescription = new QLabel();
    labelDescription->setFont(DefaultFont);
    labelDescription->setText("Done by Izak Adendorff (30934672) and Izelle Evert (30238072)");
    layout->addWidget(labelDescription,1,0);
    layout->setVerticalSpacing(10);

    QLabel *labelRowsCount = new QLabel(" Number of limits (rows):", this);
    layout->addWidget(labelRowsCount, 2, 0);

    QSpinBox *spinRowsCount = new QSpinBox(this); // Number of lines (number of restrictions)
    spinRowsCount->setRange(2, 20);

    layout->addWidget(spinRowsCount, 2, 1);

    QLabel *labelColumnsCount = new QLabel("Number of variables (columns):", this);
    layout->addWidget(labelColumnsCount, 3, 0);

    QSpinBox *spinColumnsCount = new QSpinBox(this);
    // Number of columns (number of variables)
    spinColumnsCount->setRange(2, 20);
    layout->addWidget(spinColumnsCount, 3, 1);

    QPushButton *buttonContinue = new QPushButton("Next", this);
    buttonContinue->setMinimumSize(button_minX,button_minY);
    buttonContinue->setMaximumSize(button_maxX,button_maxY);
    layout->addWidget(buttonContinue,4,1);

    widget->move(170, 0);
    widget->adjustSize();
  //  widget->move(minimum_X, minimum_Y);
    widget->show();

    connect(buttonContinue, &QPushButton::clicked, this, [=]() {
        widget->hide();

        MainWindow::GetVal(spinRowsCount->value(), spinColumnsCount->value());
        //Grabs values from the UI and sends it to the TableWidget fn in order to determine table size.
        int r = spinRowsCount->value();
        int c = spinColumnsCount->value();
        qDebug()<<r<<endl;
        qDebug()<<c<<endl;

    });
    qDebug()<<"Setup Complete"<<endl;
}
//*********************************************************************************************************************************************************************
void MainWindow::GetVal(const int &rows, const int &columns)
{
    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(widget);
    layout->setSpacing(0);
    lim = new QTableWidget(this);
    // Constraint Matrix
    TableWidget(lim, rows, columns + 2);

    //Add operators
    for (int i = 0; i < rows; ++i) {
        QComboBox *combo = new QComboBox(this);
        combo->addItem(" ≤ ");
        combo->addItem(" ≥ ");
        combo->addItem(" = ");
        lim->setCellWidget(i, columns, combo);

        for (int i = 0; i < columns; ++i)
            lim->setHorizontalHeaderItem(i, new QTableWidgetItem("x" + QString::number(i + 1)));

        lim->setHorizontalHeaderItem(columns, new QTableWidgetItem("Sign"));
        lim->setHorizontalHeaderItem(columns + 1, new QTableWidgetItem("Equal"));
        layout->addWidget(lim, 0, 0);
        fn = new QTableWidget(this);


        // objective fn
        TableWidget(fn, 1, columns + 1);
        QComboBox *extr = new QComboBox(this);
        extr->addItem("max");
        extr->addItem("min");
        fn->setCellWidget(0, columns, extr);
        for (int i = 0; i < columns; ++i)
            fn->setHorizontalHeaderItem(i, new QTableWidgetItem("x" + QString::number(i + 1)));

        fn->setHorizontalHeaderItem(columns, new QTableWidgetItem("Min/Max"));
        layout->addWidget(fn, 1, 0);

        //Add Next Button
        QPushButton *buttonContinue = new QPushButton("Solve", this);
        buttonContinue->setMinimumSize(button_minX,button_minY);
        buttonContinue->setMaximumSize(button_maxX,button_maxY);
        layout->addWidget(buttonContinue, 3, 0);
        //Add Back Button
        QPushButton *buttonBack = new QPushButton("Back", this);
        buttonBack->setMinimumSize(button_minX,button_minY);
        buttonBack->setMaximumSize(button_maxX,button_maxY);
        layout->addWidget(buttonBack, 2, 0);


        widget->adjustSize();
        widget->move(0, 0);
        widget->show();

        connect(buttonBack, &QPushButton::clicked, this, [=]() {
            widget->hide();
            Setup();
        });



        connect(buttonContinue, &QPushButton::clicked, this, [=]() {              
            if (!MainWindow::Check(lim, fn)) {

                QMessageBox *msg = new QMessageBox();
                msg->warning(this,"Error"," Not all fields are filled in or wrong input. Please enter numbers only.");
                qDebug()<<"Input Error"<<endl;

                return ;
            }
            widget->hide();

            simplex = new Simplex();
            simplex->initialize(lim, fn);
            MainWindow::ShowCanonLimAndMatrixCoeff();


            qDebug()<<"Get values"<<endl;
        });
    }
}
//*********************************************************************************************************************************************************************

//Create QTable with the values entered by the user
void MainWindow::TableWidget(QTableWidget *table, const int &rows, const int &columns)
{
    table->setRowCount(rows);
    table->setColumnCount(columns);
    for (int i = 0; i < columns; ++i)
    {
        table->setColumnWidth(i, tableColumnWidth);
    table->verticalHeader()->setFixedWidth(tableColumnWidth);
    table->horizontalHeader()->setFixedHeight(tableRowHeight + 5);
    table->setFixedSize(table->horizontalHeader()->length() + table->verticalHeader()->width() + 2,
                        table->verticalHeader()->length() + table->horizontalHeader()->height() + 2);
    }
    qDebug()<<"Table created"<<endl;
}

//*********************************************************************************************************************************************************************
//Checks for invalid inputs
bool MainWindow::Check(const QTableWidget *lim, const QTableWidget *fn) const
{
    QRegExp check("-?\\d{1,6}");
    for (int i = 0; i < fn->columnCount() - 1; ++i) {
        if (fn->item(0, i) != nullptr) {
            if (!check.exactMatch(fn->item(0, i)->text()))
                return false;
        }
        else return false;
    }

    for (int i = 0; i < lim->rowCount(); ++i) {
        for (int j = 0; j < lim->columnCount() - 2; ++j) {
            if (lim->item(i, j) != nullptr) {
                if (!check.exactMatch(lim->item(i, j)->text()))
                    return false;
            }
            else return false;
        }
        if (lim->item(i, lim->columnCount() - 1) != nullptr) {
            if (!check.exactMatch(lim->item(i, lim->columnCount() - 1)->text()))
                return false;
        }
        else return false;
    }
    return true;
}

void MainWindow::ShowCanonLimAndMatrixCoeff()
{
    QWidget *widget = new QWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    QTextEdit *textEdit = new QTextEdit(this);

    textEdit->setReadOnly(true);
    textEdit->setText(simplex->generateString());
    textEdit->setFont(QFont("Calibri", 13));

    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    const QVector<QPair<int, QString>> allVariables = simplex->getAllVars();
    const QVector<QVector<int>> matrixCoefficients = simplex->getMatrixCoefficients();

    layout->addWidget(textEdit);

    QTableWidget *tableCoefficients = new QTableWidget(this);
    TableWidget(tableCoefficients, matrixCoefficients.size(), matrixCoefficients[0].size());
    layout->addWidget(tableCoefficients);
    tableCoefficients->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < tableCoefficients->rowCount(); ++i)
        for (int j = 0; j < tableCoefficients->columnCount(); ++j)
            tableCoefficients->setItem(i, j, new QTableWidgetItem(QString::number(matrixCoefficients[i][j])));
    for (int i = 0; i < allVariables.size(); ++i)
        tableCoefficients->setHorizontalHeaderItem(i, new QTableWidgetItem(allVariables[i].second));

    QScrollArea *area = new QScrollArea();
    area->setFrameShape(QFrame::NoFrame);
    area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area->setWidget(tableCoefficients);
    area->setFixedSize(QGuiApplication::primaryScreen()->size().width() - textEdit->width() - textEdit->pos().x() - 20, tableCoefficients->height() + 20);
    layout->addWidget(area);
    textEdit->setFixedSize(35 * fn->columnCount() * 2, area->height()  );
    widget->adjustSize();
    widget->move(minimum_X, minimum_Y);
    widget->show();
    CreateSimTable(minimum_Y + widget->height());
}

void MainWindow::CreateSimTable(const int &startPosition_Y)
{
    int Y = startPosition_Y;
    const int mheight = QGuiApplication::primaryScreen()->size().height();
    int iRow = 0;
    int iColumn = 0;
    int tableCount  = 0;
    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(widget);
    QVBoxLayout *groupBoxLayout;
    QGroupBox *groupBox;

    do {
        groupBox = new QGroupBox("Simplex Table Number: " + QString::number(++tableCount));
        groupBoxLayout = new QVBoxLayout(groupBox);

        const QVector<QPair<int, QString>> basis = simplex->getBasis();
        const QVector<QPair<int, QString>> unbasis = simplex->getUnbasis();
        const QVector<QVector<Fraction>> matrix = simplex->getMatrix();

        QTableWidget *simplexTable = new QTableWidget();

        TableWidget(simplexTable, matrix.size(), matrix[0].size());
        simplexTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        for (int i = 0; i < simplexTable->rowCount(); ++i)
        {
            for (int j = 0; j < simplexTable->columnCount(); ++j)
            {
                simplexTable->setItem(i, j, new QTableWidgetItem(matrix[i][j].getFraction()));
                //See values entered into table
                qDebug()<<matrix[i][j].getFraction()<<endl;
            }
        }
        for (int i = 0; i < basis.size(); ++i)

            simplexTable->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(basis[i].first) + " " + basis[i].second));
            simplexTable->setVerticalHeaderItem(simplexTable->rowCount() - 1, new QTableWidgetItem(""));

        for (int i = 0; i < unbasis.size(); ++i)

            simplexTable->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::number(unbasis[i].first) + "\n" + unbasis[i].second));

        if (simplex->getCurrentGuidingRow() != -1 && simplex->getCurrentGuidingColumn() != -1)
            simplexTable->item(simplex->getCurrentGuidingRow(), simplex->getCurrentGuidingColumn())->setBackground(QColor(0x46FF00));

        groupBoxLayout->addWidget(simplexTable);
        groupBox->adjustSize();

        layout->addWidget(groupBox, iRow, iColumn);
        ++iRow;

        Y += groupBox->height();

        if (Y + groupBox->height() + 25 > mheight) {
            Y = startPosition_Y;
            iRow = 0;
            ++iColumn;
        }
    } while (Simplex::SolveResult::UNSOLVED == simplex->createNextSimplexMatrix());

    QLabel *lblAnswer = new QLabel(simplex->getAnswer());
    lblAnswer->setStyleSheet("color: green;");
    QFont font;
    font.setPointSize(10);
    lblAnswer->setFont(font);
    groupBoxLayout->addWidget(lblAnswer);

    layout->addWidget(groupBox, --iRow, iColumn);

    widget->setLayout(layout);
    QScrollArea *area = new QScrollArea(this);
    area->setWidget(widget);
    area->setGeometry(minimum_X, startPosition_Y, QGuiApplication::primaryScreen()->size().width() - minimum_X - 7,
                      mheight - startPosition_Y - 80);

    area->show();
}




