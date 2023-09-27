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

//void MainWindow::on_equal_clicked()
//{
//    QString str = ui->Input->text();  // забираем текст из строки ввода
//    QByteArray ba = str.toLocal8Bit();  // переводим строку в массив байтов
//    const char *chars = ba.data();  // переводим массив байтов в строку си
//    operas *operas = NULL;



//}

