#ifndef INPUT_H
#define INPUT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QLineEdit>

class Input: public QWidget
{
    Q_OBJECT
public:
    explicit Input(QWidget *parent = nullptr);
    Input(QLabel name,QLineEdit edit);

signals:

};

#endif // INPUT_H
