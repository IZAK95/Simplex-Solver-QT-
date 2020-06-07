#ifndef SOLVER_H
#define SOLVER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

class Solver : public QGraphicsView
{
    Q_OBJECT

public:
    Solver(QWidget *parent = nullptr);

    void displayMainMenu();

    QGraphicsScene * scene;
    int sceneWidth;
    int sceneHeight;
    ~Solver();

public slots:
     void start();
};
#endif // SOLVER_H
