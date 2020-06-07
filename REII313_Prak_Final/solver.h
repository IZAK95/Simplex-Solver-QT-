#ifndef SOLVER_H
#define SOLVER_H
#include "simplex.h"
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

class Solver : public QGraphicsView
{
    Q_OBJECT

public:
    Solver(QWidget *parent = nullptr);




    QGraphicsScene * scene;
    QGraphicsScene *result;
    int sceneWidth;
    int sceneHeight;
    ~Solver();

public slots:
     void start();
     int Calculate();
     void displayResults();
     void displayMainMenu();

};
#endif // SOLVER_H
