#include "primewin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PrimeWin w(NULL,1);
    w.resize(800,500);
    w.show();

    return a.exec();
}
