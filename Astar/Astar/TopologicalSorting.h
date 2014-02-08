#ifndef TOPOLOGICALSORTING_H
#define TOPOLOGICALSORTING_H
#include <vector>
#include "basicGraph.h"

template<typename T>
vector<T> topologicalSorting(Graph<T> G)
{
	vector<T> list;
	while(!G.GetVertexs().empty())
	{
		vector<vertex<T>*> V;
		for(vertex<T> * v : G.GetVertexs())
		{
			if(v->adListFrom.empty())
				V.push_back(v);
		}
		for(vertex<T> * v : V)
		{
			list.push_back(v->data);
			G.remove(v->data);
		}
	}
	return list;
}

#endif