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
    QList < QPushButton*> AllButtons = MainWindow::findChildren < QPushButton*>();
    AllButtons.removeOne(ui->clear);
    AllButtons.removeOne(ui->equal);

    for(int i = 0; i<AllButtons.length(); i++) {
            connect(AllButtons[i], SIGNAL(clicked()), this, SLOT(AllButtons_clicked()));
        }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clear_released()
{
    ui->input->clear();
    ui->result->clear();
    ui->widgetplot->clearGraphs();
}

void MainWindow::AllButtons_clicked()
{
    int countLeftBrackets = 0;
    int countRightBrackets = 0;

    QPushButton *button = (QPushButton *)sender();
        if(ui->digit->buttons().contains(button)) {
        ui->input->insertPlainText(button->text());}

        if(ui->func->buttons().contains(button)) {
        ui->input->insertPlainText(button->text() + '(');}

        if(ui->operant->buttons().contains(button)) {
            ui->input->insertPlainText( button->text());}

        else if(ui->brackets->buttons().contains(button)) {
            if(button ->text() == "(")
                countLeftBrackets++;
            else
                countRightBrackets++;
            ui->input->insertPlainText(button->text());

        } else if (ui->dot == button) {
            ui->input->insertPlainText(".");
        }
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
           setFixedSize(800, 480);
       } else if (index == 2) {
           setFixedSize(1000, 450);
       }
}


void MainWindow::on_equal_clicked()
{
    ui->widgetplot->clearGraphs();
        int err = OK;
        S *stack = NULL;
        double res = 0;
        int variables_count = 0;
        QString input = ui->input->toPlainText().toLower();
        std::string expression = input.toStdString().c_str();
        err = parser((char *)input.toStdString().c_str(), &stack, &variables_count);
           if (err == OK) {
            if (variables_count == 0) {
            err = calculate(&stack, 0, &res);
            if (err == OK) {
            ui->result->clear();
            ui->result->append(QString::number(res));
            } else {
            ui->result->clear();
          ui->result->append("Fail");
        }
      } else if (variables_count == 1) {
        ui->result->clear();
        int arr_size = 1000;
        QVector<double> x(arr_size), y(arr_size);
        for (int i = 0; i < arr_size; i++) {
          x[i] = (i * ((M_PI * 8) / arr_size)) - (M_PI * 4);
          res = 0;
          err =
              s21_smartcalc(&res, (char *)input.toStdString().c_str(), x[i]);
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
        }
        if (err != OK) {
        ui->result->clear();
        ui->result->append("Fail");
        }
        remove_stack(&stack);

}

