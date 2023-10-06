#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    setFixedSize(530, 450);
    ui->setupUi(this);
    ui->annuti->setChecked(1);

    QList < QPushButton*> AllButtons = MainWindow::findChildren < QPushButton*>();
    AllButtons.removeOne(ui->clear);
    AllButtons.removeOne(ui->equal);

    for(int i = 0; i<AllButtons.length(); i++) {
            connect(AllButtons[i], SIGNAL(clicked()), this, SLOT(AllButtons_clicked()));
        }

    QRegularExpressionValidator *validator = new QRegularExpressionValidator(
                   QRegularExpression("[-,+]?[0-9]*[.]?[0-9]*"), this);

        ui->value->setValidator(validator);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clear_released()
{
    ui->input->clear();
    ui->result->clear();
    ui->value->clear();
    ui->widgetplot->clearPlottables();
    ui->widgetplot->replot();
}

void MainWindow::AllButtons_clicked()
{

    QPushButton *button = (QPushButton *)sender();
        if(ui->digit->buttons().contains(button)) {
        ui->input->insertPlainText(button->text());}

        if(ui->func->buttons().contains(button)) {
        ui->input->insertPlainText(button->text() + '(');}

        if(ui->operant->buttons().contains(button)) {
            ui->input->insertPlainText( button->text());}

        else if(ui->brackets->buttons().contains(button)) {
                       ui->input->insertPlainText(button->text());

        } else if (ui->dot == button) {
            ui->input->insertPlainText(".");
        }
}

void MainWindow::on_x_clicked()
{
    ui->input->insertPlainText(ui->x->text());
}

void MainWindow::on_Graph_toggled(bool checked)
{
    if (checked) {
            setFixedSize(990, 450);
        } else {
            setFixedSize(530, 450);
        }
}

void MainWindow::on_Calc_tabBarClicked(int index)
{
    if (index == 0) {
           if (ui->Graph->isChecked()) {
               setFixedSize(990, 450);
           } else {
               setFixedSize(530, 450);
           }
       } else if (index == 1) {
           setFixedSize(850, 480);
       } else if (index == 2) {
           setFixedSize(840, 500);
       }
}

void MainWindow::on_equal_clicked()
{
     ui->widgetplot->clearGraphs();
        int err = OK;
        S *stack = nullptr;
        double res = 0;
        int variables_count = 0;
        QString in = ui->input->toPlainText().toLower();
        std::string expression = in.toStdString();
        char* str = expression.data();
        err = parser(str, &stack, &variables_count);
        if (err != OK) {
        ui->result->clear();
        ui->result->append("Fail");
        } else {
            if (variables_count == 0) {
                err = calculate(&stack, 0, &res);
            } else if (variables_count == 1) {
                if (ui->value->text().isEmpty()) {
                    ui->result->append("enter a value");
                } else {
                   err = calculate(&stack, ui->value->text().toDouble(), &res);
                }
            }
                if (err == OK) {
                       ui->result->clear();
                       ui->result->append(QString::number(res));
                   } else {
                       ui->result->clear();
                       ui->result->append("Fail");
                   }

                QVector<double> x(100), y(100);
                for (int i = 0; i < 100; i++) {
                    x[i] = (i * ((M_PI * 8) / 100)) - (M_PI * 4);
                    res = 0;
                    err = s21_smartcalc(&res, str, x[i]);
                    y[i] = res;
                    if (err != OK) break;
                }
                if (err == OK) {
                    ui->widgetplot->addGraph();
                    ui->widgetplot ->graph(0)->addData(x, y);
                    double y_min = *std::min_element(y.begin(), y.end());
                    double y_max = *std::max_element(y.begin(), y.end());
                    ui->widgetplot->xAxis->setRange(-10, 10);
                    ui->widgetplot->yAxis->setRange(y_min, y_max);
                    ui->widgetplot->replot();
                    ui->widgetplot->show();
                }
        }

        remove_stack(&stack);
}

void MainWindow::on_calcCredit_clicked()
{
    ui->calcGraph->clearContents();
    double sum = ui->sumCredit->toPlainText().toDouble();
    int date = ui->dateCredit->value();
    int date_type = ui->dateType->currentIndex();
    double percent = ui->persentCredit->toPlainText().toDouble();
    int pay_type = ui->diffi->isChecked();
    double *m_pay = nullptr;
    double over_pay = 0;
    double total_pay = 0;
    int ret = calculate_credit(sum, &date, date_type, percent, pay_type, &m_pay, &over_pay, &total_pay);
    if (ret == OK){
        ui->overSum->setText(QString::number(over_pay, 'f', 2));
        ui->totalSum->setText(QString::number(total_pay, 'f', 2));
        for (int i = 0; i < date; i++) {
            ui->calcGraph->insertRow(i);
            ui->calcGraph->setItem(i, 0, new QTableWidgetItem(QString::number(m_pay[i])));
        }
        delete[] m_pay;
        m_pay = nullptr;
        } else {
            ui->totalSum->setText("Fail");
            ui->overSum->clear();
            }
}

void MainWindow::on_remove_clicked()
{
    ui->dateCredit->clear();
    ui->sumCredit->clear();
    ui->persentCredit->clear();
    ui->overSum->clear();
    ui->totalSum->clear();
    ui->calcGraph->setRowCount(0);
}




void MainWindow::updateTableWidgetsRowCount(int total_month) {
    ui->inputDep->setRowCount(total_month);
    ui->outputDep->setRowCount(total_month);
}

void MainWindow::on_dep_date_valueChanged(int arg1)
{
    int total_month = arg1;
       if (ui->dep_date_type->currentIndex() == 1) total_month *= 12;
       updateTableWidgetsRowCount(total_month);
       for (int i = 0; i < total_month; i++) {
           QTableWidgetItem* newItem = new QTableWidgetItem("0");
           ui->inputDep->setItem(i, 0, newItem);
           ui->outputDep->setItem(i, 0, newItem);
       }
//    ui->inputDep->setRowCount(0);
//    ui->outputDep->setRowCount(0);
//    int total_month = arg1;
//    if (ui->dep_date_type->currentIndex() == 1) total_month *= 12;
//    ui->inputDep->setRowCount(total_month);
//    ui->outputDep->setRowCount(total_month);
//    for (int i = 0; i < total_month; i++) {
//        QTableWidgetItem *newItem = new QTableWidgetItem();
//        newItem->text().append("0");
//        ui->inputDep->setItem(i, 0, newItem);
//        ui->outputDep->setItem(i, 0, newItem);
//      }
}

void MainWindow::on_dep_date_type_currentIndexChanged(int index)
{
    ui->inputDep->setRowCount(0);
     ui->outputDep->setRowCount(0);
     int total_month = ui->dep_date->value();
     if (index == 1) total_month *= 12;
     ui->inputDep->setRowCount(total_month);
     ui->outputDep->setRowCount(total_month);
}


void MainWindow::on_removeDep_clicked()
{
    ui->inputDep->setRowCount(0);
      ui->outputDep->setRowCount(0);
      ui->dep_date->clear();
      ui->dep_percent->clear();
      ui->dep_sum->clear();
      ui->dep_tax->clear();
}

void MainWindow::on_calcDep_clicked()
{
    ui->total_percent->clear();
      ui->total_sum->clear();
      ui->total_tax->clear();
      int response = OK;
      long double dep_result = 0;
      long double total_tax = 0;
      double *input_arr = NULL;
      double *output_arr = NULL;
      int total_month = ui->dep_date->value();
      if (total_month >= 1) {
        if (ui->dep_date_type->currentIndex() == 1) total_month *= 12;
        input_arr = (double *)calloc(total_month, sizeof(double));
        output_arr = (double *)calloc(total_month, sizeof(double));
        for (int i = 0; i < total_month; i++) {
          double in_val = 0;
          double out_val = 0;
          if (ui->inputDep->item(i, 0) != nullptr) {
            in_val = ui->inputDep->item(i, 0)->text().toDouble();
          }
          if (ui->outputDep->item(i, 0) != nullptr) {
            out_val = ui->outputDep->item(i, 0)->text().toDouble();
          }
          input_arr[i] = in_val;
          output_arr[i] = out_val;
        }
      } else {
        response = FAIL;
      }
      long double sum = ui->dep_sum->toPlainText().toDouble();
      long double percent = ui->dep_percent->toPlainText().toDouble();
      long double tax = ui->dep_tax->toPlainText().toDouble();
      int payment_period = 0;
      switch (ui->dep_type_period->currentIndex()) {
        case 0:
          payment_period = 1;
          break;
        case 1:
          payment_period = 3;
          break;
        case 2:
          payment_period = 6;
          break;
        case 3:
          payment_period = 12;
          break;
        default:
          response = FAIL;
          break;
      }

      if (response == OK) {
        response = calculate_deposit(&sum, total_month, percent, tax, payment_period,
                                ui->date_capital_type->currentIndex(), input_arr,
                                output_arr, &dep_result, &total_tax);
      }

      if (response == OK) {
        std::stringstream s_res, s_sum, s_tax;
        char buffer[1000] = "";

        std::sprintf(buffer, "%.2Lf", dep_result);
        s_res << buffer;
        ui->total_percent->setText(QString::fromStdString(s_res.str()));
        std::sprintf(buffer, "%.2Lf", sum);
        s_sum << buffer;
        ui->total_sum->setText(QString::fromStdString(s_sum.str()));
        std::sprintf(buffer, "%.2Lf", total_tax);
        s_tax << buffer;
        ui->total_tax->setText(QString::fromStdString(s_tax.str()));
      } else {
        ui->total_percent->setText("Invalid arguments");
      }
      if (NULL != input_arr) {
        free(input_arr);
        input_arr = NULL;
      }
      if (NULL != output_arr) {
        free(output_arr);
        output_arr = NULL;
      }
    }





//void MainWindow::on_calcDep_released()
//{

//}

