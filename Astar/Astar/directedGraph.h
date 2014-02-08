#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include"basicGraph.h"

template<class T>
class directedGraph
{
private:

	list<vertex<T>*> vertextList;

	vertex<T> * findVertex(T t )
	{
		for each(vertex<T> * v in vertextList)
			if(v->data == t)
				return v;
		return NULL;
	}

public:
	directedGraph()
	{
	}

	void AddVertex(T data)
	{
		vertex<T> * t = new vertex<T>(data);
		vertextList.push_back(t);

	}

	void AddEdge(T a, T b)
	{
		vertex<T> * v1 = findVertex(a);
		vertex<T> * v2 = findVertex(b);
		v1->conectTo(v2);//adList.push_back(v2);
		v2->conectTo(v1);//adList.push_back(v1);
	}

	void display()
	{
		for each(vertex<T> * x in vertextList)
		{
			x->display();
		}
	}

};

#endif