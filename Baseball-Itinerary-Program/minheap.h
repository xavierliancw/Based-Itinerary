#ifndef MINHEAP_H
#define MINHEAP_H
#include <deque>
#include <string>
#include <utility>
#include <stdexcept>
#include <sstream>
using namespace std;

/*************************************************************************
 * MinMeap (Minimum Binary Heap with Map Parallel) (Int Specific)
 * ----------------------------------------------------------------------
 * This class implements a minimum binary heap using a deque.
 * There's another parallel vector that tracks the location of each key
 * in the heap so that key retrieval is O(1).
 * Because of this, this heap MUST be initialized with a size that cannot
 * change.
 * This MinMeap stores a heap of value-key pairs and will maintain heap
 * order by comparing the second variable in the pair variable.
 * ----------------------------------------------------------------------
 * PRE-REQUISITES
 * ----------------------------------------------------------------------
 * #include <deque>
 * #include <string>
 * #include <utility>
 * #include <stdexcept>
 * #include <sstream>
 * using namespace std;
 ************************************************************************/
class MinMeap
{
public:
    MinMeap(unsigned int meapInitSize);  //Constructor
    ~MinMeap(); //Destructor

    void push(pair<int,int> newData);           //Adds new data
    void popRoot();                             //Pops the root

    void clear();                               //Clears out heap
    void replace(pair<int,int> newData);        //Pops root, adds new
    void reKey(int thisData, int newKey);       //Key replacement

    pair<int,int> getMin() const;               //Returns data at root
    pair<int,int> at(unsigned int index) const; //Returns index data
    pair<int,int> mapQuery(int data) const;     //Returns data's pair
    bool thisDataExists(int data) const;        //Returns if heap has data
    int size() const;                           //Returns size of heap
    bool empty() const;                         //Returns if heap is empty

private:
    deque< pair<int,int> > heap;   //Deque to handle heap
    deque<int> meap;	//Heap map where key is index & data is location

    void bubbleUp(unsigned int index);   //Restore heap upward
    void bubbleDown();                   //Restore heap downward
    int parentOf(int index);             //Get parent index
    int lChildOf(int index);             //Get left child index
    int rChildOf(int index);             //Get right child index
};
#endif // MINHEAP_H
