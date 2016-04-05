#include "datastructures.h"

Data::Data()
//Constructor
{
    importSQL();
}

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
    newStad.opened = opened;
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
{masterVect.at(stadNum).opened = newOpened;}

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
    masterVect.at(stadNum).teamVect.push_back(addingTeam);
}

void Data::modTeam(int stadNum, QString oldTeamName, QString newTeamName)
//Changes name of a team
{
    //Search for team at the stadium (data set is naturally miniscule)
    for (unsigned int x = 0; x < masterVect.at(stadNum).teamVect.size();
         x++)
    {
        //If found, rename the team
        if (oldTeamName == masterVect.at(stadNum).teamVect.at(x).name)
        {
            masterVect.at(stadNum).teamVect.at(x).name = newTeamName;
        }
    }
}

void Data::modTeam(int stadNum, QString whichTeam, int newStadNum)
//Changes home stadium of the team at stadNum
{
    TeamObj movingTeam; //Variable to hold the moving team

    //Search for team at the stadium (data set is naturally miniscule)
    for (unsigned int x = 0; x < masterVect.at(stadNum).teamVect.size();
         x++)
    {
        //If found, remove the team
        if (whichTeam == masterVect.at(stadNum).teamVect.at(x).name)
        {
            movingTeam = masterVect.at(stadNum).teamVect.at(x);
            masterVect.at(stadNum)
                    .teamVect.erase(masterVect.at(stadNum)
                                    .teamVect.begin() + x);
        }
    }
    movingTeam.stadNum = newStadNum;
    masterVect.at(newStadNum).teamVect.push_back(movingTeam);
}

void Data::delTeam(int stadNum, QString whichTeam)
//Deletes a team
{
    //Search for team at the stadium (data set is naturally miniscule)
    for (unsigned int x = 0; x < masterVect.at(stadNum).teamVect.size();
         x++)
    {
        //If found, remove the team
        if (whichTeam == masterVect.at(stadNum).teamVect.at(x).name)
        {
            masterVect.at(stadNum)
                    .teamVect.erase(masterVect.at(stadNum)
                                    .teamVect.begin() + x);
        }
    }
}

void Data::addSouv(int stadNum, QString newName, double newPrice)
//Adds a souvenir to a stadium
{
    SouvObj newSouv;
    newSouv.stadNum = stadNum;
    newSouv.name = newName;
    newSouv.price = newPrice;

    masterVect.at(stadNum).souvVect.push_back(newSouv);
}

void Data::addDist(int x, int y, int newDist)
//Adds a new distance into both sides of the matrix
{
    //Fill in the data to there and back
    matrix[x][y] = newDist;
    matrix[y][x] = newDist;
}

unsigned int Data::size() const
//Returns number of teams in the program
//Complexity: O(t) where t = number of teams
{return masterVect.size();
    int UNFINISHED;
//    int teamCount = 0;  //Tracks how many teams are in the program

//    //Loop through every stadium
//    for (unsigned int x = 0; x < masterVect.size(); x++)
//    {
//        teamCount += masterVect.at(x).teamVect.size();
//    }
//    return teamCount;
}

void Data::importSQL()
//Fill data structures with information from SQL database
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
            for (unsigned int t = 0;
                 t < masterVect.at(x).teamVect.size(); t++)
            {
                query.bindValue(":stadnum",x);
                query.bindValue(":name",getTeamName(x,t));
                query.bindValue(":league",getTeamLeague(x,t));
                if (!query.exec())
                {
                    qDebug() << "SQL exporting team failed with "
                             << query.lastError();
                    failure = true;
                }
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
{return masterVect.at(stadNum).opened;}

int Data::getStadCapactiy(int stadNum) const
//Returns seating capacity of stadium at stadNum
{return masterVect.at(stadNum).capacity;}

QString Data::getStadGrass(int stadNum) const
//Returns grass type of stadium at stadNum
{return masterVect.at(stadNum).grass;}

QString Data::getStadType(int stadNum) const
//Returns type of stadium at stadNum
{return masterVect.at(stadNum).type;}

unsigned int Data::teamSize(int stadNum) const
//Returns number of teams at a stadium
{return masterVect.at(stadNum).teamVect.size();}

QString Data::getTeamName(int stadNum, int teamIndex) const
{return masterVect.at(stadNum).teamVect.at(teamIndex).name;}

QString Data::getTeamLeague(int stadNum, int teamIndex) const
{return masterVect.at(stadNum).teamVect.at(teamIndex).league;}

int Data::getTeamStad() const
{
    int NOTIMPLEMENTED;
    return NOTIMPLEMENTED;
}

QString Data::getSouvName(int stadNum, int souvNum) const
{return masterVect.at(stadNum).souvVect.at(souvNum).name;}

double Data::getSouvPrice(int stadNum, int souvNum) const
{return masterVect.at(stadNum).souvVect.at(souvNum).price;}
