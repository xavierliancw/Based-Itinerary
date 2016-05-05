/**
  @file
  @author jerryberry
  @date 28 April 2016
  @brief This file contains the PrimeWin class declaration.
  */
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
//<<<<<<< Updated upstream
#include <QDragEnterEvent>
//=======
#include <QDate>
//>>>>>>> Stashed changes

#include "datastructures.h"
#include "customsorts.h"
#include "adminlogin.h"
#include "editdistances.h"
#include "addsouvdialog.h"
#include "addstadiumwin.h"
using namespace std;

namespace Ui {
class PrimeWin;
}
/**
 * @brief This class handles the main window of the program.
 *
 * This class creates the main window which provides an interface between
 * all aspects of this program and any user.
 * @par REQUIREMENTS:
 * #include <QMainWindow>       <br>
 * #include <QDebug>            <br>
 * #include <QShortcut>         <br>
 * #include <QSqlDatabase>      <br>
 * #include <QSqlQuery>         <br>
 * #include <QDir>              <br>
 * #include <QProcess>          <br>
 * #include <QMessageBox>       <br>
 * #include <QFileSystemModel>  <br>
 * #include <QSignalBlocker>    <br>
 * #include "datastructures.h"  <br>
 * #include "customsorts.h"     <br>
 * #include "adminlogin.h"      <br>
 * #include "editdistances.h"   <br>
 * #include "addsouvdialog.h"   <br>
 * using namespace std;
 */
class PrimeWin : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Default constructor
     * @param parent : QWidget parent pointer
     */
    explicit PrimeWin(QWidget *parent = 0);
    ~PrimeWin();
    /**
     * @brief Construct the graphical user interface
     * @param parent : QWidget parent pointer
     * @param dummyVarForNow : UNUSED FOR NOW
     */
    PrimeWin(QWidget *parent, int dummyVarForNow);

    //Refreshes the home table based on the vector passed in (Index 1)
    /**
     * @brief Redraw the table on the home page to display stadiums and
     * teams
     * @param stadNumOrder : Vector of stadNums that dictate the order of
     * the table's listing
     * @see Data for more information about stadNums
     */
    void refreshHomeTbl(vector<int> stadNumOrder);

    //Refreshes detail labels on the home page (Index 1)
    /**
     * @brief Redraw all stadium information in the left pane of the home
     * page when a different stadium is selected
     */
    void refreshHomeDetails();

    //Refreshes the view of the itinerary builder (Index 2)
    /**
     * @brief Redraw the stadium listing on the itinerary builder page
     */
    void refreshItinBuilder();

    //Refreshes the itineray view (Index 2)
    /**
     * @brief Redraw the itinerary itself to show any changes made
     */
    void refreshItin();

    //Refreshes admin stadium table
    /**
     * @brief Redraw the administrator's stadium table that contains all
     * information that pertains to stadiums
     */
    void refreshAdminTbl();

    //Calculates the itinerary's trip distance
    /**
     * @brief Calculate the trip distance of the current itinerary
     */
    void calcTrip();

public slots:
    //Catches login signal
    /**
     * @brief Determine if PrimeWin should switch to the administrator's
     * page
     * @param status : If true, switch the page / If not, don't
     */
    void catchLoginStatus(bool status);

    /**
     * @brief Update PrimeWin's Data variable
     * @param caughtThis : Updated Data variable
     * @see Data for more information about the Data variable
     */
    void catchDataUpdate(Data caughtThis);

    /**
     * @brief Update itinerary with newly queued or dequeued stadium
     */
    void catchAddItin();

    //Event filter to detect drag and drops
    /**
     * @brief Look for Qt events
     * @param object : GUI element that's generating a Qt event
     * @param event : Qt event that's happening
     * @return Returns false always
     *
     * Drag and drop functionality within the itinerary is implemented
     * in this method.
     */
    bool eventFilter(QObject *object, QEvent *event);

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

    void refreshHomeSouvTbl();

    void filterStads();

    void on_homeAmericanCB_clicked();

    void on_homeNationalCB_clicked();

    void on_homeTurfCB_clicked();

    void on_homeSynthCB_clicked();

    void on_homeNameRd_toggled(bool checked);

    void on_homeCapRd_toggled(bool checked);

    void on_homeTypeRd_toggled(bool checked);

    void on_homeDateRd_toggled(bool checked);

//Index2==================================================================
    void on_itinStartOverBt_clicked();

    void on_itinOptimizeBt_clicked();

//Index3==================================================================

//Index4==================================================================
    void on_adminRestartBt_clicked();

    void on_adminBaseBt_clicked();

    void on_adminDistBt_clicked();

    void on_adminStadTbl_cellChanged(int row, int column);

    //Validates phone numbers and returns a formatted number
    QString phoneCheck(QString phone);

    void refreshSouvenirTableAdmin();

    void on_adminStadTbl_itemSelectionChanged();

    // on Add New Souvenir Button
    void on_pushButton_9_clicked();

    // Catch data from Add New Souv Button
    void catchNewSouvenirData(Data caughtData);

    // Catch data from Add New Team Button
    void catchNewTeamData(Data caughtData);

    // Deletes a souvenir from a specific stadium
    void on_deleteSouvBtn_clicked();

    void on_adminPrimBt_clicked();

//Index5==================================================================
    void on_dataBackBt_clicked();

    void on_dataTxtBt_clicked();

    void on_adminSouvTable_cellChanged(int row, int column);

    void on_addNewTeamBtn_clicked();

private:
    Ui::PrimeWin *ui;           //User interface
    list<ItinObj> itinList;     //List of queued stadiums in itinerary
    Data data;                  //Interface for all data structures
    QFileSystemModel *dirmodel; //Model of file directory
    ItinObj dragDrop;           //Data that's being dragged in the itin
    int pickup;                 //Row of dragged data
};
#endif // PRIMEWIN_H
