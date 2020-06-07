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
{
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
    connect(calculateButton, SIGNAL(clicked()), this, SLOT(start()));
    scene->addItem(calculateButton);
}

Solver::~Solver()
{
}

void Solver::start()
{
    scene->clear();
    qDebug()<<"Program Started"<<endl;
}

