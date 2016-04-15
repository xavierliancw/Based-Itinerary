#include "customimplementations.h"

/*CustomSorts METHODS*/
CustomSorts::CustomSorts(Data dataIn)
//Constructor
{data = dataIn;}

CustomSorts::~CustomSorts()
//Destructor
{}

vector<int> CustomSorts::InsertionSort(vector<int> sortThese)
//Insertion sort
//Complexity: O(n^2)
{
     int j;     //LCV
     int temp;  //Holds swapping variable

    //Loop until i reaches sortThese's size
    for (int i = 0; i < (int)sortThese.size(); i++)
    {
        j = i;

        //While j is greater than 0 and j is less that its postdecessor
        while (j > 0
               && data.getStadName(sortThese.at(j))
               < data.getStadName(sortThese.at(j - 1)))
        {
              temp = sortThese[j];              //Store for swap
              sortThese[j] = sortThese[j-1];    //Do the swap
              sortThese[j-1] = temp;            //Finish the swap
              j--;
        }
    }
    return sortThese;
}
//========================================================================

/*MinHeap METHODS*/
template<class data>
MinHeap<data>::MinHeap()
//Constructor
{}

template<class data>
MinHeap<data>::~MinHeap()
//Destructor
//Complexity: O(n)
{clear();}

template<class data>
void MinHeap<data>::push(data addThis)
//Adds new data to the heap and restores heap order
//Complexity: O(logn)
{
    heap.push_back(addThis);
    bubbleUp(heap.size() - 1);
}

template<class data>
deque<data> MinHeap<data>::popRoot()
//Pops the root, returns swaps made, and restores heap order
//Complexity: O(logn)
{
    data holdZero;  //Holds onto min that's getting popped
    data holdLast;  //Holds onto node that's replacing min

    //Initialize
    holdZero = heap[0];
    holdLast = heap[heap.size() - 1];

    //Reset swap deque
    swaps.clear();

    //Pop the root
    heap.pop_front();

    //Copy the last data entry and push it to the front & delete the last
    heap.push_front(heap[heap.size() - 1]);
    heap.pop_back();

    //Restore heap order while updating the swaps deque
    bubbleDown();

    //Track the root swap that took place specifcally in popRoot()
    swaps.push_front(holdZero);
    swaps.push_front(holdLast);

    //Return the deque of data swaps made
    return swaps;
}

template<class data>
deque<data> MinHeap<data>::bubbleUp(unsigned int index)
//Restores heap order starting at index and returns swaps made
//Complexity: O(logn)
{
    //Reset swap deque
    swaps.clear();

    //Validate index
    if (index < heap.size() && !heap.empty())
    {
        int cursor;     //Holds deque current index
        bool loopAgain; //Determines if done bubbling
        data swapHold;  //Holds data that's being swapped

        //Start at index & activate LCV
        cursor = index;
        loopAgain = true;

        //Only bubble if there is more than one thing in the deque
        if (heap.size() > 1)
        {
            //Loop until cursor reaches root or done bubbling
            while (cursor != 0 && loopAgain)
            {
                //If cursor index data is less than its parent, swap
                if (*heap[cursor] < *heap[parentOf(cursor)])
                {
                    swapHold = heap[cursor];
                    heap[cursor] = heap[parentOf(cursor)];
                    heap[parentOf(cursor)] = swapHold;

                    //Track the swap
                    swaps.push_back(heap[cursor]);
                    swaps.push_back(heap[parentOf(cursor)]);

                    //Move cursor up
                    cursor = parentOf(cursor);
                }
                else
                {
                    //Stop looping, heap order is restored
                    loopAgain = false;
                }
            }
        }
        return swaps;
    }
    //Throw an exception
    else
    {
        string message = "MinHeap<>::bubbleUp(): ";
        ostringstream convert;
        convert << index;
        if (heap.empty())
        {
            message += "Heap is empty";
        }
        else
        {
            message += convert.str() + " is an invalid index";
        }
        throw out_of_range(message);
    }
}

template<class data>
deque<data> MinHeap<data>::bubbleDown()
//Restores heap order when root is popped and returns swaps made
//Complexity: O(logn)
{
    //Reset swap deque
    swaps.clear();

    //Only bubble if heap > 1
    if (heap.size() > 1)
    {
        int cur;    //Current index of heap
        bool done;  //LCV
        data swap;  //Holds onto data being swapped

        //Initializations
        cur = 0;        //Start cursor at root
        done = false;   //LCV start

        //Loop if any child is smaller than cur
        while ((*heap[lChildOf(cur)] < *heap[cur]
                || *heap[rChildOf(cur)] < *heap[cur]) && !done)
        {
            //Check if lChild exists
            if (lChildOf(cur) < (int)heap.size())
            {
                //If rChild exists, then both children exist
                if (rChildOf(cur) < (int)heap.size())
                {
                    //See which child is smaller, swap, and advance
                    if (*heap[lChildOf(cur)] <= *heap[rChildOf(cur)])
                    {
                        swap = heap[lChildOf(cur)];
                        heap[lChildOf(cur)] = heap[cur];
                        heap[cur] = swap;

                        //Track the swap
                        swaps.push_back(heap[cur]);
                        swaps.push_back(heap[lChildOf(cur)]);

                        cur = lChildOf(cur);
                    }
                    else
                    {
                        swap = heap[rChildOf(cur)];
                        heap[rChildOf(cur)] = heap[cur];
                        heap[cur] = swap;

                        //Track the swap
                        swaps.push_back(heap[cur]);
                        swaps.push_back(heap[rChildOf(cur)]);

                        cur = rChildOf(cur);
                    }
                }
                //If there's not rChild, then only consider lChild
                else
                {
                    //Swap if lChild is smaller
                    if (*heap[lChildOf(cur)] < *heap[cur])
                    {
                        swap = heap[lChildOf(cur)];
                        heap[lChildOf(cur)] = heap[cur];
                        heap[cur] = swap;

                        //Track the swap
                        swaps.push_back(heap[cur]);
                        swaps.push_back(heap[lChildOf(cur)]);

                        cur = lChildOf(cur);
                    }
                }
            }
            //Stop looping, there are no more children
            else
            {
                done = true;
            }
        }
    }
    return swaps;
}

template<class data>
void MinHeap<data>::clear()
//Clears out heap
//Complexity: O(n)
{
    heap.clear();
    swaps.clear();
}

template<class data>
void MinHeap<data>::replace(data withThis)
//Pops root and adds new data
//Complexity: O(logn)
{
    popRoot();
    push(withThis);
}

template<class data>
deque<data> MinHeap<data>::replace(int index, data withThis)
//Replaces data at index location of heap and returns swaps made
//Complexity: O(logn)
{
    heap[index] = withThis;
    return bubbleUp(index);
}

template<class data>
data MinHeap<data>::getMin() const
//Returns data at root
{   if (!heap.empty())
    {
        return heap[0];
    }
    else{throw out_of_range("MinHeap<>::getMin(): Heap is empty");}
}

template<class data>
data MinHeap<data>::at(unsigned int index) const
//Returns data at heap index
{
    //Validate index
    if (index < heap.size() && !heap.empty())
    {
        return heap[index];
    }
    //Throw an exception
    else
    {
        string message = "MinHeap<>::at(): ";
        ostringstream convert;
        convert << index;
        if (heap.empty())
        {
            message += "Heap is empty";
        }
        else
        {
            message += convert.str() + " is an invalid index";
        }
        throw out_of_range(message);
    }
}

template<class data>
int MinHeap<data>::size() const
//Returns size of heap
{return heap.size();}

template<class data>
bool MinHeap<data>::empty() const
//Returns if heap is empty
{return heap.empty();}

template<class data>
int MinHeap<data>::parentOf(int index)
//Calculates index number of parent
{return (index - 1) / 2;}

template<class data>
int MinHeap<data>::lChildOf(int index)
//Calculates index number of left child
{return (2 * index) + 1;}

template<class data>
int MinHeap<data>::rChildOf(int index)
//Calculates index number of right child
{return (2 * index) + 2;}
//========================================================================

/*Dijkstra METHODS*/
Dijkstra::Dijkstra(Data inData)
//Construct by copying the adjacency list
{
    data = inData;
}
Dijkstra::~Dijkstra()
//Destructor
{}

std::vector<int> Dijkstra::getDistanceMap(int start)
//Run Dijktra's algorithm on the starting vertex and return cost map
//Complexity: O(nlogn)
{
    Djobject djnode;            //Algorithm object for vertex and cost
    MinHeap<Djobject> djheap;   //Min heap of Djobjects
    std::vector<int> djmap;     //Map of heap: djmap[vertex] = heap index
    std::vector<int> distMap;   //Distances from start to each vertex
    int current = start;        //Current vertex
    int holdSwap;               //Helps update djmap

    //Grab data's adjacency list
    std::vector< list< pair<int,int> > > adjList = data.getAdjList();

    //Initialize heap
    for (unsigned int x = 0; x < /*data.size()*/0; x++)
    {
        //Create a vertex
        djnode.vertex = x;

        //Initialize starting node with 0 cost
        if (x == (unsigned int)start)
        {
            //This keeps the starting vertex at the top of the heap
            djnode.cost = 0;
        }
        //Otherwise, give it maximum cost (undetermined cost)
        else
        {
            djnode.cost = INT_MAX;
        }
        djheap.push(djnode);
    }
    //Initialize map where djmap[vertex] = location index within djheap
    djmap.resize(djheap.size());
    for (unsigned int x = 0; x < djmap.size(); x++)
    {
        djmap.at(djheap.at(x).vertex) = x;
    }
    //Initialize distance map and reset parent map
    distMap.resize(/*data.size()*/8);
    distMap[start] = start;
    vertexPath.clear();
    vertexPath.resize(/*data.size()*/8);
    vertexPath.push_back(-1);
qDebug() << "AYYHERE change later, ok?"<< djheap.empty();
    //Algorighm execution until all vertex costs are calculated O(n)
    while(!djheap.empty())
    {
        //Update current
        current = djheap.getMin().vertex;

        //Add the cost of the vertex to the distance map
        distMap[current] = djheap.getMin().cost;qDebug() << "minCOSTLFDS:" << distMap[current];

        //Pop current off the heap and eliminate it from heap
        swaps = djheap.popRoot();

        //Update the map O(logn)
        while (!swaps.empty())
        {
            holdSwap = djmap[swaps.at(0).vertex];
            djmap[swaps.at(0).vertex] = djmap[swaps.at(1).vertex];
            djmap[swaps.at(1).vertex] = holdSwap;
            swaps.pop_front();
            swaps.pop_front();
        }
        //Mark the current vertex to indicate that it's not in the heap
        djmap[current] = -1;

        //Explore neighbors of current vertex
        list< pair<int,int> >::iterator it;
        while (!adjList.at(current).empty())
        {
            //Reset iterator to beginning of adjList
            it = adjList.at(current).begin();qDebug() << (*it).first << "LOLOLOL";

            //If cost back to start is less than the cost in the heap
            if ((*it).second + distMap[current]
                    < djheap.at(djmap[(*it).first]).cost)
            {
                //Create an updated djobject
                djnode = djheap.at(djmap[(*it).first]);
                djnode.cost = (*it).second + distMap[current];

                //Update heap and map O(logn)
                swaps.clear();
                swaps = djheap.replace(djmap[(*it).first],djnode);
                while (!swaps.empty())
                {
                    holdSwap = djmap[swaps.at(0).vertex];
                    djmap[swaps.at(0).vertex]
                            = djmap[swaps.at(1).vertex];
                    djmap[swaps.at(1).vertex] = holdSwap;
                    swaps.pop_front();
                    swaps.pop_front();
                }
                //Parent of newly updated vertex is current
                vertexPath.at((*it).first) = current;
            }
            if (!adjList.at(current).empty())
            {
                adjList.at(current).pop_front();
            }
        }
    }
    return distMap;
}

//Returns map of parents to show paths to take
std::vector<int> Dijkstra::getVertexPath() const
{return vertexPath;}
