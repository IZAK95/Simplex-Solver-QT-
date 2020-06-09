#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("REII313 Practical");
    Setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Setup()
{
     //Define Fonts Used
     QFont DefaultFont("Times",15);
     QFont TitleFont("Times", 30, QFont::Bold);

     //Set BackGround Colours

     QPalette labelQPalette;
     labelQPalette.setColor(QPalette::Window,Qt::white);

     //Set Text Colours
     labelQPalette.setColor(QPalette::WindowText,Qt::blue);

    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(widget);

    //Insert Title
    QLabel *label = new QLabel("REII313 Prakties",this);
    label->setMinimumWidth(300);
    label->setMinimumHeight(100);
    label->setFont(TitleFont);
    layout->addWidget(label,0,0);


    QLabel *labelRowsCount = new QLabel(" Number of limits (rows):", this);
    layout->addWidget(labelRowsCount, 1, 0);

    QSpinBox *spinRowsCount = new QSpinBox(this); // Number of lines (number of restrictions)
    spinRowsCount->setRange(2, 20);

    layout->addWidget(spinRowsCount, 1, 1);

    QLabel *labelColumnsCount = new QLabel("Number of variables (columns):", this);
    layout->addWidget(labelColumnsCount, 2, 0);

    QSpinBox *spinColumnsCount = new QSpinBox(this);
    // Number of columns (number of variables)
    spinColumnsCount->setRange(2, 20);
    layout->addWidget(spinColumnsCount, 2, 1);

    QPushButton *buttonContinue = new QPushButton("Next", this);
   // buttonContinue->setPalette(labelQPalette);
    layout->addWidget(buttonContinue);
    widget->adjustSize();
    widget->move(minimum_X, minimum_Y);
    widget->show();

    connect(buttonContinue, &QPushButton::clicked, this, [=]() {
        widget->hide();

        MainWindow::GetVal(spinRowsCount->value(), spinColumnsCount->value());
        //Grabs values from the UI and sends it to the TableWidget function in order to determine table size.
        int r = spinRowsCount->value();
        int c = spinColumnsCount->value();
        qDebug()<<r<<endl;
        qDebug()<<c<<endl;

    });
}

void MainWindow::GetVal(const int &rows, const int &columns)
{
    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(widget);
    layout->setSpacing(0);
    limitations = new QTableWidget(this);
    // Constraint Matrix
    TableWidget(limitations, rows, columns + 2);


    for (int i = 0; i < rows; ++i) {
        QComboBox *combo = new QComboBox(this);
        combo->addItem(" ≥ ");
        combo->addItem(" ≤ ");
        combo->addItem(" = ");
        limitations->setCellWidget(i, columns, combo);

        for (int i = 0; i < columns; ++i)
            limitations->setHorizontalHeaderItem(i, new QTableWidgetItem("x" + QString::number(i + 1)));

        limitations->setHorizontalHeaderItem(columns, new QTableWidgetItem("Sign"));
        limitations->setHorizontalHeaderItem(columns + 1, new QTableWidgetItem("Equal"));
        layout->addWidget(limitations, 0, 0);
        function = new QTableWidget(this);


        // objective function
        TableWidget(function, 1, columns + 1);
        QComboBox *extr = new QComboBox(this);
        extr->addItem("max");
        extr->addItem("min");
        function->setCellWidget(0, columns, extr);
        for (int i = 0; i < columns; ++i)
            function->setHorizontalHeaderItem(i, new QTableWidgetItem("x" + QString::number(i + 1)));

        function->setHorizontalHeaderItem(columns, new QTableWidgetItem("Min/Max"));
        layout->addWidget(function, 1, 0);

        QPushButton *buttonContinue = new QPushButton("Solve", this);
        buttonContinue->setMinimumSize(100,50);
        layout->addWidget(buttonContinue, 3, 0);

        QPushButton *buttonBack = new QPushButton("Back", this);
        //buttonBack->setMinimumSize(100,50);
        layout->addWidget(buttonBack, 2, 0);


        widget->adjustSize();
        widget->move(0, 0);
        widget->show();

        connect(buttonBack, &QPushButton::clicked, this, [=]() {
            widget->hide();
            Setup();
        });



        connect(buttonContinue, &QPushButton::clicked, this, [=]() {              
            if (!MainWindow::Verify(limitations, function)) {

                QMessageBox *msg = new QMessageBox();
                msg->warning(this,"Error"," Not all fields are filled.");

                return ;
            }
            widget->hide();
            // simplex = new Simplex();
            // simplex->initialize(limitations, function);
            // MainWindow::ShowCanonicalLimitationsAndMatrixCoefficients();
        });
    }
}

//This function draws the table based on the amount of variables and iunputs.
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
}

bool MainWindow::Verify(const QTableWidget *limitations, const QTableWidget *function) const
{
    QRegExp check("-?\\d{1,6}");
    for (int i = 0; i < function->columnCount() - 1; ++i) {
        if (function->item(0, i) != nullptr) {
            if (!check.exactMatch(function->item(0, i)->text()))
                return false;
        }
        else return false;
    }

    for (int i = 0; i < limitations->rowCount(); ++i) {
        for (int j = 0; j < limitations->columnCount() - 2; ++j) {
            if (limitations->item(i, j) != nullptr) {
                if (!check.exactMatch(limitations->item(i, j)->text()))
                    return false;
            }
            else return false;
        }
        if (limitations->item(i, limitations->columnCount() - 1) != nullptr) {
            if (!check.exactMatch(limitations->item(i, limitations->columnCount() - 1)->text()))
                return false;
        }
        else return false;
    }
    return true;
}

