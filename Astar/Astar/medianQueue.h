#ifndef MEDIANQUEUE_H
#define MEDIANQUEUE_H

#include "Heap.h"

template<class T>
class medianQueue
{
private:
	//left max media
	heap<T,sub::MAX> left;
	heap<T,sub::MIN> right;
public:
	//insert x
	void insert(T x)
	{
		int dir = 0; // 0; no move, 1:send right, -1:send left
		if(x < left.peck())
		{
			left.push(x);
			if(left.size() > right.size() +1)
				dir = 1;
		}else{
			right.push(x);
			if(right.size() > left.size())
				dir = -1;
		}
		switch (dir)
		{
		case 1:
			right.push(left.pop());
			break;
		case -1:
			left.push(right.pop());
			break;
		default:
			break;
		}
	}

	T extractMedian()
	{
		T media = left.pop();
		if(right.size() > left.size())
			left.push(right.pop());
		return media;
	}

	bool empty()
	{
		return left.empty() && right.empty();
	}
};


#endif