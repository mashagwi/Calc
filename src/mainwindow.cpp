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
           setFixedSize(1000, 400);
       }
}


void MainWindow::on_equal_clicked()
{
     ui->widgetplot->clearGraphs();
        int err = OK;
        S *stack = NULL;
        double res = 0;
        int variables_count = 0;
        QString in = ui->input->toPlainText().toLower();
        std::string expression = in.toStdString();
        char* str = (char *)expression.c_str();
        err = parser(str, &stack, &variables_count);
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
                if (ui->value->text().isEmpty()) {
                    ui->result->append("enter a value");
                } else {
                   err = calculate(&stack, ui->value->text().toDouble(), &res);
                   if (err == OK) {
                       ui->result->clear();
                       ui->result->append(QString::number(res));
                   } else {
                       ui->result->clear();
                       ui->result->append("Fail");
                   }
                }

                int arr_size = 1000;
                QVector<double> x(arr_size), y(arr_size);
                for (int i = 0; i < arr_size; i++) {
                    x[i] = (i * ((M_PI * 8) / arr_size)) - (M_PI * 4);
                    res = 0;
                    err = s21_smartcalc(&res, (char *)in.toStdString().c_str(), x[i]);
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

void MainWindow::on_calcCredit_clicked()
{
    ui->calcGraph->setRowCount(0);
    double sum = ui->sumCredit->toPlainText().toDouble();
    int date = ui->dateCredit->value();
    int date_type = ui->dateType->currentIndex();
    double percent = ui->persentCredit->toPlainText().toDouble();
    int pay_type = ui->diffi->isChecked();
    double *m_pay = NULL;
    double over_pay = 0;
    double total_pay = 0;
    int ret = calculate_credit(sum, &date, date_type, percent, pay_type, &m_pay, &over_pay, &total_pay);
    if (ret == OK){
        std::stringstream op,tp;
        char buff[1000] = "";
        std::sprintf(buff, "%.2lf", over_pay);
        op << buff;
        ui->overSum->setText(QString::fromStdString( op.str()));

        std::sprintf(buff, "%.2lf", total_pay);
        tp << buff;
        ui->totalSum->setText(QString::fromStdString(tp.str()));

        for(int i = 0; i < date; i++){
            ui->calcGraph->insertRow(i);
            ui->calcGraph->setItem(i, 0, new QTableWidgetItem(QString::number(m_pay[i])));
        }

        if(m_pay != NULL) {
            free(m_pay);
            m_pay = NULL;
        }
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

