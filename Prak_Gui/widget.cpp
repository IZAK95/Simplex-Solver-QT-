#include "widget.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QMessageBox>
#include <QComboBox>
#include "matrix.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //First name
    QLabel *numVars = new QLabel("Number of \nVariables", this);
    numVars->setMinimumSize(70,50);
    numVars->move(10,10);

    QLineEdit *numVarsEdit = new QLineEdit(this);
    numVarsEdit->setMinimumSize(200,50);
    numVarsEdit->move(110,10);
    numVarsEdit->setPlaceholderText("n");



    //Last name
    QLabel *ineq = new QLabel("Number of \nInequalities", this);
    ineq->setMinimumSize(70,50);
    ineq->move(10,70);

    QLineEdit *ineqEdit = new QLineEdit(this);
    ineqEdit->setMinimumSize(200,50);
    ineqEdit->move(110,70);
    ineqEdit->setPlaceholderText("m");

    //Last name
//    QLabel *cityname = new QLabel("City name", this);
//    cityname->setMinimumSize(10,50);
//    cityname->move(10,130);

//    QLineEdit *citynameedit = new QLineEdit(this);
//    citynameedit->setMinimumSize(200,50);
//    citynameedit->move(100,130);
//    citynameedit->setPlaceholderText("Bloemfontein");


    QLabel *selectionlabel = new QLabel("Select Type", this);
    selectionlabel->setMinimumSize(70,50);
    selectionlabel->move(10,250);


    QComboBox *selection = new QComboBox(this);
    selection->addItem("Maximize");
    selection->addItem("Minimize");
    selection->setMinimumSize(70,50);
    selection->move(100,250);


    QFont buttonFont("Times",20,QFont::Bold);
    QPushButton *button = new QPushButton("Grab data",this);
    button->setFont(buttonFont);
    button->move(80,190);


    connect(button,&QPushButton::clicked,[=](){
       QString numVars = numVarsEdit->text();
       const int a  =   numVarsEdit->text().toInt();

      QString inequalities = ineqEdit->text();
      int b = ineqEdit->text().toInt();
      qDebug()<<"Variables are:"<<a<<b<<endl;
//      QString city = citynameedit->text();


      if (!numVars.isEmpty()&& !inequalities.isEmpty())
      {
         qDebug()<<"Data is"<<numVars<<inequalities<<endl;
      }

      else
      {
        QMessageBox::information(this,"Error","Wrong Input Data");
      }


    });

//    connect(firstnameedit, &QLineEdit::cursorPositionChanged,[=](){
//     qDebug()<<"Position is "<<firstnameedit->cursorPosition();
//    });

    connect(numVarsEdit,&QLineEdit::editingFinished,[=](){
        qDebug()<<"Editing Finished"<<endl;
    });

    connect(numVarsEdit,&QLineEdit::selectionChanged,[=](){
        qDebug()<<"Selection Changed"<<endl;
    });
    connect(numVarsEdit,&QLineEdit::textChanged,[=](){
        qDebug()<<"Selecetion changed to "<<numVarsEdit->text();
    });


}




Widget::~Widget()
{
}

void Widget::Maximization()
{

}

QSize Widget::sizeHint() const
{
     return QSize(1000,1000);
}

