#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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
