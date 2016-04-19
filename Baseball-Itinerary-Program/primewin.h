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
#include <QSignalBlocker>

#include "datastructures.h"
#include "adminlogin.h"
#include "customimplementations.h"
#include "editdistances.h"
#include "addsouvdialog.h"

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

    //Refreshes the home table based on the vector passed in (Index 1)
    void refreshHomeTbl(vector<int> stadNumOrder);

    //Refreshes detail labels on the home page (Index 1)
    void refreshHomeDetails();

    //Refreshes the view of the itinerary builder (Index 2)
    void refreshItinBuilder();

    //Refreshes the itineray view (Index 2)
    void refreshItin();

    //Refreshes admin stadium table
    void refreshAdminTbl();

    //Validates phone numbers and returns a formatted number
    QString phoneCheck(QString phone);

public slots:
    void catchLoginStatus(bool status); //Catches login signal

    void catchDataUpdate(Data caughtThis);

    void catchAddItin();

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

    void on_homeSynthCB_toggled(bool checked);

    void on_homeTurfCB_toggled(bool checked);

    void on_homeNameRd_toggled(bool checked);

//Index2==================================================================
    void on_itinStartOverBt_clicked();

    void on_itinOptimizeBt_clicked();

//Index3==================================================================

//Index4==================================================================
    void on_adminRestartBt_clicked();

    void on_adminBaseBt_clicked();

    void on_adminDistBt_clicked();

    void on_adminStadTbl_cellChanged(int row, int column);

    void refreshSouvenirTableAdmin();

    void on_adminStadTbl_itemSelectionChanged();

    // on Add New Souvenir Button
    void on_pushButton_9_clicked();

    void on_adminBackBtn_clicked();

    // Catch data from Add New Souv Button
    void catchNewSouvenirData(Data caughtData);

    void on_deleteSouvBtn_clicked();

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
