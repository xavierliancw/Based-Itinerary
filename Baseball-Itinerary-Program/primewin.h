#ifndef PRIMEWIN_H
#define PRIMEWIN_H

#include <QMainWindow>

namespace Ui {
class PrimeWin;
}

class PrimeWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrimeWin(QWidget *parent = 0);
    ~PrimeWin();

private:
    Ui::PrimeWin *ui;
};

#endif // PRIMEWIN_H
