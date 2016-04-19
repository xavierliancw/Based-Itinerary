#include "customimplementations.h"

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
//========================================================================
/*MinMeap METHODS*/
MinMeap::MinMeap()
//Constructor
{}

MinMeap::~MinMeap()
//Destructor
{}

void MinMeap::push(pair<int,int> newData)
//Adds new value-key pair data to the heap and restores heap order
//Complexity: O(logn)
{
	//Push new key-pair into the back of the heap
    heap.push_back(newData);

    //Increase the size of the meap if necessary
    if (newData.first >= (int)meap.size())
    {
    	meap.resize(newData.first + 1);
    }

    //Meap's index holds heap's location
    meap.at((newData).first) = heap.size() - 1;

    //Bubble up to restore heap order
    bubbleUp(heap.size() - 1);
}

void MinMeap::popRoot()
//Pops the root, and restores heap order
//Complexity: O(logn)
{
	//Update the map to indicate that the value doesn't exist anymore
	meap.at(heap.front().first) = -1;

    //Pop the front of the heap
	heap.pop_front();

	//Restore heap order
	bubbleDown();
}

void MinMeap::bubbleUp(unsigned int index)
//Restores heap order starting at index
//Complexity: O(logn)
{
    //Validate index
    if (index < heap.size() && !heap.empty())
    {
        int cursor;     		//Holds deque current index
        bool loopAgain; 		//Determines if done bubbling
        pair<int,int> swapHold;	//Holds data that's being swapped
        int meapHold;			//Holds swapping data for meap

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
                if (heap[cursor].second < heap[parentOf(cursor)].second)
                {
                    swapHold = heap[cursor];
                    heap[cursor] = heap[parentOf(cursor)];
                    heap[parentOf(cursor)] = swapHold;

                    //Track the swap
                    meapHold = meap.at(heap[cursor].first);
                    meap.at(heap[cursor].first)
                    		= meap.at(heap[parentOf(cursor)].first);
                    meap.at(heap[parentOf(cursor)].first) = meapHold;

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
    }
    //Throw an exception
    else
    {
        string message = "MinMeap<>::bubbleUp(): ";
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

void MinMeap::bubbleDown()
//Restores heap order when root is popped
//Complexity: O(logn)
{
    //Only bubble if heap > 1
    if (heap.size() > 1)
    {
        int cur;    //Current index of heap
        bool done;  //LCV
        pair<int,int> swap;  //Holds onto data being swapped
        int meapHold;

        //Initializations
        cur = 0;        //Start cursor at root
        done = false;   //LCV start

        //Loop if any child is smaller than cur
        while ((heap[lChildOf(cur)].second < heap[cur].second
                || heap[rChildOf(cur)].second < heap[cur].second)
        		&& !done)
        {
            //Check if lChild exists
            if (lChildOf(cur) < (int)heap.size())
            {
                //If rChild exists, then both children exist
                if (rChildOf(cur) < (int)heap.size())
                {
                    //See which child is smaller, swap, and advance
                    if (heap[lChildOf(cur)].second
                    		<= heap[rChildOf(cur)].second)
                    {
                        swap = heap[lChildOf(cur)];
                        heap[lChildOf(cur)] = heap[cur];
                        heap[cur] = swap;

                        //Track the swap
                        meapHold = meap.at(heap[cur].first);
                        meap.at(heap[cur].first)
                        		= meap.at(heap[lChildOf(cur)].first);
                        meap.at(heap[lChildOf(cur)].first) = meapHold;

                        cur = lChildOf(cur);
                    }
                    else
                    {
                        swap = heap[rChildOf(cur)];
                        heap[rChildOf(cur)] = heap[cur];
                        heap[cur] = swap;

                        //Track the swap
                        meapHold = meap.at(heap[cur].first);
                        meap.at(heap[cur].first)
                        		= meap.at(heap[rChildOf(cur)].first);
                        meap.at(heap[rChildOf(cur)].first) = meapHold;

                        cur = rChildOf(cur);
                    }
                }
                //If there's not rChild, then only consider lChild
                else
                {
                    //Swap if lChild is smaller
                    if (heap[lChildOf(cur)].second < heap[cur].second)
                    {
                        swap = heap[lChildOf(cur)];
                        heap[lChildOf(cur)] = heap[cur];
                        heap[cur] = swap;

                        //Track the swap
                        meapHold = meap.at(heap[cur].first);
                        meap.at(heap[cur].first)
                        		= meap.at(heap[lChildOf(cur)].first);
                        meap.at(heap[lChildOf(cur)].first) = meapHold;

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
}

void MinMeap::clear()
//Clears out heap
//Complexity: O(n)
{
    heap.clear();
    meap.clear();
}

void MinMeap::replace(pair<int,int> newData)
//Pops root and adds new data
//Complexity: O(logn)
{
    popRoot();
    push(newData);
}

void MinMeap::reKey(int thisData, int newKey)
//Replaces data at index location of heap and returns swaps made
//Complexity: O(logn)
{
	//Validate that thisData exists & that the heap isn't empty
	if (meap.at(thisData) != -1 && !heap.empty())
	{
		//Validate that thisData is valid
		if (thisData >= 0)
		{qDebug() << "rekeying" << thisData << "with new key" << newKey << "heap.at(thisData).first =" << heap.at(meap.at(thisData)).first;
			heap.at(meap.at(thisData)).second = newKey;
			bubbleUp(meap.at(thisData));
		}
	    else{throw out_of_range("MinMeap<>::reKey(): "
	    		"Data cannot be negative");}
	}
    //Throw an exception
    else
    {
        string message = "MinMeap<>::reKey(): ";
        ostringstream convert;
        convert << thisData;
        if (heap.empty())
        {
            message += "Heap is empty";
        }
        else
        {
            message += "Data" + convert.str() + " does not exist in heap";
        }
        throw out_of_range(message);
    }
}

pair<int,int> MinMeap::getMin() const
//Returns data at root
{   if (!heap.empty())
    {
        return heap[0];
    }
    else{throw out_of_range("MinMeap<>::getMin(): Heap is empty");}
}

pair<int,int> MinMeap::at(unsigned int index) const
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
        string message = "MinMeap<>::at(): ";
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

pair<int,int> MinMeap::mapQuery(int data) const
//Returns data's pair
{
    //Validate that thisData exists & that the heap isn't empty
	if (meap.at(data) != -1 && !heap.empty())
	{
		//Validate that thisData is valid
		if (data >= 0)
		{
			return heap.at(meap.at(data));
		}
	    else{throw out_of_range("MinMeap<>::reKey(): "
	    		"Data cannot be negative");}
	}
    //Throw an exception
    else
    {
        string message = "MinMeap<>::reKey(): ";
        ostringstream convert;
        convert << data;
        if (heap.empty())
        {
            message += "Heap is empty";
        }
        else
        {
            message += "Data" + convert.str() + " does not exist in heap";
        }
        throw out_of_range(message);
    }
}

int MinMeap::size() const
//Returns size of heap
{return heap.size();}

bool MinMeap::empty() const
//Returns if heap is empty
{return heap.empty();}

int MinMeap::parentOf(int index)
//Calculates index number of parent
{return (index - 1) / 2;}

int MinMeap::lChildOf(int index)
//Calculates index number of left child
{return (2 * index) + 1;}

int MinMeap::rChildOf(int index)
//Calculates index number of right child
{return (2 * index) + 2;}
//========================================================================
