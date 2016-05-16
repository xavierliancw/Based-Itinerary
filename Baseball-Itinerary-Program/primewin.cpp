/**
  @file
  @author jerryberry
  @date 28 April 2016
  @brief This file contains the PrimeWin class methods.
  */
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
    ui->listWidget->viewport()->installEventFilter(this);
    ui->tableWidget->setHorizontalHeader(new myHeaderView(this));
    ui->totalSouvLbl->setText("Total Number of Souvenirs: 0");
    ui->totalPriceLbl->setText("Grand Total: $0.00");
    ui->tabWidget->setCurrentIndex(0);

    //Setup start screen's gif
    QMovie *titleMovie = new QMovie(":/defaults/title.gif");
    QLabel *movieLabel = new QLabel(ui->start);
    movieLabel->setMovie(titleMovie);
    movieLabel->setAlignment(Qt::AlignCenter);
    QStackedLayout *stackLay = new QStackedLayout;
    stackLay->addWidget(ui->frame_17);
    stackLay->addWidget(ui->frame_19);
    stackLay->addWidget(movieLabel);
    stackLay->setStackingMode(QStackedLayout::StackAll);
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addLayout(stackLay);
    ui->start->setLayout(lay);
    movieLabel->show();
    titleMovie->start();

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
        ui->homeStadTbl->setRowHeight(x,75);

        //First col has hidden stadNum
        item = new QTableWidgetItem;
        item->setData(0,QString::number(*it));
        ui->homeStadTbl->setItem(x,0,item);

        //Second col has field pic
        QTableWidgetItem *fieldPic = new QTableWidgetItem();
        fieldPic->setData(Qt::DecorationRole,
                          QPixmap(data.getStadFieldPicPath(*it))
                          .scaled(100,100,Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation));
        ui->homeStadTbl->setItem(x,1,fieldPic);

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
        refreshHomeSouvTbl();
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
    QPixmap logo(data.getTeamLogoPath(stadNum));
    ui->homeLogoLbl->setPixmap(logo.scaled(150,100,Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation));
}

void PrimeWin::refreshItinBuilder()
//Refreshes the view of the itinerary builder (Index 2)
{
    //Clear table
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

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

    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Stadium"
                                               << "Team"
                                               << "ADD ALL");
    ui->tableWidget->resizeColumnsToContents();
}

void PrimeWin::refreshItinSouv(int stadNum)
//Refreshes the view of the souvenir list (Index 2)
{
    QSignalBlocker blockSignals(ui->itinSouvTbl);

    list<ItinObj>::iterator itinPosn;
    list<int> existingSouv;

    //Look for stadium in itinList
    for(list<ItinObj>::iterator it = itinList.begin();
        it != itinList.end(); it++)
    {
        //If the stadium is found
        if(it->getStadNum() == stadNum)
        {
            //Push its souvenir into the list
            for(int i = 0; i < it->getCartSize(); i++)
            {
                existingSouv.push_back(it->getSouvNumAt(i));
            }
            //Record position
            itinPosn = it;
        }
    }
    //Sort the list to allow for easy popping
    if (!existingSouv.empty())
    {
        existingSouv.sort();
    }

    //Refresh the menu's view
    ui->itinSouvTbl->clear();
    ui->itinSouvTbl->setColumnCount(data.getSouvListSize(stadNum));
    ui->itinSouvTbl->setRowCount(2);
    for (int i = 0; i < data.getSouvListSize(stadNum); i++)
    {
        //Construct button to add to wish list
        QToolButton *addWish = new QToolButton();

        //Construct text label and layout to word wrap
        QHBoxLayout *boundary = new QHBoxLayout();
        QLabel *wordWrap = new QLabel();
        wordWrap->setText(data.getSouvName(stadNum,i) + "\n$"
                          + QString::number
                          (data.getSouvPrice(stadNum,i), 'f', 2));
        wordWrap->setWordWrap(true);
        wordWrap->setAlignment(Qt::AlignCenter);
        wordWrap->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        wordWrap->setMaximumHeight(100);
        wordWrap->setMaximumWidth(110);
        boundary->addWidget(wordWrap);
        addWish->setText("");
        addWish->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        addWish->setMinimumHeight(100);
        addWish->setMinimumWidth(110);
        addWish->setMaximumHeight(100);
        addWish->setMaximumWidth(110);
        addWish->setLayout(boundary);

        //Set custom properties
        addWish->setProperty("souvNum",i);
        addWish->setProperty("Add/Remove",0);

        //Connect each button to a public slot in PrimeWindow
        connect(addWish, SIGNAL(clicked(bool)),
                this, SLOT(catchAddWish()));

        //Draw the button on the table
        ui->itinSouvTbl->setCellWidget(0,i,addWish);

        //Add a spinbox to the second row if the souv is found in the itin
        if (i == existingSouv.front() && !existingSouv.empty())
        {
            //Pop the front off of existing souvs
            existingSouv.pop_front();

            //Construct a quantity spinbox widget
            QFrame *qtyFrame = new QFrame();
            QHBoxLayout *qtySpinLayout = new QHBoxLayout();
            QLabel *qtySpinLbl = new QLabel();
            QSpinBox *qtySpinBox = new QSpinBox();
            qtySpinLbl->setText("Qty:");
            qtySpinBox->setRange(1,99);
            qtySpinBox->setValue(itinPosn->getQtyFor(i));
            qtySpinLayout->addWidget(qtySpinLbl);
            qtySpinLayout->addWidget(qtySpinBox);
            qtyFrame->setLayout(qtySpinLayout);

            //Set custom property to track location of spinbox
            qtySpinBox->setProperty("stadNum",stadNum);
            qtySpinBox->setProperty("souvNum",i);

            //Connect each spinbox to a public slot in MainWindow
            connect(qtySpinBox, SIGNAL(valueChanged(int)),
                   this, SLOT(catchUpdateQuantity(int)));

            //Add frame to ui
            ui->itinSouvTbl->setCellWidget(1,i,qtyFrame);
        }
    }

    //Resize table
    ui->itinSouvTbl->resizeColumnsToContents();
    ui->itinSouvTbl->resizeRowsToContents();

    blockSignals.unblock();
}

void PrimeWin::refreshItin()
//Refreshes the itineray view (Index 2)
{
    QSignalBlocker blockSignals(ui->listWidget);
    //Clear itinerary view
    ui->listWidget->clear();

    //Populate List
    for(std::list<ItinObj>::iterator it = itinList.begin();
        it != itinList.end(); it++)
    {
        ui->listWidget->addItem(data.getStadName(it->getStadNum()));
    }

    blockSignals.unblock();
}

void PrimeWin::refreshWishList()
//Refreshes the wish list view (Index 2)
{
    int row = 0; //row in table
    int totalSouv = 0; //total number of souvenirs in wish list
    double totalPrice = 0; //total prive of wish list

    //Clear and set columns and initialize first row
    ui->wishTbl->clear();
    ui->wishTbl->setRowCount(row);
    ui->wishTbl->setColumnCount(5);
    ui->wishTbl->setHorizontalHeaderLabels(QStringList()
                                           << "Stadium"
                                           << "Souvenir"
                                           << "Price"
                                           << "Quantity"
                                           << "Total Price");

    //Go through each stadium in the intin list
    for(list<ItinObj>::iterator it = itinList.begin();
        it != itinList.end(); it++)
    {
        //Go through cart at eac stadium
        for(int i = 0; i < it->getCartSize(); i++)
        {
            //Create new row
            ui->wishTbl->insertRow(row);

            //Put the stadium name in column 1
            ui->wishTbl->setItem(row, 0, new QTableWidgetItem(
                                     data.getStadName(
                                         it->getStadNum())));
            //Put the souvenir name in colum 2
            ui->wishTbl->setItem(row, 1, new QTableWidgetItem(
                                     data.getSouvName(it->getStadNum(),
                                                      it->getSouvNumAt(i))));
            //Put the price of souvenir in column 3
            ui->wishTbl->setItem(row, 2, new QTableWidgetItem(
                                     "$" + QString::number(
                                         data.getSouvPrice(it->getStadNum(),
                                                           it->getSouvNumAt(i)),
                                         'f', 2)));
            //Put the quantity in coulumn 4
            ui->wishTbl->setItem(row, 3, new QTableWidgetItem(
                                     "x" +QString::number(
                                         it->getQtyFor(it->getSouvNumAt(i)))));
            //Put the total price in column 5
            ui->wishTbl->setItem(row, 4, new QTableWidgetItem(
                                     "$" + QString::number(
                                         it->getQtyFor(it->getSouvNumAt(i)) *
                                         data.getSouvPrice(it->getStadNum(),
                                                           it->getSouvNumAt(i)),
                                         'f', 2)));

            //Update the totalSouv and totalPrice
            totalSouv += it->getQtyFor(it->getSouvNumAt(i));
            totalPrice += it->getQtyFor(it->getSouvNumAt(i)) *
                    data.getSouvPrice(it->getStadNum(), it->getSouvNumAt(i));

            row++; //move to next row
        }
    }

    //Format table columns
    ui->wishTbl->resizeColumnsToContents();

    //Output the total count
    ui->totalSouvLbl->setText("Total Number of Souvenirs: "
                              + QString::number(totalSouv));
    ui->totalPriceLbl->setText("Grand Total: $"
                               + QString::number(totalPrice, 'f', 2));
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
        item = new QTableWidgetItem;
        item->setData(0,"$" + data.getStadRev(x));
        ui->adminStadTbl->setItem(x,10,item);
    }
    ui->adminStadTbl->resizeColumnsToContents();
    stopSignalsFrom.unblock();
}

void PrimeWin::calcTrip()
//Calculates total trip distance of itinerary
//Complexity: O(n^2)
{
    list<ItinObj>::iterator it;
    deque<int> distMap;
    int tripDist = 0;
    vector<int> paths;

    it = itinList.begin();

    while (it != itinList.end())
    {
        //Ask Dijkstra for distances to all other vertices from iterator
        distMap = data.askDijkstra((*it).getStadNum(), paths);

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
    ui->itinCountLbl->setText("Stadiums Queued: "
                              + QString::number(itinList.size()));
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
{data = caughtThis;changesMade();}

void PrimeWin::catchAddItin()
//Catches signal to update itin
{
    //Block signals to prevent unintended behavior
    QSignalBlocker blockSignals(ui->listWidget);

    //Create button object to emululate the button that was pushed
    QPushButton *addBt = qobject_cast<QPushButton*>(sender());

    //Get the stadium's num from the button's property
    int stadNum = addBt->property("stadNum").toInt();

    //If adding to itin
    if (addBt->text() == "Add")
    {
        //Add to the itin
        itinList.push_back(ItinObj(stadNum));

        ui->tableWidget->selectRow(stadNum);

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
            }
        }

        //Change the bt's text back to add
        addBt->setText("Add");
    }

    //Calculate itinerary trip distance
    calcTrip();

    //Refresh the itinerary
    refreshItin();
    refreshItinSouv(stadNum);
    blockSignals.unblock();
}

void PrimeWin::catchAddWish()
//Catches signal to update wish list
{
    //Block signals to prevent unintended behavior
    QSignalBlocker blockSignals(ui->itinSouvTbl);

    //Emulated tool button
    QToolButton *addWish = qobject_cast<QToolButton*>(sender());
    //Current stadium
    int stadNum = ui->tableWidget->currentRow();
    //Current souvenir
    int souvNum = addWish->property("souvNum").toInt();
    bool stadIsQueued = false; //If stadium is queued
    bool souvIsQueued = false; //If souvenir is queued
    list<ItinObj>::iterator itinPos; //Position of stadium

    //Search for stadium in itinerary
    for(list<ItinObj>::iterator it = itinList.begin();
        it != itinList.end(); it++)
    {
        if(it->getStadNum() == stadNum)
        {
            //If found save iterator
            stadIsQueued = true;
            itinPos = it;

            //Search if souvenir is in itinerary
            for(int i = 0; i < it->getCartSize(); i ++)
            {
                if(it->getSouvNumAt(i) == souvNum)
                {
                    souvIsQueued = true;
                }
            }
        }
    }

    //If stadium is not found
    if(!stadIsQueued)
    {
        //Add to Itin and update position iterator
        itinList.push_back(ItinObj(stadNum));
        itinPos = itinList.end();
        itinPos --;

        //Update Add/Remove button
        QPushButton *addBt = new QPushButton();
        addBt->setText("Remove");
        addBt->setProperty("stadNum", stadNum);
        connect(addBt, SIGNAL(clicked(bool)), this,
                SLOT(catchAddItin()));
        ui->tableWidget->setCellWidget(stadNum, 2, addBt);
    }

    //If souvenir is not found
    if(!souvIsQueued)
    {
        //Add souvenir to cart
        itinPos->pushCart(souvNum, 1);
        //Refresh souv menu
        refreshItinSouv(stadNum);
    }
    else
    {
        //Remove souvenir from cart
        itinPos->delCart(souvNum);
        //Refresh souv menu
        refreshItinSouv(stadNum);
    }

    //Refresh itinerary and wish list
    refreshItin();
    refreshWishList();

    blockSignals.unblock();
}

void PrimeWin::catchUpdateQuantity(int newQty)
//Catches signal to update quantity of souvenir
{
    //Block signals to prevent unintended behavior
    QSignalBlocker blockSignals(ui->itinSouvTbl);

    //Emulated spin box
    QSpinBox *qtySpinBox = qobject_cast<QSpinBox*>(sender());
    //Current stadium
    int stadNum = qtySpinBox->property("stadNum").toInt();
    //Current souvenir
    int souvNum = qtySpinBox->property("souvNum").toInt();

    //Search for stadium in itinerary
    for(list<ItinObj>::iterator it = itinList.begin();
        it != itinList.end(); it++)
    {
        if(it->getStadNum() == stadNum)
        {
            //Search for souvenir in cart
            for(int i = 0; i < it->getCartSize(); i ++)
            {
                if(it->getSouvNumAt(i) == souvNum)
                {
                    //Update Quantity
                    it->chgQty(souvNum, newQty);
                }
            }
        }
    }

    //Refresh wish list
    refreshWishList();

    blockSignals.unblock();
}

bool PrimeWin::eventFilter(QObject *object, QEvent *event)
//Event filter to detect drag and drops within the itinerary
//Complexity: O(n^2)
{
    //Only consider events happening inside the itinerary
    if (object == ui->listWidget->viewport())
    {
        QDropEvent *theDrop;  //Simulates whatever is being dropped
        QPoint pos;           //Helps model determine which row to drop to
        QModelIndex model;    //Determines row drop location
        list<ItinObj>::iterator it; //Itin list iterator

        //Setup drop data
        theDrop = static_cast<QDropEvent*>(event);
        pos = theDrop->pos();
        model = ui->listWidget->indexAt(pos);

        //Grab the data of the stadium being dragged
        if (event->type() == QEvent::DragEnter)
        {
            it = itinList.begin();
            for (int x = 0; x < ui->listWidget->currentRow(); x++)
            {
                it++;
            }
            dragDrop = *it;
            pickup = ui->listWidget->currentRow();
        }
        //Drop information
        if (event->type() == QEvent::Drop)
        {
            //Ignore Qt's drop
            theDrop->setDropAction(Qt::IgnoreAction);

            //Search for dragged item's starting point
            it = itinList.begin();
            for (int x = 0; x < pickup; x++)
            {
                it++;
            }
            //Erase the dragged entry
            itinList.erase(it);

            //Search for the item's insertion point
            it = itinList.begin();
            for (int x = 0; x < model.row(); x++)
            {
               it++;
            }
            //Insert the dragged entry into the correct spot
            itinList.insert(it,dragDrop);

            //Redraw the itinerary
            refreshItin();
            refreshWishList();
            calcTrip();
        }
    }
    return false;
}

void PrimeWin::catchAddAllStadsCmd()
//Catches command to add all stadiums to the itinerary
//Complexity: O(nlogn)
{
    //Block signals for itinWidget to stop it from showing randomly
    QSignalBlocker blockSignals(ui->listWidget);

    list<int> alreadyQd;        //List to see what stads are already in
    list<ItinObj>::iterator it; //Itinerary iterator

    //Build a list of stads that are already in the itin
    it = itinList.begin();
    while (it != itinList.end())
    {
        alreadyQd.push_back((*it).getStadNum());
        it++;
    }
    //Sort the stadNums
    alreadyQd.sort();

    //Loop until all stads are added while adding existing queued stads
    for (int x = 0; x < (int)data.size(); x++)
    {
        //If the stadNum is already queued, don't push it in
        if (x == alreadyQd.front())
        {
            //Pop it off the list
            alreadyQd.pop_front();
        }
        //Otherwise, just add an empty stad to the new itin
        else
        {
            //Create an ItinObj to push in where x is the stadNum
            ItinObj newItinObj(x);
            itinList.push_back(newItinObj);
        }
    }
    //Refresh the itin's view
    refreshItin();
    calcTrip();

    //Rebuild the buttons in the itinBuilder list to say "Remove"
    for (int x = 0; x < (int)data.size(); x++)
    {
        //Create a remove button for column 3
        QPushButton *remBt = new QPushButton();
        remBt->setText("Remove");

        //Set a custom property to the button to send the stad's num
        remBt->setProperty("stadNum", x);

        //Connect each button to addItinCatcher
        connect(remBt, SIGNAL(clicked(bool)),
                this, SLOT(catchAddItin()));

        //Put the remove button in column 3
        ui->tableWidget->setCellWidget(x, 2, remBt);
    }
    //Unblock signals
    blockSignals.unblock();
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
    refreshItinSouv(0);
    ui->tableWidget->selectRow(0);
    ui->tabWidget->setCurrentIndex(0);
    ui->wishTbl->setColumnCount(0);
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
{
    refreshHomeDetails();
    refreshHomeSouvTbl();
}

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

void PrimeWin::refreshHomeSouvTbl()
//Refreshes home page's souvenir table
{
    int stadNum = ui->homeStadTbl->item(ui->homeStadTbl->currentRow(),
                                         0)->text().toInt();
    QTableWidget *widget = ui->homeSouvTbl;

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
        item->setData(0,"$"
                      + QString::number(data.getSouvPrice(stadNum,
                                                          x),'f',2));
        item->setTextAlignment(Qt::AlignCenter);
        widget->setItem(1,x,item);
    }
    widget->resizeColumnsToContents();
    widget->setRowHeight(0,40);
    widget->setRowHeight(1,40);
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
    {on_homeTeamRd_toggled(true);}
    else if (ui->homeDateRd->isChecked())
    {on_homeDateRd_toggled(true);}
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
            //Make the capacity label stand out
            ui->homeStadzLbl
              ->setStyleSheet("font-weight: bold; color: red");
        }
        else
        {
            //Restore capacity label
            ui->homeStadzLbl->setStyleSheet("");
        }
}

void PrimeWin::on_homeTeamRd_toggled(bool checked)
//Sorts home stadium table by team name, alphabetically
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
            dataP = make_pair(stadNum, data.getTeamName(stadNum));
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

        //Make the team label stand out
        ui->homeTeamLbl->setStyleSheet("font-weight: bold; color: red");
    }
    else
    {
        //Restore team label
        ui->homeTeamLbl->setStyleSheet("");
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
        CustomSorts use;                        //Sorting class
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

void PrimeWin::on_homeDateRd_toggled(bool checked)
// Sorts the stadiums by their opening dates (Chronological Order)
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

            // pass in false to return yyyyMMdd
            dataP = make_pair(stadNum,data.getStadOpened(stadNum, false));
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
        ui->homeDateLbl->setStyleSheet("font-weight: bold; color: red");
    }
    else
    {
        //Restore capacity label
        ui->homeDateLbl->setStyleSheet("");
    }
}

//Index2 - Itinerary Page=================================================
void PrimeWin::on_itinSearchBarLE_textChanged(const QString &arg1)
//Handles the search bar function
{
    //If the bar is empty, show everything again
    if (arg1 == "")
    {
        for (int x = 0; x < (int)data.size(); x++)
        {
            ui->tableWidget->showRow(x);
        }
    }
    //Otherwise, filter
    else
    {
        itinSearchFilter(arg1);
    }
}

void PrimeWin::itinSearchFilter(QString filter)
//Filters itin stad table to show only things that have filter
{
    QList<QTableWidgetItem *> items;
    items = ui->tableWidget->findItems(filter, Qt::MatchContains);

    //Hide all rows
    for (int x = 0; x < (int)data.size(); x++)
    {
        ui->tableWidget->hideRow(x);
    }
    //Make rows appear that match search
    for (int x = 0; x < items.count(); x++)
    {
        ui->tableWidget->showRow(items.at(x)->row());
    }
}

void PrimeWin::on_tableWidget_cellClicked(int row/*, int column*/)
//Updates the souvenier menu
{
    refreshItinSouv(row);
}

void PrimeWin::on_itinStartOverBt_clicked()
//Index 2 to 0
{
    ui->stackWidg->setCurrentIndex(0);
    ui->wishTbl->clear();
    ui->wishTbl->setRowCount(0);
    itinList.clear();
    ui->totalSouvLbl->setText("Total Number of Souvenirs: 0");
    ui->totalPriceLbl->setText("Grand Total: $0.00");
    refreshItin();
    calcTrip();
}

//Struct to represent a stadium in the itinerary
/**
 * @brief The visitObj struct helps the itinerary to optimze its order
 *
 * This struct holds two variables. The first is a boolean that tells
 * the optimizing algorithm if the stadium the struct is representing
 * has already been visited by the algorithm. The second boolean tells
 * the algorithm if the stadium is actually in the itinerary, and is
 * valid to be optimized.
 */
struct visitObj
{
    bool visited;   //If visited
    bool valid;     //If in itinerary
};

/**
 * @brief The based_on_second struct provides instructions for std::sort
 * to compare only the second integer of integer std::pairs
 *
 * It does this by overloading the () operator
 *
 * PrimeWin::on_itinOptimizeBt_clicked() uses this to sort an std::list of
 * edges to determine which one is the shortest in O(nlogn) time.
 * @see PrimeWin::on_itinOptimizeBt_clicked()
 */
struct based_on_second
//Instructs list.sort() functions to sort using the cost
{
    bool operator()(const std::pair<int,int> &left,
                    const std::pair<int,int> &right)
    {return left.second < right.second;}
};

void PrimeWin::on_itinOptimizeBt_clicked()
//Optimizes order of the itinerary
//Complexity: O(n^2)
//n+i+i(elogn+n+ilogi+i) - n^2 proof
{
    //Check if there are enough stadiums to optimizeS
    if (itinList.size() > 2)
    {
        pair<bool,bool> optStatus;          //Valid,Visited
        vector<pair<bool,bool> > valvisMap; //Map of valid/visited stads
        list<ItinObj>::iterator it;         //Itin iterator
        list<ItinObj> optimal;              //New optimal itin
        deque<int> djMap;                   //Map of shortest dists
        pair<int,int> stadDist;             //stadNum/dist pair
        list<pair<int,int> > sortThese;     //List that gets sorted
        int totalTripDist = 0;              //Tracks trip distance
        vector<int> paths;                  //Tracks paths

        //Initialize valid/visited map such that all are invalid/unvisted
        for (int x = 0; x < (int)data.size(); x++)
        {
            optStatus = make_pair(false,false);
            valvisMap.push_back(optStatus);
        }
        //See what's queued in the itin, and mark them as valid stads
        it = itinList.begin();
        while (it != itinList.end())
        {
            valvisMap.at((*it).getStadNum()).first = true;
            it++;
        }
        //Push itin's starting stad into the new itin
        it = itinList.begin();
        optimal.push_back(*it);

        //Loop until the size of the new itin matches the old itin
        while (optimal.size() < itinList.size())    //O(i), i = # queued
        {
            //Mark the back of optimal as visited in the map
            valvisMap.at(optimal.back().getStadNum()).second = true;

            //Ask Dijkstra for shortest paths from optimal's back
            djMap = data.askDijkstra(optimal.back().getStadNum(),
                                     paths);//elogn

            //Build a list of VALID & UNVISITED stads
            for (int x = 0; x < (int)djMap.size(); x++)//O(n), n = # stads
            {
                //If a stadium is in the itin and it's unvisited
                if (valvisMap.at(x).first == true
                    && valvisMap.at(x).second == false)
                {
                    //Make a stadNum/it's-dist-from-optimal's-back pair
                    stadDist = make_pair(x,djMap.at(x));
                    sortThese.push_back(stadDist);
                }
            }
            //Sort that list to figure out which valid stad is closest
            sortThese.sort(based_on_second());

            //Push sortThese's top stad into optimal
            //Look for the stad in the original itin
            it = itinList.begin();
            while (it != itinList.end())    //O(i), i = #queued
            {
                //If it's found
                if ((*it).getStadNum() == sortThese.front().first)
                {
                    //Push it into the optimal list
                    optimal.push_back(*it);

                    //Update the trip distance
                    totalTripDist += djMap.at((*it).getStadNum());

                    //Leave the loop
                    it = itinList.end();
                }
                else
                {
                    it++;
                }
            }
            sortThese.clear();
        }
        //Update itinList to most optimal trip
        itinList = optimal;
        refreshItin();
        refreshWishList();
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

void PrimeWin::on_itinFinalizeBt_clicked()
{
    const int STAD_WID = 28;
    const int SOUV_NAME_WID = 20;
    const int SOUV_PRICE_WID = 5;
    const int SOUV_QTY_WID = 2;
    const int TOTAL_PRICE_WID = 7;

    list<ItinObj>::iterator it;
    list<ItinObj>::iterator it2;
    int stadListIndex;
    int stadNum;
    QString stadName;
    int souvNum;
    QString souvName;
    double souvPrice;
    int souvQty;
    QString finalOutput;
    int stadNum2;
    QString stadName2;
    deque<int> distances;
    vector<int> parents;
    vector<int> path;
    int pathIndex;
    int tripDist;
    double tripTotalPrice;

    stadListIndex = 1;
    tripDist = 0;
    tripTotalPrice = 0;

    finalOutput.append("    STADIUM" + QString(24, ' ') +
                       "SOUVENIR" + QString(13, ' ') +
                       "PRICE  #   TOTAL\n" + QString(75, '-') + "\n");

    it2 = itinList.begin();
    it2 ++;
    for(it = itinList.begin(); it != itinList.end(); it++)
    {
        stadNum = it->getStadNum();
        stadName = data.getStadName(stadNum);

        finalOutput.append(QString(2 - QString::number(
                                       stadListIndex).size(), ' ') +
                           QString::number(stadListIndex) + ". " +
                           stadName +
                           QString(STAD_WID - stadName.size(), ' '));
        stadListIndex ++;

        for(int i = 0; i < it->getCartSize(); i ++)
        {
            souvNum = it->getSouvNumAt(i);
            souvName = data.getSouvName(stadNum, souvNum);
            souvPrice = data.getSouvPrice(stadNum, souvNum);
            souvQty = it->getQtyFor(souvNum);
            tripTotalPrice += souvQty * souvPrice;

            if(i > 0)
            {
                finalOutput.append(QString(STAD_WID, ' ') + "    ");
            }
            finalOutput.append(" - " + souvName +
                               QString(SOUV_NAME_WID - souvName.size(),
                                       ' ') +
                               " $" +
                               QString(SOUV_PRICE_WID - QString::number(
                                           souvPrice, 'f', 2).size(),
                                       ' ') +
                               QString::number(souvPrice, 'f', 2) +
                               " x" +
                               QString(SOUV_QTY_WID - QString::number(
                                           souvQty).size(), ' ') +
                               QString::number(souvQty) +
                               " $" + QString(TOTAL_PRICE_WID -
                                              QString::number(
                                                  souvQty * souvPrice,
                                                  'f', 2).size(), ' ') +
                               QString::number(souvQty * souvPrice,
                                               'f', 2));
            if(i < it->getCartSize() - 1)
            {
                finalOutput.append("\n");
            }
        }

        finalOutput.append("\n" + QString(75, '-') + "\n");

        if(it2 != itinList.end())
        {
            stadNum2 = it2->getStadNum();
            stadName2 = data.getStadName(stadNum2);
            distances = data.askDijkstra(stadNum, parents);
            path.clear();
            pathIndex = stadNum2;

            finalOutput.append("    FROM " + stadName + " TO " +
                               stadName2 + "\n");


            while(pathIndex > -1)
            {
                path.push_back(pathIndex);
                pathIndex = parents[pathIndex];
            }

            if(path.size() > 2)
            {
                finalOutput.append("    LAYOVERS:\n");
                for(unsigned int i = path.size() - 2; i > 0; i--)
                {
                    finalOutput.append("     - " + data.getStadName(path[i]) +
                                       "\n");
                }
            }
            else
            {
                finalOutput.append("    NO LAYOVERS\n");
            }

            finalOutput.append("    DISTANCE TRAVELED: " +
                               QString::number(distances.at(stadNum2)) +
                               "\n" + QString(75, '-') + "\n");

            tripDist += distances.at(stadNum2);
        }

        it2 ++;
    }

    finalOutput.append("\nNUMBER OF STADIUMS VISITED:       " +
                       QString::number(itinList.size()) +
                       "\nTOTAL DISTANCE TRAVELED IN MILES: " +
                       QString::number(tripDist) +
                       "\nSOUVENIR PRICE TOTAL:             $" +
                       QString::number(tripTotalPrice, 'f', 2) +
                       "\n\nTHANK YOU FOR USING OUR PROGRAM!");

    ui->sumOutputLE->setText(finalOutput);

    if(itinList.size() > 0)
    {
        ui->stackWidg->setCurrentIndex(3);
    }
    else
    {
        QMessageBox::warning(this, tr("Nothing Queued"),
                             tr("There is nothing queued to your "
                                "itinerary."), QMessageBox::Ok);
    }
}

//Index3 - Summary Page===================================================
void PrimeWin::on_sumBackBt_clicked()
{
    ui->stackWidg->setCurrentIndex(2);
}

void PrimeWin::on_sumFinishBt_clicked()
{
    //Update stadium revenues
    list<ItinObj>::iterator it = itinList.begin();
    double revenue;
    while (it != itinList.end())
    {
        //Get revenue stadium already has
        revenue = data.getStadRev((*it).getStadNum()).toDouble();

        //Loop through all souvenirs queued at this stad
        for (int x = 0; x < (*it).getCartSize(); x++)
        {
            //Quantity x price
            revenue += (double)(*it).getQtyFor((*it).getSouvNumAt(x))
                    * data.getSouvPrice((*it).getStadNum(),
                                        (*it).getSouvNumAt(x));
        }
        //Update revenue
        data.modStadRev((*it).getStadNum(),revenue);
        it++;
    }
    itinList.clear();
    refreshItin();
    refreshItinBuilder();
    ui->itinSouvTbl->setRowCount(0);
    refreshWishList();
    ui->stackWidg->setCurrentIndex(0);
}

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

    //Check for isolated stadiums
    checkForIsolatedStads();
}

void PrimeWin::checkForIsolatedStads()
//Check for stadiums that are not connected to the graph
{
    int checker;
    bool aDisconnectExists = true;
    QString offendingStad;

    //Loop through all stadiums and check if any have only -1's
    for (int x = 0; x < (int)data.size(); x++)
    {
        aDisconnectExists = true;
        checker = -1;               //Start with a failure conditions

        //Loop through all possible stadium connections
        for (int y = 0; y < (int)data.size(); y++)
        {
            //If at least one connection exists
            if (data.getDistBetween(x,y) > -1)
            {
                checker++;
                y = data.size();//Leave loop, check next stad
            }
        }
        //If no connections were found
        if (checker == -1)
        {
            offendingStad = data.getStadName(x);
            x = data.size();//Leave the loop and return failure condition
        }
        //Otherwise this stadium is fine
        else
        {
            aDisconnectExists = false;
        }
    }
    //If there's an isolated stadium, bring up the distance edit window
    if (aDisconnectExists)
    {
        offendingStad += " is not connected. "
                         "Connect it before proceeding.";
        QMessageBox::warning(this, tr("Distance Error"),
                             QObject::tr(qPrintable(offendingStad)),
                             QMessageBox::Ok);
        on_adminDistBt_clicked();
    }
    //Otherwise, it's all good
}

void PrimeWin::changesMade()
//Updates the changes made label to show that changes need to be saved
{ui->adminChangesLbl->setText("There are unsaved changes...");}

void PrimeWin::on_adminStadTbl_cellChanged(int row, int column)
//Applies edits from the admin table to the data structure
{
    QString input;
    QIntValidator validate(1,1000000,NULL);
    int valid = 0;
    double newPrice;
    QLocale us; //Constructs a default QLocale
    QDate validQuestion;

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
        //Validate date
        validQuestion = QDate::fromString(input,"dd MM yyyy");
        if(validQuestion.isValid())
        {
            //Validate year to year baseball was invented
            if (validQuestion.year() >= 1839)
            {
                data.modStadOpened(row,input);
            }
            //Otherwise show an error
            else
            {
                QMessageBox::warning(this, tr("Editing Error"),
                                     tr("Invalid date.\nDate must be "
                                        "after the year 1839, when "
                                        "baseball was invented."),
                                     QMessageBox::Ok);
            }
        }
        //If not valid, show an error
        else
        {
            QMessageBox::warning(this, tr("Editing Error"),
                                 tr("Invalid date.\nDate must be "
                                    "formatted dd mm yyyy.\ne.g. 12 05 "
                                    "2016 for May 12, 2016."),
                                 QMessageBox::Ok);
        }
        break;
    case 9://Type
        data.modStadType(row,input);
        break;
    case 10://Revenue
        bool ok;
        QLocale::setDefault(QLocale(QLocale::English,
                                    QLocale::UnitedStates));
        us.toDouble(input, &ok);
        newPrice = input.toDouble();
        input = QString::number(newPrice, 'f', 2);
        newPrice = input.toDouble();
        if (newPrice > INT_MAX - 1 || newPrice < 0)
        {
            ok = false;
        }
        if (ok)
        {data.modStadRev(row,newPrice);}
        else
        {
            QMessageBox::warning(this, tr("Revenue Editing Error"),
                                 tr("Price must be positive and less"
                                    " than $2,147,483,646.00."),
                                 QMessageBox::Ok);
        }
        break;
    default:
        QMessageBox::critical(this, tr("Editing Critical Error"),
                              tr("Column switch case defaulted!"),
                              QMessageBox::Ok);
        break;
    }
    refreshAdminTbl();
    changesMade();
}

// when a cell in admin souvenir is modified
void PrimeWin::on_adminSouvTable_cellChanged(int row, int column)
{
    int stadNum =ui->adminStadTbl->selectionModel()->currentIndex().row();
    QString newName;
    double newPrice;
    bool ok;
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    QLocale us; // Constructs a default QLocale

    //Grab the input
    if(row == 0) // if souvenir name is selected
    {
        newName = ui->adminSouvTable->item(row,column)->text();
    }
    else // if souvenir price is selected
    {
        //Also format and validate
        QString newPriceStr = ui->adminSouvTable->item(row,column)->text();
        us.toDouble(newPriceStr, &ok);
        newPrice = newPriceStr.toDouble();
        newPriceStr = QString::number(newPrice, 'f', 2);
        newPrice = newPriceStr.toDouble();
        if (newPrice > 99999.99 || newPrice < 0)
        {
            ok = false;
        }
    }
    switch(row)
    {
        // Souvenir item name
        case 0:
            //If the name is left blank, don't do anything
            if (newName != "")
            {
                data.modSouvName(stadNum, column, newName);
            }
            break;
        // Souvenir item price
        case 1:
           //If the price is validated
            if (ok)
            {
                data.modSouvPrice(stadNum, column, newPrice);
            }
            else
            {
                QMessageBox::warning(this, tr("Souvenir Editing Error"),
                                     tr("Price must be valid and less"
                                        " than $99999.99."),
                                     QMessageBox::Ok);
            }
            break;
    default:
        QMessageBox::critical(this, tr("Editing Critical Error"),
                              tr("Row switch case defaulted!"),
                              QMessageBox::Ok);
    }
    refreshSouvenirTableAdmin();
    changesMade();
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

void PrimeWin::on_adminPrimBt_clicked()
//Opens Prim's dialog
{
    //Warn user
    if (ui->adminChangesLbl->text() != "")
    {
        QMessageBox::information(this, tr("Warning"),
                                 tr("MST may not reflect changes made "
                                    "during this session.\nPlease "
                                    "save and restart to obtain most "
                                    "accurate MST."),
                                 QMessageBox::Ok);
    }

    //Construct new dialog
    MstPrim newPrimDialog(data,this);

    //Display the dialog
    newPrimDialog.exec();
}

void PrimeWin::newTeamRefresh()
//Prompt user to connect new stad and refreshes the UI to show changes
{
    //Pull up distance window to prompt user to connect the stadium
    QMessageBox::information(this, tr("New Stadium"),
                             tr("Now make at least one connection to "
                                "another stadium on the map."),
                             QMessageBox::Ok);
    on_adminDistBt_clicked();
    refreshAdminTbl();
}

// Admin Page - On Add New Team (and corresponding stadium) Button
void PrimeWin::on_addNewTeamBtn_clicked()
{
    //Construct new dialog
    AddStadiumWin newWin(data, this);
    connect(&newWin,SIGNAL(throwNewTeamData(Data)),
            this,SLOT(catchNewTeamData(Data)));
    connect(&newWin,SIGNAL(throwRefreshCmd()),
            this,SLOT(newTeamRefresh()));
    newWin.exec();
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
        changesMade();
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
    QSignalBlocker stopSignalsFrom(ui->adminSouvTable);

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
        item->setData(0,QString::number(data.getSouvPrice(stadNum,
                                                          x),'f',2));
        widget->setItem(1,x,item);
    }
    widget->resizeColumnsToContents();
    widget->setRowHeight(0,60);
    widget->setRowHeight(1,60);

    //Update feedback label
    ui->adminSouvFeedbackLbl->setText("Souvenirs at: "
                                      + data.getStadName(stadNum));
    stopSignalsFrom.unblock();
}

// when an index is selected, the bottom panel will display a list of souvenirs
// that corresponds to its stadium
void PrimeWin::on_adminStadTbl_itemSelectionChanged()
{
    refreshSouvenirTableAdmin();
}

// on Add New Souvenir Button
void PrimeWin::on_pushButton_9_clicked()
{
    //Construct new dialog
    addSouvDialog newAddSouvWin(data, this);
    connect(&newAddSouvWin,SIGNAL(throwNewSouvData(Data,int)),
            this,SLOT(catchNewSouvenirData(Data,int)));
    newAddSouvWin.exec();
}

// process new souvenir data and refresh the ui
void PrimeWin::catchNewSouvenirData(Data caughtData, int stadChanged)
{
    data = caughtData;
    ui->adminStadTbl->selectRow(stadChanged);
    refreshSouvenirTableAdmin();
    changesMade();
}

// process new Team data
void PrimeWin::catchNewTeamData(Data caughtData)
{data = caughtData;changesMade();}

// on Delete Souvenir Button
void PrimeWin::on_deleteSouvBtn_clicked()
{
   // get selected row
   int stadNum = ui->adminStadTbl->selectionModel()->currentIndex().row();
   int itemNum = ui->adminSouvTable->selectionModel()
                   ->currentIndex().column();

   // error checking
   if(stadNum != -1 && itemNum != -1)
   {
          // delete souvenir
          data.deleteSouv(stadNum, itemNum);
          refreshSouvenirTableAdmin();
          changesMade();
   }
   else
   {
       if (stadNum == -1)
       {
           //notify admin to make a selection on souvenir
           QMessageBox::warning(this, tr("Deletion Error"),
                                tr("Please select a stadium first."),
                                QMessageBox::Ok);
       }
       else if (itemNum == -1)
       {
           QMessageBox::warning(this, tr("Deletion Error"),
                                tr("Please select a souvenir to delete."),
                                QMessageBox::Ok);
       }
       else
       {
           qDebug() << "Wow, another kind of error happened in "
                       "PrimeWin::on_deleteSouvBtn_clicked().";
       }
   }
}
