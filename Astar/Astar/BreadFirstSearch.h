#ifndef BREADFIRSTSEARCH_H
#define BREADFIRSTSEARCH_H

#include <iostream>
#include <queue>
#include "listOp.h"
#include "basicGraph.h"

using namespace std;

template<class T>
void breadFirstSearch(directedGraph<T> G, vertex<T> v)
{
	queue<vertex<T>> Q;
	list<vertex<T>> V;
	Q.push(v);
	V.insert(v);
	while(!Q.empty())
	{
		vertex<T> current = Q.pop();
		for each(vertex<T> u in current.adList)
		{
			
		}
	}

}

#endif