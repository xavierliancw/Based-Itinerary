#ifndef CUSTOMSORTS_H
#define CUSTOMSORTS_H
#include <vector>
#include <QString>
using namespace std;

/*************************************************************************
 * CustomSorts
 * ----------------------------------------------------------------------
 * This class stores all methods implemented by developers used for
 * sorting data.
 * The SortObj class is a helper class that will associate a stadNum
 * with whatever data that needs to be sorted (for now it's just QString
 * but maybe someone could make it a template *hint *HINT) ¯\_(ツ)_/¯
 * ----------------------------------------------------------------------
 * PRE-REQUISITES
 * ----------------------------------------------------------------------
 * #include <vector>
 * #include <QString>
 * using namespace std;
 ************************************************************************/
class SortObj   //Objects to sort that also store an associated stadNum
{
public:
    int stadNum;
    QString sortee;

    //Dereference to get the value to be sorted
    QString operator *() const {return sortee;}
    //Pointer operator to get stadNum
    int operator &() const {return stadNum;}
};
class CustomSorts
{
public:
    CustomSorts();
    ~CustomSorts();

    //Insertion sort
    vector<SortObj> InsertionSort(vector<SortObj> sortThese);
};
#endif // CUSTOMSORTS_H
