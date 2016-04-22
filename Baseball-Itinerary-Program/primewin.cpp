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

    //Import data from SQL
    data.importSQL();

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
    ui->adminStadTbl->horizontalHeader()
            ->setDefaultAlignment(Qt::AlignLeft);

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

void PrimeWin::refreshHomeTbl(vector<int> stadNumOrder)
//Redraws table on home page, ordering based on stadNumOrder
//Complexity: O(n)
{
    QSignalBlocker stopSignalsFrom(ui->homeStadTbl);
    QTableWidgetItem *item;
    vector<int>::iterator it = stadNumOrder.begin();

    //REFRESH STADIUM TABLE
    ui->homeStadTbl->clear();
    ui->homeStadTbl->setRowCount(0);
    ui->homeStadTbl->setColumnCount(4);

    //Populate the stadium table
    for (unsigned int x = 0; x < stadNumOrder.size(); x++)
    {
        //Add a row
        ui->homeStadTbl->insertRow(ui->homeStadTbl->rowCount());

        //First col has hidden stadNum
        item = new QTableWidgetItem;
        item->setData(0,QString::number(*it));
        ui->homeStadTbl->setItem(x,0,item);

        //Second col has field pic
        int UNIMPLEMENTED;
        //item = new QTableWidgetItem;
        //item->setData(0,data.getPicOrSomething(*it));
        ui->homeStadTbl->setItem(x,1,new QTableWidgetItem("NO_PIC"));

        //Third col has stadium name
        item = new QTableWidgetItem;
        item->setData(0,data.getStadName(*it));
        ui->homeStadTbl->setItem(x,2,item);

        //Fourth col has team name
        item = new QTableWidgetItem;
        item->setData(0,data.getTeamName(*it));
        ui->homeStadTbl->setItem(x,3,item);

        //Advance iterator
        it++;
    }
    //Prepare table for viewing
    ui->homeStadTbl->hideColumn(0);
    ui->homeStadTbl->showColumn(1); //Not sure why this is needed

    ui->homeStadTbl->resizeColumnsToContents();

    //Click on first row to prevent uninitialized labels
    if (ui->homeStadTbl->rowCount() > 0)
    {
        ui->homeStadTbl->selectRow(0);
        refreshHomeDetails();
    }
    stopSignalsFrom.unblock();
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
    for (unsigned int x = 0; x < data.teamSize(stadNum); x++)
    {
        teams += data.getTeamName(stadNum,x) + "\n";
    }
    ui->homeTeamLbl->setText(teams);
    ui->homeLeagueLbl->setText(data.getTeamLeague(stadNum) + " League");
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

void PrimeWin::refreshItinBuilder()
//Refreshes the view of the itinerary builder (Index 2)
{
    //Clear table
    ui->tableWidget->clear();

    for(unsigned int i = 0; i < data.size(); i ++)
    {
        //Create new row
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        //Put the stadium name in column 1
        ui->tableWidget->setItem(i, 0,
                            new QTableWidgetItem(data.getStadName(i)));
        //Put the team name in colum 2
        ui->tableWidget->setItem(i, 1,
                            new QTableWidgetItem(data.getTeamName(i,0)));

        //Create an add button
        QPushButton *addBt = new QPushButton();
        addBt->setText("Add");
        //Set a the button to retrieve the stadium's num
        addBt->setProperty("stadNum", i);
        //Connect each button to addItinCatcher
        connect(addBt, SIGNAL(clicked(bool)),
                this, SLOT(catchAddItin()));
        //Put the add button in column 3
        ui->tableWidget->setCellWidget(i, 2, addBt);
    }

    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Stadium"
                                               << "Team"
                                               << "Add/Remove");
}

void PrimeWin::refreshItin()
//Refreshes the itineray view (Index 2)
{
    //Clear itinerary view
    ui->listWidget->clear();

    //Populate List
    for(std::list<ItinObj>::iterator it = itinList.begin();
        it != itinList.end(); it++)
    {
        ui->listWidget->addItem(data.getStadName(it->getStadNum()));
    }
}

void PrimeWin::refreshAdminTbl()
//Refreshes admin stadium table
{
    QSignalBlocker stopSignalsFrom(ui->adminStadTbl);
    QTableWidgetItem *item; //Item to populate table cell

    //Reset and setup table
    ui->adminStadTbl->clear();
    ui->adminStadTbl->setRowCount(0);
    ui->adminStadTbl->setColumnCount(11);

    //Name column headings
    ui->adminStadTbl->setHorizontalHeaderLabels
            (QStringList() << "#" << "Stadium" << "Team" << "League"
             << "Phone" << "Address" << "Capacity" << "Turf" << "Opened"
             << "Type" << "Revenue");

    //Loop to populate table
    for (unsigned int x = 0; x < data.size(); x++)
    {
        //Add a row
        ui->adminStadTbl->insertRow(ui->adminStadTbl->rowCount());

        //Populate first column with stadNums
        item = new QTableWidgetItem;
        item->setData(0,QString::number(x));
        ui->adminStadTbl->setItem(x,0,item);

        //Populate second column with stadName
        item = new QTableWidgetItem;
        item->setData(0,data.getStadName(x));
        ui->adminStadTbl->setItem(x,1,item);

        //Populate third column with team name
        item = new QTableWidgetItem;
        item->setData(0,data.getTeamName(x));
        ui->adminStadTbl->setItem(x,2,item);

        //Populate fourth column with team league
        item = new QTableWidgetItem;
        item->setData(0,data.getTeamLeague(x));
        ui->adminStadTbl->setItem(x,3,item);

        //Populate fifth column with the phone number
        item = new QTableWidgetItem;
        item->setData(0,data.getStadPhone(x));
        ui->adminStadTbl->setItem(x,4,item);

        //Populate sixth column with address
        item = new QTableWidgetItem;
        item->setData(0,data.getStadAddress(x));
        ui->adminStadTbl->setItem(x,5,item);

        //Populate seventh column with capacity
        item = new QTableWidgetItem;
        item->setData(0,QString::number(data.getStadCapactiy(x)));
        ui->adminStadTbl->setItem(x,6,item);

        //Populate eigth column with grass type
        item = new QTableWidgetItem;
        item->setData(0,data.getStadGrass(x));
        ui->adminStadTbl->setItem(x,7,item);

        //Populate ninth column with opening date
        item = new QTableWidgetItem;
        item->setData(0,data.getStadOpened(x));
        ui->adminStadTbl->setItem(x,8,item);

        //Poplate tenth column with stadium type
        item = new QTableWidgetItem;
        item->setData(0,data.getStadType(x));
        ui->adminStadTbl->setItem(x,9,item);

        //Populate eleventh column with park revenue
        int NOTDONE;
        item = new QTableWidgetItem;
        item->setData(0,"$REVENUE");
        ui->adminStadTbl->setItem(x,10,item);
    }
    ui->adminStadTbl->resizeColumnsToContents();
    stopSignalsFrom.unblock();
}

QString PrimeWin::phoneCheck(QString phone)
//Validates phone numbers and returns a formatted number
{
    QString bareNumber;     //Cleaned up input
    bool hasPlus = false;   //Determines if there's a plus or not
    bool morePlus = false;  //Determines if there are duplicate + signs
    bool throwGen = false;  //Determines if generic error is necessary

    //Iterate through all characters, keeping only + and numbers
    for(QString::iterator it = phone.begin(); it != phone.end(); it++)
    {
        if (*it == '0' || *it == '1' || *it == '2' || *it == '3'
            || *it == '4' || *it == '5' || *it == '6' || *it == '7'
            || *it == '8' || *it == '9' || *it == '+')
        {
            bareNumber += *it;

            //If there's a plus, flag it
            if (*it == '+')
            {
                //If hasPlus is already true, then mark flag duplicate
                if (hasPlus)
                {
                    morePlus = true;
                }
                //Otherwise just mark it true
                else
                {
                    hasPlus = true;
    }   }   }   }
    //Check if there are duplicate +'s
    if (morePlus)
    {
        QMessageBox::warning(this, tr("Invalid Phone Number"),
                             tr("There can only be one \"+\" in a "
                                "phone number."),
                             QMessageBox::Ok);
        return "NULL";
    }
    //Check if phone number is the correct length if it has a +
    if (hasPlus && (bareNumber.size() - bareNumber.indexOf("+") == 12
                    || bareNumber.size() - bareNumber.indexOf("+") == 11))
    {}//It's good, I'm just too lazy to DeMorganize the conditional
    //Check if phone number has the correct length if there's no +
    else if (!hasPlus && (bareNumber.size() == 11
                          || bareNumber.size() == 10))
    {}//Same deal
    else
    {throwGen = true;}

    //Format the number if nothing's wrong
    if (!throwGen)
    {
        //Insert the start of the area code
        //If there's a +
        if (hasPlus)
        {
            //If the size is 11
            if (bareNumber.size() - bareNumber.indexOf("+") == 12)
            {
                //Insert space after the first character after the +
                bareNumber.insert(bareNumber.indexOf("+") + 2, " (");
            }
            //If the size is 10
            else
            {
                //Insert ( after the +
                bareNumber.insert(bareNumber.indexOf("+") + 1, "(");
            }
        }
        //If there's no +
        else
        {
            //If size is 11
            if (bareNumber.size() == 11)
            {
                bareNumber.insert(1, " (");
            }
            //If size is 10
            else
            {
                bareNumber.insert(0, "(");
            }
        }
        //Finish area code
        bareNumber.insert(bareNumber.indexOf("(") + 4, ") ");
        //Add the dash
        bareNumber.insert(bareNumber.lastIndexOf(" ") + 4,"-");
        return bareNumber;
    }
    //Otherwise throw the generic error
    else
    {
        QMessageBox::warning(this, tr("Invalid Phone Number"),
                             tr("Phone number is not valid."),
                             QMessageBox::Ok);
        return "NULL";
    }
}

/*PUBLIC SLOTS==========================================================*/
void PrimeWin::catchLoginStatus(bool status)
//Catches signal to see if login is good or not
{
    //Change page to admin home page if status is true
    if (status)
    {refreshAdminTbl();ui->stackWidg->setCurrentIndex(4);}
}

void PrimeWin::catchDataUpdate(Data caughtThis)
//Catches signal to update data structures
{
    data = caughtThis;
}

void PrimeWin::catchAddItin()
//Catches signal to update itin
{
//    //Block signals to prevent unintended behavior
//    const QSignalBlocker blockItinTbl(ui->itineraryTbl);

    //Create button object to emululate the button that was pushed
    QPushButton *addBt = qobject_cast<QPushButton*>(sender());

    //Get the stadium's num from the button's property
    int stadNum = addBt->property("stadNum").toInt();

    //If adding to itin
    if (addBt->text() == "Add")
    {
        //Add to the itin
        itinList.push_back(ItinObj(stadNum));

        //?????????show possible souvenirs??????????
        int UNIMPLEMENTED;

        //Change the bt's text to remove
        addBt->setText("Remove");
    }
    //Otherwise, remove from the itin
    else if (addBt->text() == "Remove")
    {
        //Loop until stadium is found
        for (std::list<ItinObj>::iterator it = itinList.begin();
             it != itinList.end(); it++)
        {
            //Check to see if stadium is found
            if (it->getStadNum() == stadNum)
            {
                //Remove the stadium from the itinerary queue
                itinList.erase(it);

                //Leave loop
                it = itinList.end();
            }
        }

        //Change the bt's text back to add
        addBt->setText("Add");
    }

    refreshItin();
    //?????????show possible souvenirs??????????
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
    ui->homeNameRd->setChecked(true);
    ui->homeAmericanCB->setChecked(true);
    ui->homeNationalCB->setChecked(true);
    ui->homeTurfCB->setChecked(true);
    ui->homeSynthCB->setChecked(true);
    ui->stackWidg->setCurrentIndex(1);
}

void PrimeWin::on_startTripBt_clicked()
//Index 0 to 2
{
    refreshItinBuilder();
    ui->stackWidg->setCurrentIndex(2);
}

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
{
    refreshItinBuilder();
    ui->stackWidg->setCurrentIndex(2);
}

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
            if(data.getTeamLeague(vecIndex, 0).toLower()=="american")
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
                if(ui->homeTurfCB->isChecked())
                {
                    if(data.getStadGrass(i).toLower() !="astro turf")
                    {
                        ui->homeStadTbl->setRowHidden(i, false);
                    }
                }
                else if(ui->homeSynthCB->isChecked())
                {
                    if(data.getStadGrass(i).toLower()!="grass")
                    {
                        ui->homeStadTbl->setRowHidden(i, false);
                    }
                }
                else if(ui->homeAmericanCB->isChecked())
                {
                    if(data.getTeamLeague(i, 0).toLower()!="national")
                    {
                        ui->homeStadTbl->setRowHidden(i, false);
                    }
                }
                else
                {
                    ui->homeStadTbl->setRowHidden(i, false);
                }
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
            if(data.getTeamLeague(vecIndex, 0).toLower()=="national")
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
                if(ui->homeTurfCB->isChecked())
                {
                    if(data.getStadGrass(i).toLower() !="astro turf")
                    {
                        ui->homeStadTbl->setRowHidden(i, false);
                    }
                }
                else if(ui->homeSynthCB->isChecked())
                {
                    if(data.getStadGrass(i).toLower()!="grass")
                    {
                        ui->homeStadTbl->setRowHidden(i, false);
                    }
                }
                else if(ui->homeNationalCB->isChecked())
                {
                    if(data.getTeamLeague(i, 0).toLower()!="american")
                    {
                        ui->homeStadTbl->setRowHidden(i, false);
                    }
                }
                else
                {
                    ui->homeStadTbl->setRowHidden(i, false);
                }
            }// end if(ui->homeStadTbl->isRowHidden(i))

        }// end for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)

    }// end else
}//end void on_homeAmericanCB_toggled(bool checked)



void PrimeWin::on_homeTurfCB_toggled(bool checked)
{
    //IF-This if statement will look for the checkbox that we made is checked
    //   and if it is then it will look for all stadiums that use natural grass
    //   and filter them out.
    if(checked)
    {
        //FOR-Loops through each item of each table to find the items in the
        //    homeStadTble to filter out.

        for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)
        {
            //This QString will be used to find our vector index.
            QString strIndex = ui->homeStadTbl->item(i, 0)->text();

            int vecIndex = strIndex.toInt();    //Convert strIndex to an int.

            //IF-The item at the given index has 'grass' then it will filter it
            //   out the grass.
            if(data.getStadGrass(vecIndex).toLower() == "astro turf")
            {
                ui->homeStadTbl->setRowHidden(i, true);
            }// if(data.getStadGrass(vecIndex) == "Astro Turf")

        }// end for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)

    }// end if(checked)
    else
    {
        //FOR LOOP- This loop will traverse the homeStadTbl
        for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)
        {
            //IF- if the row at the given index is hidden then here
            //    it will 'unhide' the row.
            if(ui->homeStadTbl->isRowHidden(i))
            {
                //IF- if the row at the given index is hidden then here
                //    it will 'unhide' the row.
                if(ui->homeStadTbl->isRowHidden(i))
                {
                    if(ui->homeNationalCB->isChecked())
                    {
                        if(data.getTeamLeague(i, 0).toLower() !="american")
                        {
                            ui->homeStadTbl->setRowHidden(i, false);
                        }
                    }
                    else if(ui->homeSynthCB->isChecked())
                    {
                        if(data.getStadGrass(i).toLower()!="astro turf")
                        {
                            ui->homeStadTbl->setRowHidden(i, false);
                        }
                    }
                    else if(ui->homeAmericanCB->isChecked())
                    {
                        if(data.getTeamLeague(i, 0).toLower()!="national")
                        {
                            ui->homeStadTbl->setRowHidden(i, false);
                        }
                    }
                    else
                    {
                        ui->homeStadTbl->setRowHidden(i, false);
                    }
                }// end if(ui->homeStadTbl->isRowHidden(i))

            }// end if(ui->homeStadTbl->isRowHidden(i))

        }// end for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)

    }// end else

}// end void PrimeWin::on_homeTurfCB_toggled(bool checked)


void PrimeWin::on_homeSynthCB_toggled(bool checked)
{
    //IF-This if statement will look for the checkbox that we made is checked
    //   and if it is then it will look for all stadiums that use natural grass
    //   and filter them out.
    if(checked)
    {
        //FOR-Loops through each item of each table to find the items in the
        //    homeStadTble to filter out.

        for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)
        {
            //This QString will be used to find our vector index.
            QString strIndex = ui->homeStadTbl->item(i, 0)->text();

            int vecIndex = strIndex.toInt();    //Convert strIndex to an int.

            //IF-The item at the given index has 'grass' then it will filter it
            //   out the grass.
            if(data.getStadGrass(vecIndex).toLower() == "grass")
            {
                ui->homeStadTbl->setRowHidden(i, true);
            }// if(data.getStadGrass(vecIndex) == "Astro Turf")

        }// end for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)

    }// end if(checked)
    else
    {
        //FOR LOOP- This loop will traverse the homeStadTbl
        for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)
        {
            //IF- if the row at the given index is hidden then here
            //    it will 'unhide' the row.
            if(ui->homeStadTbl->isRowHidden(i))
            {
                //IF- if the row at the given index is hidden then here
                //    it will 'unhide' the row.
                if(ui->homeStadTbl->isRowHidden(i))
                {
                    if(ui->homeNationalCB->isChecked())
                    {
                        if(data.getTeamLeague(i, 0).toLower() !="american")
                        {
                            ui->homeStadTbl->setRowHidden(i, false);
                        }
                    }
                    else if(ui->homeTurfCB->isChecked())
                    {
                        if(data.getStadGrass(i).toLower()!="astro turf")
                        {
                            ui->homeStadTbl->setRowHidden(i, false);
                        }
                    }
                    else if(ui->homeAmericanCB->isChecked())
                    {
                        if(data.getTeamLeague(i, 0).toLower()!="national")
                        {
                            ui->homeStadTbl->setRowHidden(i, false);
                        }
                    }
                    else
                    {
                        ui->homeStadTbl->setRowHidden(i, false);
                    }
                }// end if(ui->homeStadTbl->isRowHidden(i))

            }// end if(ui->homeStadTbl->isRowHidden(i))

        }// end for(int i = 0; i < ui->homeStadTbl->rowCount(); i++)

    }// end else
}// end void

void PrimeWin::on_homeNameRd_toggled(bool checked)
//Sorts home's stadium table alphabetically by stadium name
//Complexity: O(n^2)
{
    //Perform sort when radio toggle is checked
    if (checked)
    {
        SortObj object;             //Object to be pushed into sortThese
        vector<SortObj> sortThese;  //Vector of stadiums to be sorted
        vector<int> stadNumOrder;   //Vector of stadNums
        CustomSorts use;          //Sorting class

        //Create a list of stadNums to match the order the table is in now
        for (int x = 0; x < ui->homeStadTbl->rowCount(); x++)
        {
            object.stadNum = ui->homeStadTbl->item(x,0)->text().toInt();
            object.sortee = ui->homeStadTbl->item(x,2)->text();
            sortThese.push_back(object);
        }
        //Ask insertion sort to reorder the stadiums
        sortThese = use.InsertionSort(sortThese);

        //Build a vector of just stadNums that mirrors sortThese
        for (int x = 0; x < (int)sortThese.size(); x++)
        {
            stadNumOrder.push_back(sortThese.at(x).stadNum);
        }
        //Refresh home table
        refreshHomeTbl(stadNumOrder);
    }
}

//Index2 - Itinerary Page=================================================
void PrimeWin::on_itinStartOverBt_clicked()
//Index 2 to 0
{
    ui->stackWidg->setCurrentIndex(0);
    int UNFINISHED;//needs to clear itinerary
}

void PrimeWin::on_itinOptimizeBt_clicked()
//Optimizes order of the itinerary
//Complexity: O(n^2)
{int UNFINISHED;
    //TEST ITINERARY
    std::list<int> itin;
    itin.push_back(0);
    itin.push_back(17);
    itin.push_back(20);
    itin.push_back(27);
    itin.push_back(10);
    itin.push_back(6);

    std::deque<int> optimized;  //Optimized order of stadNums
    std::deque<int> djMap;      //Map of costs to visit each stadium
    int totalTripDist = 0;      //Total trip distance
    int shortest;               //Stores current shortest distance
    int nextStad;               //Stores next stad to visit

    //Create and initialize a list iterator
    std::list<int>::iterator it = itin.begin();

    //Struct to represent a stadium in the itinerary
    struct visitObj
    {
        bool visited;   //If visited
        bool valid;     //If in itinerary
    };
    //Array of visited booleans where index is stadNum
    visitObj visitAr[data.size()];

    //Initialize the array to the uninitialized states
    for (unsigned int x = 0; x < data.size(); x++)
    {
        visitAr[x].visited = false;
        visitAr[x].valid = false;
    }
    //Make stadiums in the itin valid within the array
    for (it = itin.begin(); it != itin.end(); it++)
    {
        visitAr[*it].valid = true;
    }
    //Reset itin iterator
    it = itin.begin();

    //Mark current true (visited) in the hash map, visiting itin's first
    visitAr[*it].visited = true;

    //Add it to the NEW itinerary
    optimized.push_back(*it);

    //Build the optimized itinerary
    for (int i = 0; i < (int)itin.size() - 1; i++)
    {
        //Call Dijkstra's on the last stadium on the optimized itinerary
        djMap = data.askDijkstra(optimized.back());

        //Reinitialize temporary values
        shortest = INT_MAX;
        nextStad = -1;

        //Find next stad in the itin that has the shortest dist
        for (int x = 0; x < (int)djMap.size(); x++)
        {
            //If stad is in the itin & not visited & it has a shorter dist
            if (visitAr[x].valid
                && !visitAr[x].visited && djMap[x] < shortest)
            {
                //Update shortest and the next stad to visit
                shortest = djMap[x];
                nextStad = x;
            }
        }
        //Add that distance to a running total
        totalTripDist += shortest;

        //Mark it as visited on the visited array
        visitAr[nextStad].visited = true;

        //Add it to the NEW itinerary
        optimized.push_back(nextStad);
    }
    //Return new itinerary and the total distance travelled
    qDebug() << "OPTIMAL";
    for (int x = 0; x < optimized.size(); x++)
    {
        qDebug() << optimized[x];
    }
    qDebug() << "total trip distance is" << totalTripDist;
}

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

void PrimeWin::on_adminDistBt_clicked()
//Opens distance editing dialog
{
    //Construct new dialog
    editdistances newDistDialog(this,data);

    //Line main window up to catch newDistDialog's signal
    connect(&newDistDialog,SIGNAL(throwUpdatedData(Data)),
            this,SLOT(catchDataUpdate(Data)));

    //Display the dialog
    newDistDialog.exec();
}

void PrimeWin::on_adminStadTbl_cellChanged(int row, int column)
//Applies edits from the admin table to the data structure
{
    QString input;
    QIntValidator validate(1,1000000,NULL);
    int valid = 0;

    //Grab the input
    input = ui->adminStadTbl->item(row,column)->text();

    //Depending on column selected, make appropriate data changes
    switch (column)
    {
    case 0://StadNum
        QMessageBox::warning(this, tr("Editing Error"),
                             tr("Editing stadium numbers is forbidden."),
                             QMessageBox::Ok);
        break;
    case 1://Stadium
        data.modStadName(row,input);
        break;
    case 2://Team
        data.modTeam(row,input);
        break;
    case 3://League
        //Validate league
        input = input.simplified();
        input.replace(" ","");
        input = input.toLower();
        if (input == "national" || input == "american")
        {
            input[0] = input[0].toUpper();
            data.modLeague(row,input);
        }
        //If neither national nor american, throw error
        else
        {
            QMessageBox::warning(this, tr("Editing Error"),
                                 tr("League can only be \"National\" "
                                    "or \"American\"."),
                                 QMessageBox::Ok);
        }
        break;
    case 4://Phone
        if (phoneCheck(input) != "NULL")
        {
            data.modStadPhone(row,phoneCheck(input));
        }
        break;
    case 5://Address
        data.modStadAddress(row,input);
        break;
    case 6://Capacity
        //Validate input
        valid = validate.validate(input,valid);
        if (valid == 0)
        {
            QMessageBox::warning(this, tr("Editing Error"),
                                 tr("Invalid capacity.\n"
                                    "Must be between 0 & 1,000,001."),
                                 QMessageBox::Ok);
        }
        else
        {
            data.modStadCapacity(row,input.toInt());
        }
        break;
    case 7://Turf
        data.modStadGrass(row,input);
        break;
    case 8://Opened
        data.modStadOpened(row,input);
        break;
    case 9://Type
        data.modStadType(row,input);
        break;
    case 10://Revenue
        int UNIMPLEMENTED;
        qDebug() << "REVENUE EDITING IS UNIMPLEMENTED";
        break;
    default:
        QMessageBox::critical(this, tr("Editing Critical Error"),
                              tr("Column switch case defaulted!"),
                              QMessageBox::Ok);
        break;
    }
    refreshAdminTbl();
}

//Index5 - Database Management Page=======================================
void PrimeWin::on_dataBackBt_clicked()
//Index 5 to 4
{
    refreshAdminTbl();
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

void PrimeWin::refreshSouvenirTableAdmin()
//Refreshes admin page's souvenir table
{
    int stadNum = ui->adminStadTbl->item(ui->adminStadTbl->currentRow(),
                                         0)->text().toInt();
    QTableWidget *widget = ui->adminSouvTable;

    //REFRESH SOUVENIR TABLE
    widget->clear();
    widget->setRowCount(0);
    widget->setColumnCount(2);
    QTableWidgetItem *item; //Item to populate table cell
    widget->setHorizontalHeaderLabels(QStringList() << "Item Name"
                                                    << "Price");

    //Loop to populate table
    for (int x = 0; x < data.getSouvListSize(stadNum); x++)
    {
        //Add a row
        widget->insertRow(widget->rowCount());

        //Populate second column with souvName
        item = new QTableWidgetItem;
        item->setData(0,data.getSouvName(stadNum, x));
        widget->setItem(x,0,item);

        //Populate third column with souvPrice
        item = new QTableWidgetItem;
        item->setData(0,data.getSouvPrice(stadNum, x));
        widget->setItem(x,1,item);
    }
    widget->resizeColumnsToContents();
}

// when an index is selected, the bottom panel will display a list of souvenirs
// that corresponds to its stadium
void PrimeWin::on_adminStadTbl_itemSelectionChanged()
{ refreshSouvenirTableAdmin(); }

// on Add New Souvenir Button
void PrimeWin::on_pushButton_9_clicked()
{
    //Construct new dialog
    // also pass in data object so we can access data structures
    addSouvDialog newAddSouvWin(data, this);
    connect(&newAddSouvWin,SIGNAL(throwNewSouvData(Data)),
            this,SLOT(catchNewSouvenirData(Data)));
    newAddSouvWin.exec();
}

// process new souvenir data
void PrimeWin::catchNewSouvenirData(Data caughtData)
{data = caughtData;}

void PrimeWin::on_adminBackBtn_clicked()
{ ui->adminHome->hide();  ui->start->show();}

// on Delete Souvenir Button
void PrimeWin::on_deleteSouvBtn_clicked()
{
   // get selected row
   int stadNum = ui->adminStadTbl->selectionModel()->currentIndex().row();
   int itemNum = ui->adminSouvTable->selectionModel()->currentIndex().row();

   // error checking
   if(stadNum != -1 && itemNum != -1)
   {
          // delete souvenir
          data.deleteSouv(stadNum, itemNum);
          refreshSouvenirTableAdmin();
   }
   else
   {
       //notify admin to make a selection on souvenir
       QMessageBox::information(this, tr("Error"),
                            tr("Please select a stadium and a souvenir to remove."),
                            QMessageBox::Ok);
   }
}

