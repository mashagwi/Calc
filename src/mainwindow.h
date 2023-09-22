#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpressionValidator>

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

//    void on_zero_clicked();

    void on_zero_released();

    void AllPButtons_clicked();

    void on_one_released();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
