#include "primewin.h"
#include "ui_primewin.h"

PrimeWin::PrimeWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PrimeWin)
{
    ui->setupUi(this);
}

PrimeWin::~PrimeWin()
{
    delete ui;
}
