/**
  @file
  @author jerryberry
  @date 27 April 2016

  @brief This file contains the class declarations for the TeamObj,
  SouvObj, StadObj, ItinObj, and Data classes.
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
     */
    ItinObj();
    /**
     * @brief Overloaded constructor
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
 * @note All stadium data is stored in a vector of StadObj's. Each index
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
 * #include "minmeap.h"      <br>
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
    /**
     * @brief Add a new stadium to Data's vector of stadiums
     * @param name : New stadium's name
     * @param address : New stadium's address
     * @param phone : New stadium's phone number
     * @param opened : New stadium's inception date
     * @param capacity : New stadium's capacity
     * @param grass : The kind of turf the new stadium uses on its
     * baseball field
     * @param type : The type of the new stadium
     */
    void addStad(QString name, QString address,
                 QString phone, QString opened, int capacity,
                 QString grass, QString type);
    //Change a stadium's name
    /**
     * @brief Modify a stadium's name
     * @param stadNum : Index location of the stadium of interest
     * @param newName : Stadium's new name
     */
    void modStadName(int stadNum, QString newName);
    //Change a stadium's address
    /**
     * @brief Modify a stadium's address
     * @param stadNum : Index location of the stadium of interest
     * @param newAddress : Stadium's new address
     */
    void modStadAddress(int stadNum, QString newAddress);
    //Change a stadium's phone number
    /**
     * @brief Modify a stadium's phone number
     * @param stadNum : Index location of the stadium of interest
     * @param newPhone : Stadium's new phone number
     */
    void modStadPhone(int stadNum, QString newPhone);
    //Change the date a stadium first opened
    /**
     * @brief Modify a stadium's opening date
     * @param stadNum : Index location of the stadium of interest
     * @param newOpened : Stadium's new opening date
     */
    void modStadOpened(int stadNum, QString newOpened);
    //Change the seating capacity a stadium has
    /**
     * @brief Modify a stadium's seating capacity
     * @param stadNum : Index location of the stadium of interest
     * @param newCapacity : Stadium's new seating capacity
     */
    void modStadCapacity(int stadNum, int newCapacity);
    //Change the kind of turf a stadium uses
    /**
     * @brief Modify the kind of turf a stadium uses
     * @param stadNum : Index location of the stadium of interest
     * @param newGrass : Stadium's new turf
     */
    void modStadGrass(int stadNum, QString newGrass);
    //Change the type of a stadium
    /**
     * @brief Modify a stadium's typology
     * @param stadNum : Index location of the stadium of interest
     * @param newType : Stadium's new typology
     */
    void modStadType(int stadNum, QString newType);
    //Delete a stadium
    /**
     * @brief Delete a stadium
     * @param stadNum : Index location of the stadium of interest
     */
    void delStad(int stadNum);

    //Modifies data pertaining to teams
    /**
     * @brief Modify everything about a stadium's team
     * @param stadNum : Index location of the stadium of interest
     * @param newTeam : New name of the baseball team
     * @param newLeague : The new baseball team's baseball league
     */
    void addTeam(int stadNum, QString newTeam, QString newLeague);
    /**
     * @brief Modify just the name of a team at a stadium
     * @param stadNum : Index location of the stadium of interest
     * @param newTeamName : New name of the baseball team
     */
    void modTeam(int stadNum, QString newTeamName);
    /**
     * @brief Modify just the league of a particular baseball team at a
     * particular stadium
     * @param stadNum : Index location of the stadium of interest
     * @param newLeague : The new baseball team's new baseball league
     */
    void modLeague(int stadNum, QString newLeague);

    //Modifies data pertaining to souvenirs
    /**
     * @brief Add a new souvenir to a stadium
     * @param stadNum : Index location of the stadium of interest
     * @param newName : Name of the new souvenir
     * @param newPrice : Price of the new souvenir
     */
    void addSouv(int stadNum, QString newName, double newPrice);
    /**
     * @brief Modify the name of a particular souvenir
     */
    void modSouvName(int stadNum, int souvNum, QString newName);
    /**
     * @brief Modify the price of a particular souvenir
     */
    void modSouvPrice(int stadNum, int souvNum, double newPrice);
    /**
     * @brief Delete a particular souvenir
     * @param stadNum : Index location of the stadium of interest
     * @param souvNum : Index location of the souvenir in question
     */
    void deleteSouv(int stadNum, int souvNum);

    //Modifies a distance in the matrix and adjacency list
    /**
     * @brief Modify the distance between two stadiums
     * @param x : The first stadium of interest's stadNum
     * @param y : The second stadium of interest's stadNum
     * @param newDist : New distance between x and y
     *
     * This function updates both the two dimensional matrix and the
     * adjacency list that represent the graph of stadiums.
     *
     * The distance that is updated in the matrix is updated twice. Once
     * in the appropriate upper diagonal, and once in the corresponding
     * location of the lower diagonal.
     *
     * The adjacency list is updated as necessary and always maintains
     * sorted neighbors of a stadium, sorted by distance where the
     * shortest is closest to the top.
     *
     * Complexity: O(nlogn)
     * @note A newDist of -1 indicates that there is no connection between
     * x and y.
     * @warning newDist cannot exceed 2147483647
     */
    void addDist(int x, int y, int newDist);

    //Returns number of stadiums in the program
    /**
     * @brief Obtain the total number of stadiums
     * @return Returns the size of the stadium vector, masterVect
     */
    unsigned int size() const;

    //Return various data points pertaining to stadiums
    QString getStadName(int stadNum) const;
    QString getStadAddress(int stadNum) const;
    QString getStadPhone(int stadNum) const;
    /**
     * @brief Obtain the date a stadium first opened
     * @param stadNum : Index location of the stadium of interest
     * @return Returns a QString of only numbers, formatted dd mm yyyy
     *
     * An example return value looks like "27 04 2016".
     */
    QString getStadOpened(int stadNum) const;
    /**
     * @brief Obtain the date a stadium first opened that is formatted
     * @param stadNum : Index location of the stadium of interest
     * @param format : If true, this function will return something
     * @return Returns a QString of the date, formatted dd MMMM yyyy
     * where MMMM is the month spelled out if format is true / Returns
     * nothing if format is false
     *
     * An example return value looks like "27 April 2016" if format is
     * true.
     */
    QString getStadOpened(int stadNum,bool format) const;
    int getStadCapactiy(int stadNum) const;
    QString getStadGrass(int stadNum) const;
    QString getStadType(int stadNum) const;

    //Return various data points pertaining to teams
    QString getTeamName(int stadNum) const;
    QString getTeamLeague(int stadNum) const;
    int getTeamStad() const;

    //Return various data points pertaining to souvenirs
    QString getSouvName(int stadNum, int souvNum) const;
    double getSouvPrice(int stadNum, int souvNum) const;
    /**
     * @brief Obtain the number of different souvenirs a particular
     * stadium has
     * @param stadNum : Index location of the stadium of interest
     * @return Returns the size of the souvenir vector of the specified
     * StadObj
     */
    int getSouvListSize(int stadNum) const;

    //Returns distance between two stadiums
    int getDistBetween(unsigned int here, unsigned int there) const;

    //Fill data structures with information from SQL database
    /**
     * @brief Import all stadium data from the SQL database file,
     * DATABASE.db
     *
     * Complexity: O(n^2)
     */
    void importSQL();

    //Export contents of all databases to SQL database
    /**
     * @brief Export all stadium data to the SQL database file,
     * DATABASE.db
     * @return Returns true if successful / Returns false if not
     *
     * Any changes made to data within the Data class will be written
     * into the database file.
     *
     * Complexity: O(n^2)
     * @note All of the information is committed in one SQL transaction.
     * @warning DATABASE.db will be corrupted if this function is
     * interrupted while writing to the file
     */
    bool exportSQL();

    //Import from a text file (Really just resets to defaults for devs)
    /**
     * @brief Import stadium information from a correctly formatted text
     * file
     * @param path : File path that includes the text file's name
     * @return Returns true if succesful / Returns false if not
     *
     * This will overwrite all data in the Data class with the data from
     * the text file.
     *
     * Complexity: O(n)
     * @note The distances between stadiums should not be in the text
     * file. This function will just reset them back to the default
     * values as of 05 April 2016
     * @warning If an incorrectly formatted text file is used, unintended
     * behavior may result
     */
    bool importTXT(QString path);

    //Performs a Dijkstra operation on a specified stadium
    /**
     * @brief Perform a Dijkstra operation on the adjacency list from
     * a specified stadium to get distances to all other stadiums from the
     * specified starting stadium
     * @param startingVertex : StadNum of the stadium that is the start
     * point
     * @return Returns a deque of integers that are distances, in miles,
     * from the starting point to all other stadiums where the each index
     * of the deque represents its respective stadium by stadNum
     *
     * Complexity: O(elogv), e = number of stadium connections, v = number
     * of stadiums
     */
    deque<int> askDijkstra(int startingVertex);

    //Performs a Prim operation on the entire graph of stadiums
    /**
     * @brief Perform a Prim operation on the adjacency list of stadiums
     * to get a minimum spanning tree of the stadium graph
     * @param edges : askPrim() builds this vector with the
     * branches(edges) of the minimum spanning tree
     * @return Returns the total mileage of the minimum spanning tree and
     * returns a vector of edges by reference
     *
     * Complexity: O(elogv), e = number of stadium connections, v = number
     * of stadiums
     * @note This method always starts at stadNum 0
     * @note An edge is represented by an std::pair of stadNum integers
     * @note askPrim() will clear edges before beginning
     */
    int askPrim(vector<pair<int,int> > &edges);

private:
    vector<StadObj> masterVect;             //Vector of all stadiums
    vector<vector<int> > matrix;            //2D matrix of all distances
    vector<list<pair<int,int> > > adjList;  //Adjacency list of distances
};
#endif // DATASTRUCTURES_H
