#include "primewin.h"
#include "ui_primewin.h"

/*----------------------------------------------------------------------*/
//Default constructor
PrimeWin::PrimeWin(QWidget *parent):QMainWindow(parent),
ui(new Ui::PrimeWin){ui->setupUi(this);}
/*----------------------------------------------------------------------*/
//Default destructor
PrimeWin::~PrimeWin(){delete ui;}
/*----------------------------------------------------------------------*/
//Primary constructor
PrimeWin::PrimeWin(QWidget *parent, int dummyVarForNow) :
    QMainWindow(parent),
    ui(new Ui::PrimeWin)
{
    ui->setupUi(this);

    //GUI display initializations
    ui->stackWidg->setCurrentIndex(0);
    ui->adminLoginBt->hide();

    //Keystroke to pull up admin login window
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return),
                  this, SLOT(on_adminLoginBt_clicked()));
}

/*PUBLIC SLOTS==========================================================*/
void PrimeWin::catchLoginStatus(bool status)
//Catches signal to see if login is good or not
{
    //Change page to admin home page if status is true
    if (status)
    {ui->stackWidg->setCurrentIndex(4);}
}

/*PAGE INDEX============================================================*/
//Index 0 = start page
//Index 1 = home page
//Index 2 = itinerary page
//Index 3 = summary page
//Index 4 = admin home page
/*======================================================================*/

//Index0==================================================================
void PrimeWin::on_startInfoBt_clicked()
//Index 0 to 1
{ui->stackWidg->setCurrentIndex(1);}

void PrimeWin::on_startTripBt_clicked()
//Index 0 to 2
{ui->stackWidg->setCurrentIndex(2);}

void PrimeWin::on_adminLoginBt_clicked()
//Pulls up admin login window
{
    //Only allow logging in from the start page
    if (ui->stackWidg->currentIndex() == 0)
    {
        //Construct new dialog
        AdminLogin newLoginWin(this);

        //Line PrimeWin up to catch newLoginWin's signal
        connect(&newLoginWin,SIGNAL(throwLoginStatus(bool)),
                this,SLOT(catchLoginStatus(bool)));

        //Display newFoodWin's dialog
        newLoginWin.exec();
    }
}

//Index1==================================================================
void PrimeWin::on_homeBackBt_clicked()
//Index 1 to 0
{ui->stackWidg->setCurrentIndex(0);}

void PrimeWin::on_homePlanTripBt_clicked()
//Index 1 to 2
{ui->stackWidg->setCurrentIndex(2);}


