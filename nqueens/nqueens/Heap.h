#ifndef HEAP_H
#define HEAP_H
#include <iostream>
#include <math.h>


//enum sub{MAX,MIN};
namespace sub
{
	class BASE{public: std::string name; BASE(){}};
	class MIN : public BASE{ public: MIN(){name = "MIN";}};
	class MAX : public BASE{ public: MAX(){name = "MAX";}};
}

template<class T, class OP = sub::MIN>
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
		if(_heap != NULL)
			delete [] _heap;
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
		_heap = NULL;
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
		bool val = false;
		OP _op;
		if( _op.name == "MAX")
			val = a < b;
		if( _op.name == "MIN")
			val = a > b;
		return val;
	}
	
public:
	heap()
	{
		_heap = NULL;
		Init(100);
	}

	heap(int size)
	{
		_heap = NULL;
		Init(size);
	}

	heap(heap&other)
	{
		_heap = NULL;
		Init(other._heapSize);
		for( int i = 0; i < other.itemsCount; i++)
		{
			_heap[i] = other._heap[i];
		}
		itemsCount = other.itemsCount;
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


	T remove(int index)
	{
		T out = _heap[index];
		itemsCount--;
		if(itemsCount >= 0)
		{
			T temp = _heap[itemsCount];
			_heap[index] = temp;
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

	void operator=(heap&rhs){
		Init(rhs._heapSize);
		for( int i = 0; i < other._heapSize; i++)
		{
			_heap[i] = other._heap[i];
		}
		itemsCount = other.itemsCount;
	}

	T& operator[](unsigned int i){ return _heap[i];}
};

#endif
