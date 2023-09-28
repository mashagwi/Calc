#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList <QPushButton *> AllPButtons = MainWindow::findChildren<QPushButton *>();
    AllPButtons.removeOne(ui->equal);
    AllPButtons.removeOne(ui->clear);
    AllPButtons.removeOne(ui->graf);
    AllPButtons.removeOne(ui->credit);
    AllPButtons.removeOne(ui->depos);

    for(int i = 0; i<AllPButtons.length(); i++) {
        connect(AllPButtons[i], SIGNAL(clicked()), this, SLOT(AllPButtons_clicked()));
    }

    QRegularExpressionValidator *validator = new QRegularExpressionValidator(
               QRegularExpression("[-,+]?[0-9]*[.]?[0-9]*"), this);

    ui->val->setValidator(validator);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_clear_released()
{
    ui->Input->clear();
    ui->result->clear();
    ui->widgetplot->clearGraphs();
}

void MainWindow::AllPButtons_clicked(){

    QPushButton *button = (QPushButton *)sender();
    if(ui->ditgits->buttons().contains(button)) {
    ui->Input->setText(ui->Input->text() + button->text());}

    if(ui->func->buttons().contains(button)) {
    ui->Input->setText(ui->Input->text() + button->text() + '(');}

    if(ui->operands->buttons().contains(button)) {
        ui->Input->setText(ui->Input->text() + button->text());}

    else if(ui->brackets->buttons().contains(button)) {
        if(button ->text() == "(")
            countLeftBrackets++;
        else
            countRightBrackets++;
        ui->Input->setText(ui->Input->text() + button->text());

    } else if (ui->dot == button) {
        ui->Input->setText(ui->Input->text() + ".");
    }

}

void MainWindow::on_equal_clicked(){

//    QString str = ui->Input->text();  // забираем текст из строки ввода
//    QByteArray ba = str.toLocal8Bit();  // переводим строку в массив байтов
//    const char *chars = ba.data();  // переводим массив байтов в строку си
//    operas *operas = NULL;

    ui->widgetplot->clearGraphs();
    int err = OK;
    S *stack = NULL;
    double res = 0;
    int variables_count = 0;
    QString input = ui->Input->toPlainText().toLower();
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
      ui->result->append("Invalid expression");
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
    ui->result->append("Invalid expression");
    }
    remove_stack(&stack);



}

