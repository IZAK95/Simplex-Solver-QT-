#include "input.h"

Input::Input(QWidget *parent) : QWidget(parent)
{

}

Input::Input(QLabel name, QLineEdit edit)
{
    setMinimumSize(70,50);
}
