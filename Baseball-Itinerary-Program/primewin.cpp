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

    //Data initializations
    dirmodel = new QFileSystemModel(this);  //Model of file directory
    QString dir = QDir::currentPath();      //Path to executable
    dir.resize(dir.lastIndexOf("/build"));  //Truncate executable folder
    dirmodel->setRootPath(dir);             //Activate model

    //GUI display initializations
    ui->dataFileBrowser->setModel(dirmodel);    //Activate tree view
    ui->stackWidg->setCurrentIndex(0);
    ui->adminLoginBt->hide();
    ui->startInfoBt->setFocus();
    ui->dataFileBrowser->resizeColumnToContents(0);



    //Keystroke to pull up admin login window
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return),
                  this, SLOT(on_adminLoginBt_clicked()));
}

void PrimeWin::refreshHome()
//Refreshes the view of everything on the home page
//Complexity: O(t), t = number of teams
{
    int missingCompensator = 0; //Compensates for missing teams

    //REFRESH STADIUM TABLE
    ui->homeStadTbl->clear();
    ui->homeStadTbl->setRowCount(0);
    ui->homeStadTbl->setColumnCount(5);

    //Populate the stadium table
    for (unsigned int x=0;x<data.size();x++)
    {
        //Add a row
        ui->homeStadTbl->insertRow(ui->homeStadTbl->rowCount());

        //Determine if compensation is needed
        if (data.teamSize(x) > 0)
        {
            missingCompensator = 0;
        }
        else
        {
            missingCompensator = 1;
        }

        //Loop until all teams at the stadium are added
        for (unsigned int t=0;t<data.teamSize(x)+missingCompensator;t++)
        {
            //Add a new row after listing first team if stad has > 1 team
            if (t != 0)
            {
                ui->homeStadTbl->insertRow(ui->homeStadTbl->rowCount());
            }
            //Add stadNum to hidden first col
            ui->homeStadTbl
              ->setItem(ui->homeStadTbl
                        ->rowCount()-1,0,
                        new QTableWidgetItem(QString::number(x)));

            //If there are teams here
            if (data.teamSize(x) != 0)
            {
                //Add teamNum to hidden second col
                ui->homeStadTbl
                        ->setItem(ui->homeStadTbl->rowCount()-1,1,
                                  new
                                  QTableWidgetItem(QString::number(t)));
            }
            //If there's no team at this stadium
            else
            {
                //Add -1 to hidden second col
                ui->homeStadTbl
                        ->setItem(ui->homeStadTbl->rowCount()-1,1,
                                  new
                                  QTableWidgetItem(QString::number(-1)));
            }

            //Add field picture to third col
            int UNIMPLEMENTED;
            ui->homeStadTbl->setItem(ui->homeStadTbl->rowCount()-1,2,
                                     new QTableWidgetItem("NO_PIC"));

            //Add stadium name to fourth col
            ui->homeStadTbl
              ->setItem(ui->homeStadTbl
                        ->rowCount()-1,3,
                        new QTableWidgetItem(data.getStadName(x)));

            //If there are teams here, add the team name
            if (data.teamSize(x) != 0)
            {
                //Add team name to fifth col
                ui->homeStadTbl
                        ->setItem(ui->homeStadTbl->rowCount()-1,4,
                                  new
                                  QTableWidgetItem(
                                      data.getTeamName(x,t)));
            }
            else
            {
                ui->homeStadTbl
                        ->setItem(ui->homeStadTbl->rowCount()-1,4,
                                  new QTableWidgetItem(""));
            }
        }
    }
    //Prepare table for viewing
    ui->homeStadTbl->hideColumn(0);
    ui->homeStadTbl->hideColumn(1);
    ui->homeStadTbl->resizeColumnsToContents();
    ui->homeStadTbl->setFocus();

    //Click on first row to prevent uninitialized labels
    if (ui->homeStadTbl->rowCount() > 0)
    {
        ui->homeStadTbl->selectRow(0);
        refreshHomeDetails();
    }
}

void PrimeWin::refreshHomeDetails()
//Refreshes detail labels on the home page (Index 1)
{
    int stadNum;        //Stadium vector index number
    QString teams;      //Teams that play at the stadium
    QString address;    //Address of stadium
    QString cityZip;    //Everything in the address after the street
    QString capacity;   //Capacity of stadium

    //Initialize
    stadNum = ui->homeStadTbl
                    ->item(ui->homeStadTbl
                           ->currentRow(),0)->text().toInt();
    address = data.getStadAddress(stadNum);
    cityZip = address;
    capacity = QString::number(data.getStadCapactiy(stadNum));

    //Set labels and modify variables if necessary
    ui->homeStadzLbl->setText(data.getStadName(stadNum));
    for (unsigned int x=0;x<data.teamSize(stadNum);x++)
    {
        teams += data.getTeamName(stadNum,x) + "\n";
    }
    ui->homeTeamLbl->setText(teams);
    ui->homePhoneLbl->setText(data.getStadPhone(stadNum));
    address.resize(address.indexOf(","));   //Cut off city and zip
    ui->homeAddressLbl->setText(address);
    cityZip.remove(0,cityZip.indexOf(",") + 2); //Cut off street
    ui->homeCityZipLbl->setText(cityZip);
    ui->homeDateLbl->setText(data.getStadOpened(stadNum));
    capacity.insert(capacity.size() - 3,",");
    ui->homeCapLbl->setText(capacity);
    ui->homeTurfLbl->setText(data.getStadGrass(stadNum));
    ui->homeTypeLbl->setText(data.getStadType(stadNum));
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
//Index 5 = index management page
/*======================================================================*/

//Index0 - Start Page=====================================================
void PrimeWin::on_startInfoBt_clicked()
//Index 0 to 1
{
    refreshHome();
    ui->stackWidg->setCurrentIndex(1);
}

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

void PrimeWin::on_homeStadTbl_itemSelectionChanged()
//Refreshes home page when the home page's table selection changes
{refreshHomeDetails();}

//Index1 - Home Page======================================================
void PrimeWin::on_homeBackBt_clicked()
//Index 1 to 0
{ui->stackWidg->setCurrentIndex(0);}

void PrimeWin::on_homePlanTripBt_clicked()
//Index 1 to 2
{ui->stackWidg->setCurrentIndex(2);}

//Toggles a filter to filter out American League teams and only
//displays American league teams.
void PrimeWin::on_homeNationalCB_toggled(bool checked)
{
    //IF - Checks to see if our Checkbox is checked. If it is then
    //     it will filter out all American League teams. Else it will
    //     revert the table back to its original state.
    if(checked)
    {
        //FOR LOOP- This will loop through the table.
        for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)
        {
            //This QString will be used to find our vector index.
            QString strIndex = ui->homeStadTbl->item(i, 0)->text();

            int vecIndex = strIndex.toInt();    //Convert strIndex to an int.

            //IF - Here we find if the team league at the team indicated then
            //     it will hide all the rows that are American League team.
            if(data.getTeamLeague(vecIndex, 0)=="American")
            {
                ui->homeStadTbl->setRowHidden(i, true);
            } //end if(data.getTeamLeague(vecIndex, 0)=="American")

        } //end  for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)

    } // end if(checked)
    else
    {
        //FOR LOOP- This loop will traverse the homeStadTbl
        for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)
        {
            //IF- if the row at the given index is hidden then here
            //    it will 'unhide' the row.
            if(ui->homeStadTbl->isRowHidden(i))
            {
                ui->homeStadTbl->setRowHidden(i, false);
            }// end if(ui->homeStadTbl->isRowHidden(i))

        }// end for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)

    }// end else
}// end void PrimeWin::on_homeNationalCB_toggled(bool checked)



void PrimeWin::on_homeAmericanCB_toggled(bool checked)
{
    //IF - Checks to see if our Checkbox is checked. If it is then
    //     it will filter out all National League teams. Else it will
    //     revert the table back to its original state.
    if(checked)
    {
        //FOR LOOP- This will loop through the table.
        for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)
        {
            //This QString will be used to find our vector index.
            QString strIndex = ui->homeStadTbl->item(i, 0)->text();

            int vecIndex = strIndex.toInt();    //Convert strIndex to an int.

            //IF - Here we find if the team league at the team indicated then
            //     it will hide all the rows that are National League team.
            if(data.getTeamLeague(vecIndex, 0)=="National")
            {
                ui->homeStadTbl->setRowHidden(i, true);
            } //end if(data.getTeamLeague(vecIndex, 0)=="American")

        } //end  for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)

    } // end if(checked)
    else
    {
        //FOR LOOP- This loop will traverse the homeStadTbl
        for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)
        {
            //IF- if the row at the given index is hidden then here
            //    it will 'unhide' the row.
            if(ui->homeStadTbl->isRowHidden(i))
            {
                ui->homeStadTbl->setRowHidden(i, false);
            }// end if(ui->homeStadTbl->isRowHidden(i))

        }// end for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)

    }// end else
}//end void on_homeAmericanCB_toggled(bool checked)

//Index2 - Itinerary Page=================================================

//Index3 - Summary Page===================================================

//Index4 - Admin Page=====================================================
void PrimeWin::on_adminRestartBt_clicked()
//Exports to SQL database then restarts program
{
    //Export data structures
    if (data.exportSQL() == true)
    {
        //Restart the program if export is successful
        qDebug() << "Restarting program...";
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0],qApp->arguments());
    }
    else
    {
        qDebug() << "Program restart aborted.";
        QMessageBox::critical(this, tr("SQL Export Failure"),
                              tr("Failed to export data.\n"
                                 "Restart has been aborted."),
                              QMessageBox::Ok);
    }
}

void PrimeWin::on_adminBaseBt_clicked()
//Index 4 to 5
{
    ui->stackWidg->setCurrentIndex(5);
}

//Index5 - Database Management Page=======================================
void PrimeWin::on_dataBackBt_clicked()
//Index 5 to 4
{
    ui->stackWidg->setCurrentIndex(4);
}

void PrimeWin::on_dataTxtBt_clicked()
//Imports a text file and overwrites the entire data structure
{
    QString selection = dirmodel
            ->fileInfo(ui->dataFileBrowser
                       ->currentIndex()).absoluteFilePath();

    //Validate that selection is a text file
    if (selection.right(4) == ".txt")
    {
        if (!data.importTXT(selection))
        {
            QMessageBox::warning(this, tr("Cannot Overwrite"),
                                 tr("File did not open."),
                                 QMessageBox::Ok);
        }
        else
        {
            QMessageBox::information(this, tr("Data Overwritten"),
                                     tr("Data overwritten."),
                                     QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Cannot Overwrite"),
                             tr("Invalid selection.\n"
                                "Only text files can be imported.\n"),
                             QMessageBox::Ok);
    }
}



