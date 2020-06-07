#include "solver.h"
#include <QApplication>

Solver *solver;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Solver w;
//    w.show();

    solver = new Solver();
    solver->show();
    solver->displayMainMenu();
    return a.exec();



}
