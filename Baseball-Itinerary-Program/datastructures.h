/**
  @file
  @author jerryberry
  @date 27 April 2016

  This file contains the class declarations for the TeamObj, SouvObj,
  StadObj, ItinObj, and Data classes.
  */
#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H
#include <vector>
#include <list>
#include <deque>
#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDate>

#include "minmeap.h"
using namespace std;

/**
 * @brief This class is the object representation of a team.
 *
 * This class represents a team by holding variables that relate to
 * baseball teams.
 * @par REQUIREMENTS:
 * #include <QString>   <br>
 * using namespace std;
 */
class TeamObj
{
public:
    /**
     * @brief The stadium this baseball team belongs to
     *
     * Each stadium is identified through its own unique stadNum.
     * @see Data
     */
    int stadNum;    //The stadium this team belongs to
    /**
     * @brief The name of the baseball team
     */
    QString name;   //The team's name
    /**
     * @brief The baseball league this baseball team belongs to
     */
    QString league; //The team's baseball league
    //PERHAPS A LOGO alkjfeiaj;fkejrlaj;lkja;lfkj;ejf;ajf;aiejf;lakjdf;lajeoilfj;
};
/*======================================================================*/
/**
 * @brief This class is the object representation of a souvenir.
 *
 * This class represents a souvenir by holding variables that relate to
 * souvenirs.
 * @par REQUIREMENTS:
 * #include <QString>   <br>
 * using namespace std;
 */
class SouvObj
{
public:
    /**
     * @brief The stadium this souvenir is sold at
     *
     * Each stadium is identified through its own unique stadNum.
     * @see Data
     */
    int stadNum;    //The stadium this souvenir belongs to
    /**
     * @brief The name of this souvenir
     */
    QString name;   //This souvenir's name
    /**
     * @brief The selling price of this souvenir
     */
    double price;   //This souvenir's selling price
};
/*======================================================================*/
/**
 * @brief This class is the object representation of a stadium.
 *
 * This class represents a stadium by holding variables that relate to
 * stadiums. A stadium has its own baseball team and its own souvenirs.
 * @par REQUIREMENTS:
 * class SouvObj        <br>
 * class TeamObj        <br>
 * #include <vector>    <br>
 * #include <QString>   <br>
 * #include <QDate>     <br>
 * using namespace std;
 * @see SouvObj
 * @see TeamObj
 */
class StadObj
{
public:
    /**
     * @brief The name of this stadium
     */
    QString name;              //This stadium's name
    /**
     * @brief The address of this stadium
     */
    QString address;           //This stadium's address
    /**
     * @brief The phone number of this stadium
     */
    QString phone;             //This stadium's phone number
    /**
     * @brief The date this stadium first opened
     */
    QDate opened;            //This stadium's opening date
    /**
     * @brief The seating capacity of this stadium
     */
    int capacity;              //The seating capacity of this stadium
    /**
     * @brief The type of turf this stadium uses for the baseball field
     */
    QString grass;             //The type of turf this stadium uses
    /**
     * @brief This stadium's type
     */
    QString type;              //This stadium's type
    /**
     * @brief This stadium's baseball team
     */
    TeamObj team;              //This stadium's team
    //PERHAPS A PIC OF THE FIELD d;alskfjeoijfalkajgl;aijerlkajmsdlkjiejr;alkjr(like the wiki page)
    /**
     * @brief This stadium's vector of souvenir objects
     */
    vector<SouvObj> souvVect;  //Vector of souvenirs
};
/*======================================================================*/
/**
 * @brief This class is the object representation of a queued stadium
 * within the itinerary.
 *
 * This class represents a queued stadium within the itinerary. It also
 * performs functions that will help a tourist plan a trip.
 *
 * For example, when a tourist wants to add souvenirs from a particular
 * stadium that's on the trip itinerary into the wishlist, this class
 * object will store the wanted souvenirs and their quantities.
 * @par REQUIREMENTS:
 * #include <vector>    <br>
 * using namespace std;
 */
class ItinObj
{
public:
    /**
     * @brief The default constructor
     * @param stadium : The stadium number this ItinObj represents
     *
     * Each stadium is identified through its own unique stadNum.
     * @see Data
     */
    ItinObj(int stadium);                   //Constructor
    /**
     * @brief Push a souvenir into the wishlist
     * @param souvNum : The index number that identifies the souvenir from
     * its StadObj's vector
     * @param qty : The quantity of a souvenir desired
     */
    void pushCart(int souvNum, int qty);    //Pushes a souv into the cart
    /**
     * @brief Remove a souvenir from the wishlist
     * @param souvNum : The index number that identifies the souvenir from
     * its StadObj's vector
     */
    void delCart(int souvNum);              //Removes a souv from the cart
    /**
     * @brief Change the quantity of a souvenir wanted within the wishlist
     * @param souvNum : The index number that identifies the souvenir from
     * its StadObj's vector
     * @param newQty : The new quantity of a souvenir wanted
     */
    void chgQty(int souvNum, int newQty);   //Changes qty of souv in cart

    /**
     * @brief Obtain the stadNum of the stadium represented by this
     * ItinObj
     * @return Returns the stadNum this ItinObj represents
     *
     * Each stadium is identified through its own unique stadNum.
     * @see Data
     */
    int getStadNum() const;                 //Returns stadNum
    /**
     * @brief Obtain the number of different kinds of souvenirs wanted
     * @return Returns the size of the wishlist
     */
    int getCartSize() const;                //Gets size of shopping cart
    /**
     * @brief Obtain the index number that identifies the souvenir from
     * its StadObj's vector that is at a location within the wishlist
     * @param index : Specifies which index location to look in within
     * the wishlist vector
     * @return  Returns the index number that identifies the souvenir from
     * its StadObj's vector from the specified index location
     */
    int getSouvNumAt(int index) const;      //Gets souvNum at index
    /**
     * @brief Obtain the quantity of a certain souvenir within the
     * wishlist
     * @param souvNum : The index number that identifies the souvenir from
     * its StadObj's vector
     * @return Returns the quantity of a specified souvenir from the
     * wishlist vector
     */
    int getQtyFor(int souvNum) const;       //Returns quantity of a souv

private:
    int stadNum;               //The stadium this object represents
    vector<int> itinSouvList;  //Vector of souvenirs wanted
    vector<int> itinSouvQuant; //Quantity of each souvenir wanted
};
/*======================================================================*/
/*************************************************************************
 * class: Data
 * ----------------------------------------------------------------------
 * This class is the primary interface between the GUI and all of the
 * data that pertains to stadiums, the distances between stadiums, teams,
 * and souvenirs.
 ************************************************************************/
/**
 * @brief This class is the primary interface between the GUI and all of
 * the data that pertains to stadiums, the distances between stadiums,
 * teams, and souvenirs.
 *
 * This class is an interface class that performs functions related to
 * stadium data. It handles all operations that relate to stadiums, the
 * distances between stadiums, teams, and souvenirs.
 *
 * All stadium data is stored in a vector of StadObj's. Each index
 * location of the vector holds an individual, unique stadium. As such,
 * each stadium can be identified by its index location. This entire
 * program refers to this identification number as a stadNum.
 *
 * This also means that all stadNums are contiguous, non-negative, and
 * begin at 0.
 *
 * Because this class actually holds representations of all the stadiums,
 * this class is used to add, modify, delete, or retrieve information that
 * pertains to stadiums.
 *
 * Additionally, this class handles an SQL interface that can save and
 * load all stadium data to and from, respectively, an SQL database file
 * that is stored locally.
 * @par REQUIREMENTS:
 * class StadObj             <br>
 * #include <vector>         <br>
 * #include < list>          <br>
 * #include <deque>          <br>
 * #include <utility>        <br>
 * #include <QDebug>         <br>
 * #include <QSqlDatabase>   <br>
 * #include <QSqlQuery>      <br>
 * #include <QSqlError>      <br>
 * #include <QDir>           <br>
 * #include <QFile>          <br>
 * #include <QTextStream>    <br>
 * using namespace std;
 * @warning This class does not have exception handling
 * @see StadObj
 */
class Data
{
public:
    Data();     //Constructor
    ~Data();    //Destructor

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
    void addTeam(int stadNum, QString newTeam, QString newLeague);
    void modTeam(int stadNum, QString newTeamName);
    void modLeague(int stadNum, QString newLeague);

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
    QString getStadOpened(int stadNum,bool format) const;
    int getStadCapactiy(int stadNum) const;
    QString getStadGrass(int stadNum) const;
    QString getStadType(int stadNum) const;

    //The following returns information about teams
    QString getTeamName(int stadNum) const;
    QString getTeamLeague(int stadNum) const;
    int getTeamStad() const;

    QString getSouvName(int stadNum, int souvNum) const;
    double getSouvPrice(int stadNum, int souvNum) const;
    int    getSouvListSize(int stadNum) const;

    //Returns distance between two stadiums
    int getDistBetween(unsigned int here, unsigned int there) const;

    //Returns the 2D matrix
    std::vector< std::vector<int> > getMatrix() const;

    //Returns adjacency list
    std::vector< std::list< std::pair<int,int> > > getAdjList() const;

    int getMST() const;

    //Fill data structures with information from SQL database
    void importSQL();

    //Export contents of all databases to SQL database
    bool exportSQL();

    //Import from a text file (Really just resets to defaults for devs)
    bool importTXT(QString path);

    deque<int> askDijkstra(int startingVertex);

    int askPrim(vector<pair<int,int> > &edges);

private:
    std::vector<StadObj> masterVect;        //Vector of all stadiums
    std::vector< std::vector<int> > matrix; //2D matrix of all distances

    //Adjacency list representation of the 2D matrix
    std::vector< std::list< std::pair<int,int> > > adjList;
};

#endif // DATASTRUCTURES_H
