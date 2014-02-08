/*
Roberto Flores
CMPE 3333
*/
#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <list>

namespace listOp
{
	template<class T>
	T get(list<T> inlist, T key)
	{
		for each(T t in inlist)
		if(t ==  key)
			return t;
		return NULL;
	}
	template<class T>
	bool contains(list<T> inlist, T key)
	{
		return get(inlist,key) != NULL;
	}
};
#endif