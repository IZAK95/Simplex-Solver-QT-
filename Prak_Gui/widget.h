#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    void Maximization();
    void Minimization();
private:
    QSize sizeHint() const;
};
#endif // WIDGET_H
