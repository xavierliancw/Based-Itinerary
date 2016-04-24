#ifndef CUSTOMSORTS_H
#define CUSTOMSORTS_H
#include <vector>
#include <utility>
#include <QString>
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
class CustomSorts
{
public:
    CustomSorts();
    ~CustomSorts();

    //Insertion sort
    vector<pair<int,QString> > InsertionSort(vector<pair<int,QString> >
                                             sortThese);
};
#endif // CUSTOMSORTS_H
