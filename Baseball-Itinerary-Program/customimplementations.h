#ifndef CUSTOMIMPLEMENTATIONS_H
#define CUSTOMIMPLEMENTATIONS_H
#include <deque>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <cstddef>
using namespace std;

/*************************************************************************
 * MinHeap (custom)
 * ----------------------------------------------------------------------
 * This class implements a minimum heap data structure through use of a
 * deque.
 * This is a custom implementation with the following modifications:
 *
 *  - Minimum comparison is based on the dereference operator (*). That
 *    is, when comparing some <data> against some other <data>, this heap
 *    will compare the dereferenced values against each other.
 *
 *      - e.g.: data first;    //First variable being compared
 *              data second;   //Second variable being compared
 *
 *              //This heap will compare like this
 *              if (*first < *second) {...
 *
 *    And since this implementation only uses one template, IT IS
 *    RECOMMENDED to initialize this custom heap with a custom object
 *    that has overloaded its * operator to obtain the intended values.
 *
 *  - There is an overloaded replace function that additionally accepts
 *    an index as a parameter. This allows for replacement of specific
 *    data within the heap instead of the traditional root pop then new
 *    data push.
 *
 *  - That overloaded replace function, root popping, and both heap
 *    restoring functions all return a deque that contains a list of
 *    steps the heap took to reorder itself. For example, if this heap
 *    swapped the contents of index 0 with the contents of index 2, then
 *    swapped the contents index 2 with the contents of index 5
 *    (0<->2,2<->5), the returning deque would look like:
 *    [<data> from 2], [<data> from 0], [<data> from 5], [<data> from 2]
 *
 *  - The function that restores the heap property upwards can start
 *    at any given index
 * ----------------------------------------------------------------------
 * PRE-REQUISITES
 * ----------------------------------------------------------------------
 * #include <deque>
 * #include <stdexcept>
 * #include <string>
 * #include <sstream>
 * using namespace std;
 ************************************************************************/
template<class data>
class MinHeap
{
public:
    MinHeap();  //Constructor
    ~MinHeap(); //Destructor

    void push(data addThis);    //Adds new data
    deque<data> popRoot();      //Pops the root

    void clear();                                   //Clears out heap
    void replace(data withThis);                    //Pops root, adds new
    deque<data> replace(int index, data withThis);  //Specific replacement
    void heapify(data heapThis);                    //Unimplemented
    void merge(MinHeap a, MinHeap b);               //Unimplemented
    void meld(MinHeap a, MinHeap b);                //Unimplemented

    data getMin() const;                //Returns data at root
    data at(unsigned int index) const;  //Returns data at heap index
    int size() const;                   //Returns size of heap
    bool empty() const;                 //Returns if heap is empty

private:
    deque<data> heap;   //Deque to handle heap
    deque<data> swaps;  //Queue of swaps that occured

    deque<data> bubbleUp(unsigned int index);   //Restore heap upward
    deque<data> bubbleDown();                   //Restore heap downward
    int parentOf(int index);                    //Get parent index
    int lChildOf(int index);                    //Get left child index
    int rChildOf(int index);                    //Get right child index
};
/*************************************************************************
 * Dijkstra
 * ----------------------------------------------------------------------
 * This is an implementation of Dijkstra's algorithm.
 * This object must be constructed with a 2D matrix implemented through
 * a nested vector of ints.
 * This implementation requires a custom minimum heap that can return
 * the swaps it makes to regain heap order, and can also compare two
 * values using an overloaded * operator.
 * Through use of the custom minimum heap, this implementation is able
 * to run Dijkstra's algorithm with a complexity of O(ElogV) where E
 * is the number of edges in the matrix, and V is the number of vertices
 * in the matrix.
 * ----------------------------------------------------------------------
 * PRE-REQUISITES
 *
 * - A 2D adjacency matrix that uses a nested vector of ints
 * - A custom minumum heap
 * #include <vector>
 * #include <cstddef>
 * using namespace std;
 * ----------------------------------------------------------------------
 ************************************************************************/
class Dijkstra
{
public:
    Dijkstra(vector<vector<int> > const &inMatrix);  //Constructor
    ~Dijkstra();                                      //Destructor

    //Returns map of vertices and their distances from start
    std::vector<int> getDistanceMap(int start);

    //Returns map of parents to show paths to take
    std::vector<int> getVertexPath() const;

private:
    //Djikstra object
    class Djobject
    {
    public:
        int vertex;
        int cost;

        //Overloaded operators
        int operator *() const {return cost;}
        int operator &() const {return vertex;}
    };
    vector< vector<int> > matrix; //The matrix of the graph
    deque<Djobject> swaps;                  //Deque of heap swaps
    vector<int> vertexPath;                 //Parent map
};

#endif // CUSTOMIMPLEMENTATIONS_H
