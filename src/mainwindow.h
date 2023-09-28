#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpressionValidator>
#include <QString>
#include <QtWidgets/QMainWindow>
#include <sstream>

#ifdef __cplusplus
    extern "C"{
#endif
        #include "s21_smartcalc.h"
#ifdef __cplusplus
    }
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:

  void on_clear_released();

  void AllPButtons_clicked();

  void on_equal_clicked();

private:
  Ui::MainWindow *ui;

  int countLeftBrackets = 0;
  int countRightBrackets = 0;
};
#endif  // MAINWINDOW_H
