#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    setFixedSize(600, 450);
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

void MainWindow::on_Graph_toggled(bool checked)
{
    if (checked) {
            setFixedSize(980, 450);
        } else {
            setFixedSize(600, 450);
        }
}

void MainWindow::on_Calc_tabBarClicked(int index)
{
    if (index == 0) {
           if (ui->Graph->isChecked()) {
               setFixedSize(980, 450);
           } else {
               setFixedSize(600, 450);
           }
       } else if (index == 1) {
           setFixedSize(800, 480);
       } else if (index == 2) {
           setFixedSize(1000, 450);
       }
}

