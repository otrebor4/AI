#include <iostream>
#include <list>
#include <vector>
#include "Heap.h"
#include "pairKeyComp.h"
using namespace std;

/*
input:
arrays, is a two dimensional array of items T
int k, number of arrays in the 2d array
size [], the size of each invidual array
size[i] is the array size for the array at arrays[i]

OutPut:
sorted list
*/
template<class T>
list<T> sortKArrays( T ** arrays, int k, int size [])
{
	list<T> out; //final sorted list
	heap< pairKeyComp<T,pair<int,int>> > minHeap(k); //pairKeyComp used to sort by value and store array index, and positin index
	for(int i = 0; i < k; i++)
	{
		pair<int,int> pos(i,0); //item info, come from i array, item position 0
		pairKeyComp<T,pair<int,int> > pair(arrays[i][0], pos ); //add fist item from each array to the minheap
		minHeap.push(pair);
	}
	while(!minHeap.empty())
	{
		pairKeyComp<T,pair<int,int>> val = minHeap.pop(); //remove item from minHeap, ensure is the smaller item of all list
		pair<int,int> pos = val.value; //get item info
		int l = pos.first; //get the index of the array where the item come
		int i = pos.second+1; //get the position on the array, add 1 to be the next item
		if(size[l] > i) //check if the next item index is in range
		{
			pos.first = l; 
			pos.second = i;
			pairKeyComp<T,pair<int,int> > pair(arrays[l][i], pos); //make data to push to the minHeap (item, itemInfo)
			minHeap.push(pair);//add item to minHeap
		}
		out.push_back(val.key);//add item to the output list
	}
	return out;
}
