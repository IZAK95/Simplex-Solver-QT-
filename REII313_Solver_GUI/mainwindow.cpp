#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("REII313 Practical");

//    QWidget *backGround = new QWidget();
//    backGround->setGeometry(0,0,300,100);
//    QPalette *paletteBack = new QPalette();

//    paletteBack->setColor(QPalette::Background, Qt::black);
//    backGround->setAutoFillBackground(true);
//    backGround->setPalette(paletteBack);


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
     QFont TitleFont("Times", 35, QFont::Bold);

     //Set BackGround Colours

     QPalette labelQPalette;
     labelQPalette.setColor(QPalette::Window,Qt::white);

     //Set Text Colours
     labelQPalette.setColor(QPalette::WindowText,Qt::blue);

    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(widget);
    //Insert Title
    QLabel *labelTitle = new QLabel("REII313 Prakties",this);
    labelTitle->setMinimumWidth(100);
    labelTitle->setMinimumHeight(100);
    labelTitle->setFont(TitleFont);
    layout->addWidget(labelTitle,0,0);

    //Insert Description
    QLabel *labelDescription = new QLabel();
    labelDescription->setFont(DefaultFont);
    labelDescription->setText("Done by Izak Adendorff and Izelle Evert");
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
        buttonContinue->setMinimumSize(button_minX,button_minY);
        buttonContinue->setMaximumSize(button_maxX,button_maxY);
        layout->addWidget(buttonContinue, 3, 0);

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
            if (!MainWindow::Verify(limitations, function)) {

                QMessageBox *msg = new QMessageBox();
                msg->warning(this,"Error"," Not all fields are filled in.");

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

