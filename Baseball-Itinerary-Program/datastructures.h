#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QDebug>
#include <vector>
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
    int NOTIMPLEMENTED()
    {int unimplemented;int NOTIMPLEMENTED;return unimplemented;}
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

    void addTeam(int stadNum, QString newTeam, QString newLeague);
    void modTeam(int stadNum, QString oldTeamName, QString newTeamName);
    void modTeam(int stadNum, QString whichTeam, int newStadNum);
    void delTeam(int stadNum, QString whichTeam);

    void addSouv(int stadNum, QString newName, double newPrice);
    void modSouvName();
    void modSouvPrice();

    //Adds a new distance into both sides of the distance matrix
    void addDist(int x, int y, int newDist);
    void modDist();
    void delDist();

    int size() const;
    QString getStadName(int stadNum) const;
    QString getStadAddress(int stadNum) const;
    QString getStadPhone(int stadNum) const;
    QString getStadOpened(int stadNum) const;
    int getStadCapactiy(int stadNum) const;
    QString getStadGrass(int stadNum) const;
    QString getStadType(int stadNum) const;

    QString getTeamName(int stadNum, int teamIndex) const;
    QString getTeamLeague(int stadNum, int teamIndex) const;
    int getTeamStad() const;

    QString getSouvName(int stadNum, int souvNum) const;
    double getSouvPrice(int stadNum, int souvNum) const;

    double getDistBetween() const;

    int getMST() const;

    //Export contents of all databases to SQL database
    void exportSQL();

private:
    std::vector<StadObj> masterVect;        //Vector of all stadiums
    std::vector< std::vector<int> > matrix; //2D matrix of all distances

    //Fill data structures with information from SQL database
    void importSQL();
};

#endif // DATASTRUCTURES_H
