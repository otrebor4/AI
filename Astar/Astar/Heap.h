#ifndef HEAP_H
#define HEAP_H
#include <iostream>
#include <math.h>


enum sub{MAX,MIN};
template<class T, sub _op = sub::MIN>
class heap
{
private:
	static const int IMC_NUM = 100; //resising
	int _heapSize; //array size
	T * _heap; //_heaparray
	int itemsCount;

	int getParentIndex(int index)
	{
		return (index-1)/2;
	}
	int getLChildIndex(int index)
	{
		return 2*index+1;
	}
	int getRChildIndex(int index)
	{
		return 2*index +2;
	}
	//
	int getTChildIndex(int index)
	{
		int li = getLChildIndex(index);
		int ri = getRChildIndex(index);
		if(itemsCount <= li)//index is a leaf
			return index;
		if(itemsCount <= ri)//li is the only child
			return li;

		if( op(_heap[li], _heap[ri]) )
			return ri;
		else
			return li;

	}

	T parent(int index)
	{
		int i = getParentIndex(index);
		return _heap[i];
	}
	T left(int index)
	{
		int i = getLChildIndex(index);
		return _heap[i];
	}
	T right(int index)
	{
		int i = getRChildIndex(index);
		return _heap[i];
	}

	void Init(int size)
	{
		_heapSize =size;
		_heap = new T[_heapSize];
		itemsCount = 0;
	}

	void swap(T & a, T & b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

	void incrementArray(unsigned int space = IMC_NUM)
	{
		T * temp = _heap;
		int oldSize = _heapSize;
		Init(_heapSize+space);
		//copy array to new array
		for(int i = 0; i < oldSize; i++)
		{
			_heap[i] = temp[i];
			itemsCount++;
		}
	}

	bool op(T a, T b)
	{
		switch (_op)
		{
		case MAX:
			return a < b;
		case MIN:
			return a > b;
		}
		return false;
	}
public:
	heap()
	{
		Init(100);
	}

	heap(int size)
	{
		Init(size);
	}

	~heap()
	{
		delete [] _heap;
	}

	void push(T val)
	{
		if(itemsCount == _heapSize)
			incrementArray();

		int index = itemsCount;
		_heap[index] = val;
		itemsCount++;
		while( op( parent(index), _heap[index]) )
		{
			swap(_heap[getParentIndex(index)],_heap[index]);
			index = getParentIndex(index);
		}
	}

	T pop()
	{
		T out = _heap[0];
		itemsCount--;
		//check if need swaping
		if(itemsCount >= 0)
		{
			T temp = _heap[itemsCount];
			_heap[0] = temp;
			int index = 0;
			int child = getTChildIndex(index);
			while( op(_heap[index], _heap[child]) )
			{
				swap(_heap[index], _heap[child]);
				index = child;
				child=getTChildIndex(index);
			}
		}
		return out;
	}

	T peck()
	{
		return _heap[0];
	}

	bool empty()
	{
		return itemsCount <= 0;
	}

	int size()
	{
		return itemsCount;
	}

	void draw( std::string sep = " ")
	{
		for(int i = 0; i < itemsCount; i++)
		{
			cout << _heap[i] << sep;
		}
	}
};

#endif