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
//    AllPButtons.removeOne(ui->cos);
//    AllPButtons.removeOne(ui->sin);
//    AllPButtons.removeOne(ui->tg);
//    AllPButtons.removeOne(ui->acos);
//    AllPButtons.removeOne(ui->asin);
//    AllPButtons.removeOne(ui->atg);

    for(int i = 0; i<AllPButtons.length(); i++) {
        connect(AllPButtons[i], SIGNAL(clicked()), this, SLOT(AllPButtons_clicked()));
    }

    // регулярка для исключения символов
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

//void MainWindow::on_zero_clicked()
//{
////    QString text = ui->result->text();
////    text += "0";
////    ui->result->setText(text);
//}


//void MainWindow::on_zero_released()
//{
//    ui->Input->insert(ui->zero->text());
//}

//void MainWindow::on_one_released()
//{
//    ui->Input->insert(ui->one->text());
//}

