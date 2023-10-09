#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtWidgets/QMainWindow>
#include <sstream>

#include "qcustomplot.h"

// extern "C" {
//     #include "s21_smartcalc.h"
// }

#ifdef __cplusplus
extern "C" {
#endif
#include "credit.h"
#include "deposit.h"
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
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void on_clear_released();

  void AllButtons_clicked();

  void on_x_clicked();

  void on_Graph_toggled(bool checked);

  void on_Calc_tabBarClicked(int index);

  void on_equal_clicked();

  void on_calcCredit_clicked();

  void on_remove_clicked();

  void updateTableWidgetsRowCount(int total_month);

  void on_dep_date_valueChanged(int arg1);

  void on_dep_date_type_currentIndexChanged(int index);

  void on_removeDep_clicked();

  void on_calcDep_clicked();

  void initializeData(double*& input_arr, double*& output_arr,
                      int& total_month);

  void calculateAndUpdateDeposit(double* input_arr, double* output_arr,
                                 int total_month);

 private:
  Ui::MainWindow* ui;
};
#endif  // MAINWINDOW_H
