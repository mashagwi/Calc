#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QString>
#include <QtWidgets/QMainWindow>
#include <sstream>

extern "C" {
    #include "s21_smartcalc.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clear_released();
    void AllButtons_clicked();

    void on_Graph_toggled(bool checked);

    void on_Calc_tabBarClicked(int index);

    void on_equal_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
