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
    ui->homeStadTbl->setHorizontalHeaderLabels(QStringList() << "STADNUM"
                                               << "" << "Stadium"
                                               << "Team");

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
    ui->homeTeamLbl->setText(data.getTeamName(stadNum));
    ui->homeLeagueLbl->setText(data.getTeamLeague(stadNum) + " League");
    ui->homePhoneLbl->setText(data.getStadPhone(stadNum));
    address.resize(address.indexOf(","));   //Cut off city and zip
    ui->homeAddressLbl->setText(address);
    cityZip.remove(0,cityZip.indexOf(",") + 2); //Cut off street
    ui->homeCityZipLbl->setText(cityZip);
    ui->homeDateLbl->setText(data.getStadOpened(stadNum,true));
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
                            new QTableWidgetItem(data.getTeamName(i)));

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
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Stadium"
                                               << "Team"
                                               << "");
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

void PrimeWin::calcTrip()
//Calculates total trip distance of itinerary
{
    list<ItinObj>::iterator it;
    deque<int> distMap;
    int tripDist = 0;

    it = itinList.begin();

    while (it != itinList.end())
    {
        //Ask Dijkstra for distances to all other vertices from iterator
        distMap = data.askDijkstra((*it).getStadNum());

        //Look at next stadium in the itin
        it++;

        //Only add to running total if not at end
        if (it != itinList.end())
        {
            //Add the distance to that stad to the running total
            tripDist += distMap.at((*it).getStadNum());
        }
    }
    ui->itinDistLbl->setText("Total Distance: "
                             + QString::number(tripDist)
                             + " miles");
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

    //Calculate itinerary trip distance
    calcTrip();

    //Refresh the itinerary
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
    ui->homeNameRd->setChecked(true);
    ui->homeAmericanCB->setChecked(true);
    ui->homeNationalCB->setChecked(true);
    ui->homeTurfCB->setChecked(true);
    ui->homeSynthCB->setChecked(false);
    ui->homeSynthCB->click();
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

void PrimeWin::filterStads()
//Filters stadiums based on the checkbox filters
//Complexity: O(n)
{
    vector<bool> filteredStads;
    vector<int> applicableStadNums;
    bool applicable;

    //Iterate through all stadNums and see which ones need to be filtered
    for (int x = 0; x < (int)data.size(); x++)
    {
        //Initialize applicable to false, meaning filter this stad out
        applicable = false;

        //If filtering in National Teams
        if (ui->homeNationalCB->isChecked()
            && data.getTeamLeague(x).toLower() == "national")
        {applicable = true;}
        //If filtering in American Teams
        if (ui->homeAmericanCB->isChecked()
            && data.getTeamLeague(x).toLower() == "american")
        {applicable = true;}
        //If filtering in synthetic turf
        if (ui->homeSynthCB->isChecked()
            && data.getStadGrass(x).toLower() != "grass")
        {applicable = true;}
        //If filtering in natural turf
        if (ui->homeTurfCB->isChecked()
            && data.getStadGrass(x).toLower() == "grass")
        {applicable = true;}

        //Push the stadNum's applicability into the vector
        filteredStads.push_back(applicable);
    }
    //Create a vector of applicable stadNums
    for (int x = 0; x < (int)filteredStads.size(); x++)
    {
        if (filteredStads.at(x))
        {
            applicableStadNums.push_back(x);
        }
    }
    //Pass the stadNums into the refresh function
    refreshHomeTbl(applicableStadNums);

    //Reclick the sorting radio button that's currently selected
    if (ui->homeNameRd->isChecked())
    {on_homeNameRd_toggled(true);}
    else if (ui->homeTeamRd->isChecked())
    {
        int UNIMPLEMENTED;
    }
    else if (ui->homeDateRd->isChecked())
    {
        int NOTIMPLEMENTED;
    }
    else if (ui->homeCapRd->isChecked())
    {on_homeCapRd_toggled(true);}
    else if (ui->homeTypeRd->isChecked())
    {on_homeTypeRd_toggled(true);}

    //Hide contents of side panel if all stadiums are filtered out
    if (ui->homeStadTbl->rowCount() == 0)
    {
        ui->frame_6->hide();
        ui->frame_8->hide();
        ui->homeNirLbl->hide();
        ui->homeSouvTbl->hide();
    }
    //Otherwise make them reappear
    else
    {
        ui->frame_6->show();
        ui->frame_8->show();
        ui->homeNirLbl->show();
        ui->homeSouvTbl->show();
    }
}

void PrimeWin::on_homeAmericanCB_clicked()
{filterStads();}

void PrimeWin::on_homeNationalCB_clicked()
{filterStads();}

void PrimeWin::on_homeTurfCB_clicked()
{filterStads();}

void PrimeWin::on_homeSynthCB_clicked()
{filterStads();}

void PrimeWin::on_homeNameRd_toggled(bool checked)
//Sorts home's stadium table alphabetically by stadium name
//Complexity: O(n^2)
{
    //Perform sort when radio toggle is checked
    if (checked)
    {
        pair<int,QString> data;                //Data to be sorted
        vector<pair<int,QString> > sortThese;  //Vector of data to sort
        vector<int> stadNumOrder;              //Vector of stadNums
        CustomSorts use;                       //Sorting class

        //Create a list of stadNums to match the order the table is in now
        for (int x = 0; x < ui->homeStadTbl->rowCount(); x++)
        {
            data = make_pair(ui->homeStadTbl->item(x,0)->text().toInt(),
                             ui->homeStadTbl->item(x,2)->text());
            sortThese.push_back(data);
        }
        //Ask insertion sort to reorder the stadiums
        sortThese = use.InsertionSort(sortThese);

        //Build a vector of just stadNums that mirrors sortThese
        for (int x = 0; x < (int)sortThese.size(); x++)
        {
            stadNumOrder.push_back(sortThese.at(x).first);
        }
        //Refresh home table
        refreshHomeTbl(stadNumOrder);
    }
}

void PrimeWin::on_homeCapRd_toggled(bool checked)
//Sorts home table by capacity
{
    //Perform sort when radio toggle is checked
    if (checked)
    {
        pair<int,QString> dataP;                //Data to be sorted
        vector<pair<int,QString> > sortThese;   //Vector of data to sort
        vector<int> stadNumOrder;               //Vector of stadNums
        CustomSorts use;                        //Sorting class
        int stadNum;

        //Create a list of stadNums to match the order the table is in now
        for (int x = 0; x < ui->homeStadTbl->rowCount(); x++)
        {
            stadNum = ui->homeStadTbl->item(x,0)->text().toInt();
            dataP = make_pair(stadNum,QString::number
                              (data.getStadCapactiy(stadNum)));
            sortThese.push_back(dataP);
        }
        //Ask insertion sort to reorder the stadiums
        sortThese = use.InsertionSort(sortThese);

        //Build a vector of just stadNums that mirrors sortThese
        for (int x = 0; x < (int)sortThese.size(); x++)
        {
            stadNumOrder.push_back(sortThese.at(x).first);
        }
        //Refresh home table
        refreshHomeTbl(stadNumOrder);

        //Make the capacity label stand out
        ui->homeCapLbl->setStyleSheet("font-weight: bold; color: red");
    }
    else
    {
        //Restore capacity label
        ui->homeCapLbl->setStyleSheet("");
    }
}

void PrimeWin::on_homeTypeRd_toggled(bool checked)
//Sorts home table by park typology
{
    //Perform sort when radio toggle is checked
    if (checked)
    {
        pair<int,QString> dataP;                //Data to be sorted
        vector<pair<int,QString> > sortThese;   //Vector of data to sort
        vector<int> stadNumOrder;               //Vector of stadNums
        CustomSorts use;                         //Sorting class
        int stadNum;

        //Create a list of stadNums to match the order the table is in now
        for (int x = 0; x < ui->homeStadTbl->rowCount(); x++)
        {
            stadNum = ui->homeStadTbl->item(x,0)->text().toInt();
            dataP = make_pair(stadNum,data.getStadType(stadNum));
            sortThese.push_back(dataP);
        }
        //Ask insertion sort to reorder the stadiums
        sortThese = use.InsertionSort(sortThese);

        //Build a vector of just stadNums that mirrors sortThese
        for (int x = 0; x < (int)sortThese.size(); x++)
        {
            stadNumOrder.push_back(sortThese.at(x).first);
        }
        //Refresh home table
        refreshHomeTbl(stadNumOrder);

        //Make the typology label stand out
        ui->homeTypeLbl->setStyleSheet("font-weight: bold; color: red");
    }
    else
    {
        //Restore capacity label
        ui->homeTypeLbl->setStyleSheet("");
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
{
    //Check if there are enough stadiums to optimizeS
    if (itinList.size() > 2)
    {
        list<ItinObj> newItin;      //New itinerary
        std::deque<int> optimized;  //Optimized order of stadNums
        std::deque<int> djMap;      //Map of costs to visit each stadium
        int totalTripDist = 0;      //Total trip distance
        int shortest;               //Stores current shortest distance
        int nextStad;               //Stores next stad to visit

        //Create and initialize a list iterator
        std::list<ItinObj>::iterator it = itinList.begin();

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
        for (it = itinList.begin(); it != itinList.end(); it++)
        {
            visitAr[(*it).getStadNum()].valid = true;
        }
        //Reset itin iterator
        it = itinList.begin();

        //Mark current true in the hash map, visiting itin's first
        visitAr[(*it).getStadNum()].visited = true;

        //Add it to the NEW itinerary
        optimized.push_back((*it).getStadNum());

        //Build the optimized itinerary
        for (int i = 0; i < (int)itinList.size() - 1; i++)
        {
            //Call Dijkstra's on the last stadium on the optimized itin
            djMap = data.askDijkstra(optimized.back());

            //Reinitialize temporary values
            shortest = INT_MAX;
            nextStad = -1;

            //Find next stad in the itin that has the shortest dist
            for (int x = 0; x < (int)djMap.size(); x++)
            {
                //If stad is in the itin, unvisited, & has a shorter dist
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
        //Build the new itinerary
        for (int x = 0; x < (int)optimized.size(); x++)
        {
            it = itinList.begin();
            while (it != itinList.end())
            {
                if (optimized.at(x) == (*it).getStadNum())
                {
                    newItin.push_back(*it);
                    it = itinList.end();
                }
                else
                {
                    it++;
                }
            }
        }
        //Update the old itin wiht the new, updated one
        itinList = newItin;
        refreshItin();
        ui->itinDistLbl->setText("Total Distance: "
                                 + QString::number(totalTripDist)
                                 + " miles");
    }
    //Otherwise show an error
    else
    {
        QMessageBox::warning(this, tr("Not Enough Stadiums"),
                             tr("There needs to be at least three "
                                "stadiums queued in your itinerary."),
                             QMessageBox::Ok);
    }
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
        int ValidateThis;
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

void PrimeWin::on_adminPrimBt_clicked()
//Pulls up a QMessageBox that displays everything about Prim's MST
{
    QMessageBox msgBox;             //Message box to display tree info
    vector<pair<int,int> > edges;   //Vector of edges
    int mileage;                    //MST mileage

    //Ask Prim for the MST
    mileage = data.askPrim(edges);

    msgBox.setWindowTitle("Prim's MST");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("Minimum Spanning Tree Mileage: "
                   + QString::number(mileage) + " miles");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
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
    widget->setRowCount(2);
    widget->setColumnCount(0);
    QTableWidgetItem *item;

    //Loop to populate table
    for (int x = 0; x < data.getSouvListSize(stadNum); x++)
    {
        //Add a col
        widget->insertColumn(widget->columnCount());

        //Populate first row with souvName
        item = new QTableWidgetItem;
        item->setData(0,data.getSouvName(stadNum, x));
        widget->setItem(0,x,item);

        //Populate second row with souvPrice
        item = new QTableWidgetItem;
        item->setData(0,data.getSouvPrice(stadNum, x));
        widget->setItem(1,x,item);
    }
    widget->resizeColumnsToContents();
    widget->setRowHeight(0,60);
    widget->setRowHeight(1,60);
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
       QMessageBox::warning(this, tr("Error"),
                            tr("Select a stadium and a souvenir."),
                            QMessageBox::Ok);
   }
}
