#include "primewin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PrimeWin w(NULL,0);
    w.resize(900,500);
    w.show();
    return a.exec();
}
