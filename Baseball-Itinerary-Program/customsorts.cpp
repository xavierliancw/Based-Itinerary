#include "customsorts.h"

/*CustomSorts METHODS*/
CustomSorts::CustomSorts()
//Constructor
{}

CustomSorts::~CustomSorts()
//Destructor
{}

vector<SortObj> CustomSorts::InsertionSort(vector<SortObj> sortThese)
//Insertion sort
//Complexity: O(n^2)
{
     int j;         //LCV
     SortObj temp;  //Holds swapping variable

    //Loop until i reaches sortThese's size
    for (int i = 0; i < (int)sortThese.size(); i++)
    {
        j = i;

        //While j is greater than 0 and j is less that its postdecessor
        while (j > 0 && *sortThese.at(j) < *sortThese.at(j - 1))
        {
              temp = sortThese[j];              //Store for swap
              sortThese[j] = sortThese[j-1];    //Do the swap
              sortThese[j-1] = temp;            //Finish the swap
              j--;
        }
    }
    return sortThese;
}
