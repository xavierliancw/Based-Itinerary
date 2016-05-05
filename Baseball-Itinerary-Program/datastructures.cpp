/**
  @file
  @author jerryberry
  @date 27 April 2016

  @brief This file contains the methods for the TeamObj, SouvObj, StadObj,
  ItinObj, and Data classes.
  */
#include "datastructures.h"

//========================================================================
/*ItinObj METHODS*/

ItinObj::ItinObj()
//Default constructor
{}

ItinObj::ItinObj(int stadium)
//Overloaded constructor
{
    stadNum = stadium;
    itinSouvList.clear();
    itinSouvQuant.clear();
}

void ItinObj::pushCart(int souvNum, int qty)
//Pushes a souvenir to the cart at the current stadium
{
    itinSouvList.push_back(souvNum);
    itinSouvQuant.push_back(qty);
}

void ItinObj::delCart(int souvNum)
//Deletes a souvenir from the cart
{
    bool found = false; //Validation boolean

    //Look for the souvenir in the itinerary
    for (unsigned int x = 0; x < itinSouvList.size(); x++)
    {
        //If the souvenir in question is found
        if (itinSouvList.at(x) == souvNum)
        {
            //Delete it
            std::vector<int>::iterator it = itinSouvList.begin() + x;
            itinSouvList.erase(it);
            itinSouvQuant.erase(it);

            //Skip exception
            found = true;

            //Exit loop
            x = itinSouvList.size();
        }
    }
    //Throw an exception if souvenir is not found
    if (!found)
    {
        qDebug() << "Cannot find souvenir in cart to delete";
    }
}

void ItinObj::chgQty(int souvNum, int newQty)
//Changes the quantity of the indexed souvenir
{
    bool found = false; //Validation boolean

    //Look for the souvenir in the itinerary
    for (unsigned int x = 0; x < itinSouvList.size(); x++)
    {
        //If the souvenir in question is found
        if (itinSouvList.at(x) == souvNum)
        {
            //Change its quantity
            itinSouvQuant.at(x) = newQty;

            //Skip exception
            found = true;

            //Exit loop
            x = itinSouvList.size();
        }
    }
    //Throw an exception if souvenir is not found
    if (!found)
    {
        qDebug() << "Cannot find souvenir in cart to change";
    }
}

int ItinObj::getStadNum() const
//Returns stadium number
{return stadNum;}

int ItinObj::getCartSize() const
//Returns the size of the shopping cart at the current stadium
{return itinSouvList.size();}

int ItinObj::getSouvNumAt(int index) const
//Returns the souvenir at index
{return itinSouvList.at(index);}

int ItinObj::getQtyFor(int souvNum) const
//Returns the quantity of the souvenir bought
{
    int quantity = -1;   //Quantity of a souvenir in the cart

    //Search for souvenir in itin
    for (unsigned int x = 0; x < itinSouvList.size(); x++)
    {
        //If the souvenir is found, prep to return its quantity
        if (souvNum == itinSouvList.at(x))
        {
            quantity = itinSouvQuant.at(x);
            x = itinSouvList.size(); //Leave loop
        }
    }
    //Throw an exception if souvenir isn't found
    if (quantity < 0)
    {
        qDebug() << "Cannot find souvenir in cart";
    }
    return quantity;
}

//========================================================================
/*Data METHODS*/

Data::Data()
//Constructor
{}

Data::~Data()
//Destructor
{}

void Data::addStad(QString name, QString address,
                   QString phone, QString opened, int capacity,
                   QString grass, QString type)
//Add a stadium to the end of the vector
{
    StadObj newStad;    //New stadium object

    //Populate the object
    newStad.name = name;
    newStad.address = address;
    newStad.phone = phone;
    newStad.opened = QDate::fromString(opened,"dd MM yyyy");
    newStad.capacity = capacity;
    newStad.grass = grass;
    newStad.type = type;

    //Push the stadium into the vector (it's vector index is its stadNum)
    masterVect.push_back(newStad);
}

void Data::modStadName(int stadNum, QString newName)
//Modifies name of stadNum
{masterVect.at(stadNum).name = newName;}

void Data::modStadAddress(int stadNum, QString newAddress)
//Modifies address of stadNum
{masterVect.at(stadNum).address = newAddress;}

void Data::modStadPhone(int stadNum, QString newPhone)
//Modifies phone number of stadNum
{masterVect.at(stadNum).phone = newPhone;}

void Data::modStadOpened(int stadNum, QString newOpened)
//Modifies opening date of stadNum
{
    masterVect.at(stadNum).opened
            = QDate::fromString(newOpened,"MMMM dd, yyyy");
}

void Data::modStadCapacity(int stadNum, int newCapacity)
//Modifies stadNum's capacity
{masterVect.at(stadNum).capacity = newCapacity;}

void Data::modStadGrass(int stadNum, QString newGrass)
//Modifies stadNum's grass type
{masterVect.at(stadNum).grass = newGrass;}

void Data::modStadType(int stadNum, QString newType)
//Modifies the type of stadium stadNum is
{masterVect.at(stadNum).type = newType;}

void Data::delStad(int stadNum)
//Deletes stadNum from the vector
{
    //WHAT HAPPENS IF A TEAM IS STILL IN THE STADIUM
//    masterVect.erase(masterVect.begin() + stadNum);
    //NEED TO CLEAR OUT DISTANCE ROW AND COLUMN IN THE MATRIX TOO FJSDKL:JROIWEJ:KDJFL:SKHGer
    //IF YOU WANT TO PROGRAM THIS, BRING IT UP IN THE NEXT SCRUM MEETING
    qDebug() << "Data::delStad is unimplemented" << stadNum;
    bool NOTIMPLEMENTED;
}

void Data::addTeam(int stadNum, QString newTeam, QString newLeague/*LOGOVARIABLE*/)
//Adds a team to stadNum
{
    TeamObj addingTeam;
    addingTeam.name = newTeam;
    addingTeam.stadNum = stadNum;
    addingTeam.league = newLeague;
    //maybe add a logo?
    masterVect.at(stadNum).team = addingTeam;
}

void Data::modTeam(int stadNum, QString newTeamName)
//Modifies name of team
{masterVect.at(stadNum).team.name = newTeamName;}

void Data::modLeague(int stadNum, QString newLeague)
//Modifies league of team
{masterVect.at(stadNum).team.league = newLeague;}

void Data::addSouv(int stadNum, QString newName, double newPrice)
//Adds a souvenir to a stadium
{
    SouvObj newSouv;
    newSouv.stadNum = stadNum;
    newSouv.name = newName;
    newSouv.price = newPrice;
    masterVect.at(stadNum).souvVect.push_back(newSouv);
}

void Data::modSouvName(int stadNum, int souvNum, QString newName)
{
// modifies a souvenir name
    masterVect.at(stadNum).souvVect.at(souvNum).name = newName;
}

void Data::modSouvPrice(int stadNum, int souvNum, double newPrice)
{
// modifies a souvenir price
    masterVect.at(stadNum).souvVect.at(souvNum).price = newPrice;
}

void Data::deleteSouv(int stadNum, int souvNum)
//Removes a souvenir object from souvVect
{
    //Erase element if possible
    if(souvNum <= this->getSouvListSize(stadNum))
    {
        masterVect.at(stadNum)
                .souvVect.erase(masterVect.at(stadNum)
                                .souvVect.begin()+souvNum);
    }
    else
    {
        qDebug() << "Error: Can't remove souvenir *** Out of Range ***";
    }

}

/**
 * @brief The based_on_second struct provides instructions for std::sort
 * to compare only the second integer of integer std::pairs
 *
 * It does this by overloading the () operator
 *
 * The only function that uses this is Data::addDist() since the adjacency
 * list needs to sort its respective std::list whenever a new distance
 * is added, such that the smallest distance is always at the front and
 * the longest is at the back.
 * @see Data::addDist()
 */
struct based_on_second
//Instructs list.sort() functions to sort using the cost
{
    bool operator()(const std::pair<int,int> &left,
                    const std::pair<int,int> &right)
    {return left.second < right.second;}
};

void Data::addDist(int x, int y, int newDist)
//Changes data in matrix and modifies adjacency list
//Complexity: O(nlogn)
{
    //Declare an iterator for adjList
    std::list< std::pair<int,int> >::iterator it = adjList.at(x).begin();

    //Fill in the data to there and back
    matrix[x][y] = newDist;
    matrix[y][x] = newDist;

    //See if y already exists in the list at x
    while ((*it).first != y && !adjList.at(x).empty()
           && it != adjList.at(x).end())
    {
        it++;
    }
    //If adding a new distance
    if (newDist != -1)
    {
        //If it is at the end, that means y isn't in the list, so push
        if (it == adjList.at(x).end())
        {
            adjList.at(x).push_back(std::make_pair(y,newDist));
        }
        //Otherwise, just modify the cost of y
        else
        {
            (*it).second = newDist;
        }
        //Now re-sort the list
        adjList.at(x).sort(based_on_second());
    }
    //Otherwise delete the distance from the list
    else if (!adjList.at(x).empty() && it != adjList.at(x).end())
    {
        adjList.at(x).erase(it);
    }
}

unsigned int Data::size() const
//Returns number of stadiums in the program
{return masterVect.size();}

void Data::importSQL()
//Fill data structures with information from SQL database
//Complexity: O(e^2) where e = number of edges
{
    //Determine path to APPDATA folder
    QString dir = QDir::currentPath();
    dir.resize(dir.lastIndexOf("/build"));
    dir += "/Baseball-Itinerary-Program/APPDATA/";

    //Access the database
    QFile ourDatabase(dir + "DATABASE.db");

    //Import data if the database is accessible
    if (ourDatabase.exists())
    {
        //Initialize database and its driver
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        //Line up database with APPDATA's database
        db.setDatabaseName(dir + "DATABASE.db");

        //Open up the database
        db.open();
        qDebug() << "Importing data from" << dir + "DATABASE.db";

        //Create a variable to execute SQL commands
        QSqlQuery query;

        //Import stadiums
        query.exec("SELECT name, address, phone, opened,"
                   "       capacity, grass, type "
                   "FROM stadiuminfo;");

        //Loop through all SQL table rows
        while (query.next())
        {
            //Add a stadium (one stadium per row of data)
            addStad(query.value(0).toString(),query.value(1).toString(),
                    query.value(2).toString(),query.value(3).toString(),
                    query.value(4).toInt(),query.value(5).toString(),
                    query.value(6).toString());
        }
        //Import teams
        query.exec("SELECT stadnum,name,league FROM teams");
        while (query.next())
        {
            addTeam(query.value(0).toInt(),query.value(1).toString(),
                    query.value(2).toString());
        }
        //Import souvenirs
        query.exec("SELECT stadnum,souvenir,price FROM souvenirs");
        while (query.next())
        {
            addSouv(query.value(0).toInt(),query.value(1).toString(),
                    query.value(2).toDouble());
        }
        //Import distances
        int y = 0;
        matrix.resize(masterVect.size());
        adjList.resize(masterVect.size());
        for (unsigned int x = 0; x < masterVect.size(); x++)
        {
            matrix[x].resize(masterVect.size());
        }
        query.exec("SELECT * FROM distances");
        while (query.next())
        {
            for (unsigned int x = 0; x < masterVect.size(); x++)
            {
                addDist(x,y,query.value(x).toInt());
            }
            y++;
        }
        //Close the database
        db.close();
        qDebug() << "Import successful";
    }
    else
    {
        qDebug() << "Data::importSQL(): Cannot access database";
    }
}

bool Data::exportSQL()
//Exports all data structures into the SQL database
{
    bool failure = false;

    //Determine path to APPDATA folder
    QString dir = QDir::currentPath();
    dir.resize(dir.lastIndexOf("/build"));
    dir += "/Baseball-Itinerary-Program/APPDATA/";

    //Access the database
    QFile ourDatabase(dir + "DATABASE.db");

    //Import data if the database is accessible
    if (ourDatabase.exists())
    {
        //Initialize database and its driver
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        //Line up database with APPDATA's database
        db.setDatabaseName(dir + "DATABASE.db");

        //Open up the database
        db.open();
        if (db.open())
        {
            qDebug() << "Exporting data to" << dir + "DATABASE.db";
        }
        else
        {
            qDebug() << "Data::exportSQL(): Cannot open database";
            failure = true;
        }

        //Create transaction for instant commit
        QSqlDatabase::database().transaction();

        //Create a variable to execute SQL commands
        QSqlQuery query;

        //Export stadium data
        qDebug() << "Writing into stadiuminfo table...";
        query.exec("DELETE FROM stadiuminfo");
        query.prepare("INSERT INTO stadiuminfo(stadnum,name,address,"
                      "                        phone,opened,capacity,"
                      "                        grass,type)"
                      "VALUES (:stadnum,:name,:address,:phone,"
                      "        :opened,:capacity,:grass,:type)");
        for (unsigned int x = 0; x < masterVect.size(); x++)
        {
            query.bindValue(":stadnum",x);
            query.bindValue(":name",getStadName(x));
            query.bindValue(":address",getStadAddress(x));
            query.bindValue(":phone",getStadPhone(x));
            query.bindValue(":opened",getStadOpened(x));
            query.bindValue(":capacity",getStadCapactiy(x));
            query.bindValue(":grass",getStadGrass(x));
            query.bindValue(":type",getStadType(x));
            if (!query.exec())
            {
                qDebug() << "SQL exporting stadiuminfo failed with "
                         << query.lastError();
                failure = true;
            }
        }
        qDebug() << "Complete";
        //Export team data
        qDebug() << "Writing into teams table...";
        query.exec("DELETE FROM teams");
        query.prepare("INSERT INTO teams(stadnum,name,league)"
                      "VALUES (:stadnum,:name,:league)");
        for (unsigned int x = 0; x < masterVect.size(); x++)
        {
            query.bindValue(":stadnum",x);
            query.bindValue(":name",getTeamName(x));
            query.bindValue(":league",getTeamLeague(x));
            if (!query.exec())
            {
                qDebug() << "SQL exporting team failed with "
                         << query.lastError();
                failure = true;
            }
        }
        qDebug() << "Complete";
        //Export souvenir data
        qDebug() << "Writing into souvenirs table...";
        query.exec("DELETE FROM souvenirs");
        query.prepare("INSERT INTO souvenirs(stadnum,souvenir,price)"
                      "VALUES (:stadnum,:souvenir,:price)");
        for (unsigned int x = 0; x < masterVect.size(); x++)
        {
            for (unsigned int s = 0;
                 s < masterVect.at(x).souvVect.size(); s++)
            {
                query.bindValue(":stadnum",x);
                query.bindValue(":souvenir",getSouvName(x,s));
                query.bindValue(":price",getSouvPrice(x,s));
                if (!query.exec())
                {
                    qDebug() << "SQL exporting souvenir failed with "
                             << query.lastError();
                    failure = true;
                }
            }
        }
        qDebug() << "Complete";
        //Export distance data
        qDebug() << "Writing into distances table...";
        query.exec("DROP TABLE distances");
        QString prepareCmd = "CREATE TABLE 'distances' (";
        for (unsigned int x = 0; x < masterVect.size(); x++)
        {
            prepareCmd += "'" + QString::number(x) + "' INTEGER,";
        }
        prepareCmd.chop(1);
        prepareCmd += ")";
        if (!query.exec(prepareCmd))
        {
            qDebug() << "SQL exporting failure. Cannot create table: "
                     << query.lastError();
        }
        for (unsigned int y = 0; y < masterVect.size(); y++)
        {
            prepareCmd = "INSERT INTO distances(";
            for (unsigned int x = 0; x < masterVect.size(); x++)
            {
                prepareCmd += "'" + QString::number(x) + "',";
            }
            prepareCmd.chop(1);
            prepareCmd += ") VALUES(";
            for (unsigned int x = 0; x < masterVect.size(); x++)
            {
                prepareCmd += "'" + QString::number(matrix[x][y]) + "',";
            }
            prepareCmd.chop(1);
            prepareCmd += ")";
            if (!query.exec(prepareCmd))
            {
                qDebug() << "SQL exporting distance matrix failed with "
                         << query.lastError();
                failure = true;
            }
        }
        //Instantly write all changes to the database
        QSqlDatabase::database().commit();

        qDebug() << "Complete";
        db.close();

        if (!failure)
        {
            qDebug() << "Export successful";
        }
        else
        {
            qDebug() << "Export unsuccessful";
        }
        return !failure;
    }
    else
    {
        qDebug() << "Data::exportSQL(): Cannot access database";
        return false;
    }
}

bool Data::importTXT(QString path)
//Import from a text file (Really just resets to defaults for devs)
{
    QFile file(path);       //QFile to read from
    QTextStream fin(&file); //Text stream for data reading
    QString str;            //Stores strings
    int stadNum;            //Stadium index number
    StadObj newStadObj;     //Stadium object
    TeamObj newTeamObj;     //Team object
    bool failure = true;    //Failure condition

    //Check to make sure that the file opens
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug("Data::importTXT(): File did not open");
    }
    else
    {
        failure = false;

        //Clear out data
        masterVect.clear();

        while(!fin.atEnd())
        {
            stadNum = fin.readLine().toInt();
            newStadObj.name = fin.readLine();
            newTeamObj.stadNum = stadNum;
            newTeamObj.name = fin.readLine();
            newTeamObj.league = fin.readLine();
            newStadObj.address = fin.readLine();
            newStadObj.address += " " + fin.readLine();
            newStadObj.phone = fin.readLine();
            fin.read(9);//skip "Opened - "
            newStadObj.opened =
                    QDate::fromString(fin.readLine(),"MMMM dd, yyyy");
            fin.read(11); //skip "Capacity - "
            str = fin.readLine();
            str.remove(QChar(','));
            newStadObj.capacity = str.toInt();
            newStadObj.grass = fin.readLine();
            newStadObj.type = fin.readLine();
            fin.readLine();
            masterVect.push_back(newStadObj);
            masterVect.at(stadNum).team = newTeamObj;
        }
    }
    file.flush();
    file.close();

    if (!failure)
    {
        //Import default souvenirs into each stadium
        for (unsigned int x = 0; x < masterVect.size(); x++)
        {
            addSouv(x,"Baseball Cap",23.99);
            addSouv(x,"Baseball Bat",45.39);
            addSouv(x,"Team Pennant",15.99);
            addSouv(x,"Autographed Baseball",19.99);
            addSouv(x,"Team Jersey",85.99);
        }
        //Add default distances into distance matrix
        matrix.resize(masterVect.size());
        for (unsigned int x = 0; x < masterVect.size(); x++)
        {
            matrix[x].resize(masterVect.size());
        }
        addDist(10,9,2070); addDist(10,11,1390);addDist(10,15,680);
        addDist(10,6,680);  addDist(15,0,6);    addDist(15,0,-1);
        addDist(15,6,0);    addDist(15,17,650); addDist(6,17,650);
        addDist(15,21,340); addDist(6,21,340);  addDist(15,0,340);
        addDist(6,0,340);   addDist(21,0,0);    addDist(21,11,1500);
        addDist(0,11,1500); addDist(21,26,110); addDist(0,26,110);
        addDist(26,17,300); addDist(26,20,830); addDist(1,9,210);
        addDist(1,8,90);    addDist(1,13,240);  addDist(1,29,240);
        addDist(2,23,1255); addDist(2,14,195);  addDist(2,8,195);
        addDist(2,9,430);   addDist(3,4,460);   addDist(3,28,740);
        addDist(3,5,230);   addDist(3,17,870);  addDist(3,20,650);
        addDist(4,29,415);  addDist(4,13,415);  addDist(4,16,235);
        addDist(4,3,460);   addDist(4,20,560);  addDist(5,16,680);
        addDist(5,12,790);  addDist(5,23,965);  addDist(5,17,1115);
        addDist(5,3,230);   addDist(7,19,90);   addDist(7,25,0);
        addDist(7,23,930);  addDist(7,28,560);  addDist(7,27,195);
        addDist(8,27,115);  addDist(8,22,225);  addDist(8,1,90);
        addDist(9,2,430);   addDist(9,27,225);  addDist(9,1,210);
        addDist(9,24,430);  addDist(9,10,2070); addDist(11,24,300);
        addDist(11,16,465); addDist(11,21,1500);addDist(11,0,1500);
        addDist(11,10,1390);addDist(12,22,790); addDist(12,23,210);
        addDist(12,5,790);  addDist(13,1,240);  addDist(13,22,250);
        addDist(13,4,415);  addDist(13,24,80);  addDist(13,29,0);
        addDist(14,2,195);  addDist(14,19,80);  addDist(14,27,315);
        addDist(14,18,0);   addDist(16,22,310); addDist(16,5,680);
        addDist(16,4,235);  addDist(16,11,465); addDist(17,20,580);
        addDist(17,3,870);  addDist(17,5,1115); addDist(17,26,300);
        addDist(17,6,650);  addDist(17,15,650); addDist(18,2,195);
        addDist(18,19,80);  addDist(18,27,315); addDist(18,14,0);
        addDist(19,14,80);  addDist(19,18,80);  addDist(19,7,90);
        addDist(19,25,90);  addDist(20,4,560);  addDist(20,3,650);
        addDist(20,17,580); addDist(20,26,830); addDist(22,8,225);
        addDist(22,27,260); addDist(22,12,790); addDist(22,28,375);
        addDist(22,16,310); addDist(22,13,250); addDist(22,19,250);
        addDist(23,7,930);  addDist(23,25,930); addDist(23,2,1255);
        addDist(23,5,965);  addDist(23,12,210); addDist(23,28,600);
        addDist(24,9,430);  addDist(24,29,80);  addDist(24,13,80);
        addDist(24,11,300); addDist(25,7,0);    addDist(25,19,90);
        addDist(25,23,930); addDist(25,28,560); addDist(25,27,195);
        addDist(27,9,225);  addDist(27,14,315); addDist(27,18,315);
        addDist(27,7,195);  addDist(27,25,195); addDist(27,22,260);
        addDist(27,8,115);  addDist(28,7,560);  addDist(28,25,560);
        addDist(28,23,600); addDist(28,3,740);  addDist(28,22,375);
        addDist(29,13,0);   addDist(29,24,80);  addDist(29,1,240);
        addDist(29,22,250); addDist(29,4,415);
    }
    return !failure;
}

deque<int> Data::askDijkstra(int startingVertex)
//Uses Dijkstra's algorithm to calculate paths to all other vertices
//Complexity: O(elogv) where e = number of edges, v = number of vertices
{
    vector<int> parentMap;      //Vector that maps discovery lineage
    deque<int> distMap;         //Distances from start to all other verts
    MinMeap meap(matrix.size());//Minimum heap to help algorithm execution
    int current;                //Current vertex
    list< pair<int,int> >::iterator it;

    //Initialize heap
    for (int x = 0; x < (int)matrix.size(); x++)
    {
        //Initialize starting vertex with 0 cost
        if (x == startingVertex)
        {
            meap.push(make_pair(x,0));
        }
        //Otherwise give all other vertices maximum cost
        else
        {
            meap.push(make_pair(x,INT_MAX));
        }
    }
    //Initialize distance map and  parent map
    distMap.resize(matrix.size());
    parentMap.resize(matrix.size());
    parentMap.at(startingVertex) = -1;  //Start has no parent

    //Algorithm execution until all vertex costs are calculated
    while (!meap.empty())
    {
        //Update current from the top of the heap
        current = meap.getMin().first;

        //Record its distance to the source as well
        distMap.at(current) = meap.getMin().second;

        //Pop current off the heap
        meap.popRoot();

        //Explore neighbors of current vertex
        it = adjList.at(current).begin();
        while (it != adjList.at(current).end())
        {
            //If the heap isn't empty, the data exists in the heap, and
            //  the the total cost from start to data is less than
            //  the cost heap has for that data
            if (!meap.empty() && meap.thisDataExists((*it).first)
                && (*it).second + distMap[current]
                < meap.mapQuery((*it).first).second)
            {
                //Update heap with new, better cost
                meap.reKey((*it).first,(*it).second + distMap[current]);

                //Parent of newly updated vertex is current
                parentMap.at((*it).first) = current;
            }
            it++;
        }
    }
    return distMap;
}

/**
 * @brief The probject struct is used specifically by Data::askPrim() to
 * aid in algorithm execution
 * @see Data::askPrim()
 */
struct probject
//Prim object for algorithm execution
{
    pair<int,int> edge;			//An edge between two vertices
    int cost;					//The cost of the edge
    bool discovered;			//Edge visitation boolean
};
int Data::askPrim(vector<pair<int, int> > &edges)
//Returns cost and edges of Prim's MST
{
    MinMeap meap(adjList.size());		//Minimum heap to help algorithm
    int current;                 		//Current vertex
    list<pair<int,int> >::iterator it;	//List iterator
    int totalCost = 0;                  //Total cost of tree
    vector<probject> mstVect;           //Vector of prim objects

    //Reset edges vector
    edges.clear();

    //Initialize heap and vector of prim objects
    mstVect.resize(adjList.size());
    for (int x = 0; x < (int)adjList.size(); x++)
    {
        //Initialize starting vertex with 0 cost
        if (x == 0)
        {
            meap.push(make_pair(x,0));
        }
        //Otherwise give all other vertices maximum cost
        else
        {
            meap.push(make_pair(x,INT_MAX));
        }
        mstVect.at(x).discovered = false;
    }
    //Algorithm execution until all vertex costs are calculated
    while (!meap.empty())
    {
        //Update current from the top of the heap
        current = meap.getMin().first;

        //Check which edge contributed to current
        if (mstVect.at(current).discovered == true)
        {
        	//Add the edge to the result
        	edges.push_back(mstVect.at(current).edge);

        	//Update total MST cost
        	totalCost += mstVect.at(current).cost;
        }

        //Pop current off the heap
        meap.popRoot();

        //Explore neighbors of current vertex
        it = adjList.at(current).begin();
        while (it != adjList.at(current).end())
        {
            //If the heap isn't empty, the data exists in the heap, and
            //  the the edge cost is less than the cost heap has
            if (!meap.empty() && meap.thisDataExists((*it).first)
                && (*it).second < meap.mapQuery((*it).first).second)
            {
                //Update heap with new, better cost
                meap.reKey((*it).first,(*it).second);

                //Update the edge in the prim vector
                mstVect.at((*it).first).discovered = true;
                mstVect.at((*it).first).edge
                        = make_pair(current,(*it).first);
                mstVect.at((*it).first).cost = (*it).second;
            }
            it++;
        }
    }
    return totalCost;
}

QString Data::getStadName(int stadNum) const
//Returns name of stadium at stadNum
{return masterVect.at(stadNum).name;}

QString Data::getStadAddress(int stadNum) const
//Returns address of stadium at stadNum
{return masterVect.at(stadNum).address;}

QString Data::getStadPhone(int stadNum) const
//Returns phone number of stadium at stadNum
{return masterVect.at(stadNum).phone;}

QString Data::getStadOpened(int stadNum) const
//Returns opening date of stadium at stadNum
{return masterVect.at(stadNum).opened.toString("dd MM yyyy");}

QString Data::getStadOpened(int stadNum,bool format) const
//Returns formatted opening date of stadium at stadNum
{
    if (format)
    {
        return masterVect.at(stadNum).opened.toString("dd MMMM yyyy");
    }
    else
    {
        return masterVect.at(stadNum).opened.toString("yyyyMMdd"); // used to compare dates

    }
    return "";
}

int Data::getStadCapactiy(int stadNum) const
//Returns seating capacity of stadium at stadNum
{return masterVect.at(stadNum).capacity;}

QString Data::getStadGrass(int stadNum) const
//Returns grass type of stadium at stadNum
{return masterVect.at(stadNum).grass;}

QString Data::getStadType(int stadNum) const
//Returns type of stadium at stadNum
{return masterVect.at(stadNum).type;}

QString Data::getTeamName(int stadNum) const
//Returns team name
{return masterVect.at(stadNum).team.name;}

QString Data::getTeamLeague(int stadNum) const
//Returns a team's league
{return masterVect.at(stadNum).team.league;}

int Data::getTeamStad() const
//Returns the stadNumber of the stadium the team belongs to
{
    int NOTIMPLEMENTED;
    return NOTIMPLEMENTED;
}

QString Data::getSouvName(int stadNum, int souvNum) const
{return masterVect.at(stadNum).souvVect.at(souvNum).name;}

double Data::getSouvPrice(int stadNum, int souvNum) const
{return masterVect.at(stadNum).souvVect.at(souvNum).price;}

int Data::getSouvListSize(int stadNum) const
{return masterVect.at(stadNum).souvVect.size();}

int Data::getDistBetween(unsigned int here, unsigned int there) const
//Returns distance between two stadiums
{return matrix[here][there];}
