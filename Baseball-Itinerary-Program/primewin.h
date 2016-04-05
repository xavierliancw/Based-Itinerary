#ifndef PRIMEWIN_H
#define PRIMEWIN_H

#include <QMainWindow>
#include <QDebug>
#include <QShortcut>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDir>

#include "datastructures.h"
#include "adminlogin.h"

namespace Ui {
class PrimeWin;
}

class PrimeWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrimeWin(QWidget *parent = 0);
    ~PrimeWin();
    PrimeWin(QWidget *parent, int dummyVarForNow);

public slots:
    void catchLoginStatus(bool status); //Catches login signal

private slots:
/*PAGE INDEX============================================================*/
//Index 0 = start page
//Index 1 = home page
//Index 2 = itinerary page
//Index 3 = summary page
//Index 4 = admin home page
/*======================================================================*/
//Index0==================================================================
    void on_startInfoBt_clicked();

    void on_startTripBt_clicked();

    void on_adminLoginBt_clicked();

//Index1==================================================================
    void on_homeBackBt_clicked();

    void on_homePlanTripBt_clicked();

//Index2==================================================================

//Index3==================================================================

//Index4==================================================================


private:
    Ui::PrimeWin *ui;
    Data master;
};

#endif // PRIMEWIN_H
