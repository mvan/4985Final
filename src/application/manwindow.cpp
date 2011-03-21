#include "manwindow.h"
#include "ui_manwindow.h"

ManWindow::ManWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManWindow)
{
    ui->setupUi(this);
}

ManWindow::~ManWindow()
{
    delete ui;
}
