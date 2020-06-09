#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QTextEdit>
#include <QScreen>
#include <QMessageBox>
#include <QGridLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QPalette>

const int button_minX = 35;
const int button_minY = 35;

const int button_maxX = 70;
const int button_maxY = 70;

const int minimum_Y = 0;
const int minimum_X = 0;

//These values determine the size of the Table Rows and Columns
const int tableColumnWidth = 80;
const int tableRowHeight = 40;

const int QTextEditRowHeight = 26;
const int buttonWidth = 150;
const int buttonHeight = 30;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Setup();
    void GetVal(const int &rows, const int &columns);
    void TableWidget(QTableWidget *table, const int &rows, const int &columns);
    bool Verify(const QTableWidget *limitations, const QTableWidget *function) const;

private:
    Ui::MainWindow *ui;
    QTableWidget *limitations, *function;
};
#endif // MAINWINDOW_H
