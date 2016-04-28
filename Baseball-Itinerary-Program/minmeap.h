/**
  @file
  @author Xavier
  @date 27 April 2016

  The MinMeap class declaration is implemented in this file.
  */
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
/**
 * @brief This class implements a combination data structure of a minumum
 * binary heap and a map.
 *
 * This class stores a series of integer pairs in a minimum binary heap.
 * When the heap accepts an integer pair, it uses the latter integer to
 * make comparisons, and will always keep the pair with the smallest
 * latter integer at the top of the heap. It also maps the former integer
 * of the pair to its index within the heap. This is possible because
 * the map uses a deque where the index locations are the former integers
 * themselves, and the corresponding data is an integer representing the
 * actual index location of the integer pair in the heap's deque.
 *
 * This class implements a combination of two data structures, a minimum
 * binary heap and a map. Both the heap and the map are implemented
 * through deques.
 *
 * Since this class maps an integer pair's first integer to a deque index
 * location within the heap, access to any data point within the heap
 * happens in constant time. This makes it easy to update any values
 * within the heap efficiently while also reaping all the benefits that
 * a minimum binary heap offers.
 *
 * This combination data structure does have limitations though. The
 * nature of the map can only accept a series of integer pairs that have
 * contiguous former integers that start at 0. None of those former
 * integers can be negative either since each former integer represents
 * a physical index location within the map's deque. This also means that
 * the entire data structure's size is not dynamic, and must be fixed.
 *
 * @par REQUIREMENTS:
 * #include <deque>     <br>
 * #include <string>    <br>
 * #include <utility>   <br>
 * #include <stdexcept> <br>
 * #include <sstream>   <br>
 * using namespace std;
 * @warning The first integer of all integer pairs that get pushed into
 * the MinMeap must be contiguous, start at 0, and cannot be negative.
 * @warning The size of MinMeap is fixed, and cannot be resized.
 */
class MinMeap
{
public:
    /**
     * @brief The default constructor
     * @param meapInitSize : The fixed size of the meap
     */
    MinMeap(unsigned int meapInitSize);         //Constructor
    ~MinMeap();                                 //Destructor

    void push(pair<int,int> newData);
    void popRoot();                             //Pops the root
    void clear();                               //Clears out heap
    void replace(pair<int,int> newData);        //Pops root, adds new
    /**
     * @brief Change the value of an integer pair's second integer within
     * the heap, then restores minimum heap order if necessary
     * @param thisData : The second integer of the integer pair that is
     * being modified
     * @param newKey : The new value for the integer pair's latter integer
     *
     * Complexity: O(logn)
     */
    void reKey(int thisData, int newKey);       //Key replacement

    pair<int,int> getMin() const;               //Returns data at root
    /**
     * @brief Get an integer pair from a specified index location of the
     * meap
     * @param index : Index location of interest
     * @return Returns the integer pair at the specified index location
     * of the heap deque
     */
    pair<int,int> at(unsigned int index) const; //Returns index data
    /**
     * @brief Get a specific integer pair from the meap querying the
     * meap's map
     * @param data : Query for the meap's map
     * @return Returns the integer pair whose first integer matches the
     * map query
     */
    pair<int,int> mapQuery(int data) const;     //Returns data's pair
    /**
     * @brief Determine a specific integer pair exists within the meap
     * @param data : Query for the meap's map to see if any data within
     * the meap has the query as its first integer
     * @return Returns true if the query exists, otherwise returns false
     */
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
