/**
  @file
  @author Stephen
  @date 28 April 2016
  @brief This file contains the CustomSorts class declaration.
  */
#ifndef CUSTOMSORTS_H
#define CUSTOMSORTS_H
#include <vector>
#include <utility>
#include <QString>
#include <QDate>
using namespace std;

/*************************************************************************
 * CustomSorts
 * ----------------------------------------------------------------------
 * This class stores all methods implemented by developers used for
 * sorting data.
 * Sorts rely on a std::pair where it sorts based on the second variable.
 * ----------------------------------------------------------------------
 * PRE-REQUISITES
 * ----------------------------------------------------------------------
 * #include <vector>
 * #include <utility>
 * #include <QString>
 * using namespace std;
 ************************************************************************/
/**
 * @brief This class contains all jerryberry developer-implemented
 * sorting functions.
 * @par REQUIREMENTS:
 * #include <vector>    <br>
 * #include <utility>   <br>
 * #include <QString>   <br>
 * using namespace std;
 */
class CustomSorts
{
public:
    CustomSorts();
    ~CustomSorts();

    //Insertion sort
    /**
     * @brief Using an insertion sort, alphabetize a vector of
     * (integer/QString) pairs.
     * @param sortThese : Vector of data to be sorted
     * @return Returns a sorted version of sortThese
     *
     * This sort sorts based on comparing the QStrings of the
     * (integer/QString) pairs. It basically alphabetizes the vector.
     *
     * The integer of the (integer/QString) pair represents a stadNum.
     *
     * Complexity: O(n^2)
     * @see Data for more information about stadNums
     */
    vector<pair<int,QString> > InsertionSort(vector<pair<int,QString> >
                                             sortThese);
};
#endif // CUSTOMSORTS_H
