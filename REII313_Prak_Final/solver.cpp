#include "solver.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QMessageBox>
#include <QComboBox>
#include "button.h"
#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <simplex.h>


Solver::Solver(QWidget *parent)    
{
    sceneWidth = 1920;
    sceneHeight = 1080;

    // set up screen
    setFixedSize(sceneWidth, sceneHeight);

    // set up scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, sceneWidth, sceneHeight);
    setScene(scene);




}

void Solver::displayMainMenu()
{   scene->clear();
    // create title text
    QGraphicsTextItem *titleText = new QGraphicsTextItem(QString("Linear Solver"));
    QFont titleFont("comic sans", 50);
    titleText->setFont(titleFont);
    int xTitlePos = this->width()/2 - titleText->boundingRect().width()/2;
    int yTitlePos = 150;
    titleText->setPos(xTitlePos, yTitlePos);
    scene->addItem(titleText);

    // create Calulate button
    Button *calculateButton = new Button(QString("Solve"));
    int xPlayPos = this->width()/2 - calculateButton->boundingRect().width()/2 +800;
    int yPlayPos = 800;
    calculateButton->setPos(xPlayPos, yPlayPos);
    connect(calculateButton, SIGNAL(clicked()), this, SLOT(displayResults()));
    scene->addItem(calculateButton);
}

void Solver::displayResults()
{
    scene->clear();

    Solver solution;
    solution.Calculate();

    // create title text
    QGraphicsTextItem *titleText = new QGraphicsTextItem(QString("Results"));
    QFont titleFont("comic sans", 50);
    titleText->setFont(titleFont);
    int xTitlePos = this->width()/2 - titleText->boundingRect().width()/2;
    int yTitlePos = 150;
    titleText->setPos(xTitlePos, yTitlePos);
    scene->addItem(titleText);

    // create Calulate button
    Button *BackButton = new Button(QString("Back"));
    int xPlayPos = this->width()/2 - BackButton->boundingRect().width()/2 ;
    int yPlayPos = 800;
    BackButton->setPos(xPlayPos, yPlayPos);
    connect(BackButton, SIGNAL(clicked()), this, SLOT(displayMainMenu()));
    scene->addItem(BackButton);
}

int Solver::Calculate()
{
    int colSizeA=6;  //should initialise columns size in A
    int rowSizeA = 3;  //should initialise columns row in A[][] vector

    float C[]= {-6,-5,-4,0,0,0};  //should initialis the c arry here
    float B[]={180,300,240};  // should initialis the b array here
    float a[3][6] = {    //should intialise the A[][] array here
                         { 2,  1,  1, 1, 0, 0},
                         { 1,  3,  2, 0, 1, 0},
                         { 2,  1,  2, 0, 0, 1}
                    };


    std::vector <std::vector<float> > vec2D(rowSizeA, std::vector<float>(colSizeA, 0));

    std::vector<float> b(rowSizeA,0);
    std::vector<float> c(colSizeA,0);




    for(int i=0;i<rowSizeA;i++){         //make a vector from given array
        for(int j=0; j<colSizeA;j++){
            vec2D[i][j] = a[i][j];
        }
    }


    for(int i=0;i<rowSizeA;i++){
        b[i] = B[i];
    }

    for(int i=0;i<colSizeA;i++){
        c[i] = C[i];
    }

    Simplex Simplex(vec2D,b,c);
    Simplex.CalculateSimplex();
}

Solver::~Solver()
{
}

void Solver::start()
{
    scene->clear();
    qDebug()<<"Program Started"<<endl;





}

