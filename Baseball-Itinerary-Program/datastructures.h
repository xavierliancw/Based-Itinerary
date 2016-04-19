#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QDebug>
#include <vector>
#include <list>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>

#include <QFile>
#include <QTextStream>

class TeamObj
{
public:
    int stadNum;
    QString name;
    QString league;
    //PERHAPS A LOGO alkjfeiaj;fkejrlaj;lkja;lfkj;ejf;ajf;aiejf;lakjdf;lajeoilfj;
};
/*======================================================================*/
class SouvObj
{
public:
    int stadNum;
    QString name;
    double price;
};
/*======================================================================*/
class StadObj
{
public:
    QString name;
    QString address;
    QString phone;
    QString opened;
    int capacity;
    QString grass;
    QString type;
    //PERHAPS A PIC OF THE FIELD d;alskfjeoijfalkajgl;aijerlkajmsdlkjiejr;alkjr(like the wiki page)

    std::vector<SouvObj> souvVect;
    std::vector<TeamObj> teamVect;
};
/*======================================================================*/
class ItinObj
{
public:
    ItinObj(int stadium);
    void pushCart(int souvNum, int qty);//Pushes a souv into the cart
    void delCart(int souvNum);          //Removes a souv from the cart
    void chgQty(int souvNum, int newQty); //Changes qty of souv in cart

    int getStadNum() const;             //Returns stadNum
    int getCartSize() const;            //Gets size of shopping cart
    int getSouvNumAt(int index) const;  //Gets souvNum at index
    int getQtyFor(int souvNum) const;   //Returns quantity of a souv

private:
    int stadNum;                    //current stadium

    std::vector<int> itinSouvList;  //list of souviners bought
    std::vector<int> itinSouvQuant; //quantity of each souviner
};
/*======================================================================*/
/*************************************************************************
 * class: Data
 * ----------------------------------------------------------------------
 * This class is the middle man between the GUI and all of the baseball
 * data.
 * **********************************************************************/
class Data
{
public:
    //Constructs the structure and imports data from the SQL database
    Data();
    //Default destructor
    ~Data();

    //Copy master vector to prevent altering master
    void copyVector(std::vector<StadObj>& copy);

    //Add a stadium object to the vector
    void addStad(QString name, QString address,
                 QString phone, QString opened, int capacity,
                 QString grass, QString type);
    void modStadName(int stadNum, QString newName);
    void modStadAddress(int stadNum, QString newAddress);
    void modStadPhone(int stadNum, QString newPhone);
    void modStadOpened(int stadNum, QString newOpened);
    void modStadCapacity(int stadNum, int newCapacity);
    void modStadGrass(int stadNum, QString newGrass);
    void modStadType(int stadNum, QString newType);
    void delStad(int stadNum);

    //Modifies data pertaining to teams
    void addTeam(int stadNum, QString newTeam, QString newLeague);//Legacy implementation
    void modTeam(int stadNum, QString newTeamName);//Legacy implementation
    void modLeague(int stadNum, QString newLeague);//Legacy implementation

    //Legacy methods
//    void modTeam(int stadNum, QString oldTeamName, QString newTeamName);
//    void modTeam(int stadNum, QString whichTeam, int newStadNum);
//    void delTeam(int stadNum, QString whichTeam);

    void addSouv(int stadNum, QString newName, double newPrice);
    void deleteSouv(int stadNum, int souvNum);
    void modSouvName();
    void modSouvPrice();

    //Changes a value in the matrix and its symmetrical counterpart
    void addDist(int x, int y, int newDist);

    //Returns number of stadiums in the program
    unsigned int size() const;
    QString getStadName(int stadNum) const;
    QString getStadAddress(int stadNum) const;
    QString getStadPhone(int stadNum) const;
    QString getStadOpened(int stadNum) const;
    int getStadCapactiy(int stadNum) const;
    QString getStadGrass(int stadNum) const;
    QString getStadType(int stadNum) const;

    //The following returns information about teams
    unsigned int teamSize(int stadNum) const;//Vital legacy
    QString getTeamName(int stadNum) const;//Legacy implementation
    QString getTeamLeague(int stadNum) const;//Legacy implementation
    QString getTeamName(int stadNum, int teamIndex) const;//Legacy
    QString getTeamLeague(int stadNum, int teamIndex) const;//Legacy
    int getTeamStad() const;

    QString getSouvName(int stadNum, int souvNum) const;
    double getSouvPrice(int stadNum, int souvNum) const;
    int    getSouvListSize(int stadNum) const;

    //Returns distance between two stadiums
    int getDistBetween(unsigned int here, unsigned int there) const;

    //Returns the 2D matrix
    std::vector< std::vector<int> > getMatrix() const;
    //Blarmos
    void initializeStuff();

    //Returns adjacency list
    std::vector< std::list< std::pair<int,int> > > getAdjList() const;

    int getMST() const;

    //Fill data structures with information from SQL database
    void importSQL();

    //Export contents of all databases to SQL database
    bool exportSQL();

    //Import from a text file (Really just resets to defaults for devs)
    bool importTXT(QString path);

private:
    std::vector<StadObj> masterVect;        //Vector of all stadiums
    std::vector< std::vector<int> > matrix; //2D matrix of all distances

    //Adjacency list representation of the 2D matrix
    std::vector< std::list< std::pair<int,int> > > adjList;


};

#endif // DATASTRUCTURES_H
