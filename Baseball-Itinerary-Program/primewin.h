#ifndef PRIMEWIN_H
#define PRIMEWIN_H

#include <QMainWindow>
#include <QDebug>
#include <QShortcut>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <QFileSystemModel>

#include "datastructures.h"
#include "adminlogin.h"
#include "customimplementations.h"

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

    //Refreshes the view of everything on the home page (Index 1)
    void refreshHome();

    //Refreshes detail labels on the home page (Index 1)
    void refreshHomeDetails();

public slots:
    void catchLoginStatus(bool status); //Catches login signal

private slots:
/*PAGE INDEX============================================================*/
//Index 0 = start page
//Index 1 = home page
//Index 2 = itinerary page
//Index 3 = summary page
//Index 4 = admin home page
//Index 5 = database management page
/*======================================================================*/
//Index0==================================================================
    void on_startInfoBt_clicked();

    void on_startTripBt_clicked();

    void on_adminLoginBt_clicked();

    void on_homeStadTbl_itemSelectionChanged();

//Index1==================================================================
    void on_homeBackBt_clicked();

    void on_homePlanTripBt_clicked();

    void on_homeNationalCB_toggled(bool checked);

    void on_homeAmericanCB_toggled(bool checked);
//Index2==================================================================
    void on_itinStartOverBt_clicked();

    void on_itinOptimizeBt_clicked();

//Index3==================================================================

//Index4==================================================================
    void on_adminRestartBt_clicked();

    void on_adminBaseBt_clicked();

//Index5==================================================================
    void on_dataBackBt_clicked();

    void on_dataTxtBt_clicked();


private:
    Ui::PrimeWin *ui;           //User interface

    std::list<ItinObj> itinList;

    Data data;                  //Interface for all data structures
    QFileSystemModel *dirmodel; //Model of file directory
};

#endif // PRIMEWIN_H
