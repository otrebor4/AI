#ifndef WEIGHTGRAPH_H
#define WEIGHTGRAPH_H
#include<list>
using namespace std;

template<typename DATA_TYPE, typename WEIGHT>
class edge;
template<typename DATA_TYPE, typename WEIGHT>
class vertex;

template<typename DATA_TYPE, typename WEIGHT_TYPE>
class edge
{
private:
	typedef edge<DATA_TYPE, WEIGHT_TYPE> Edge;
	typedef vertex<DATA_TYPE, WEIGHT_TYPE> Vertex;

public:
	WEIGHT_TYPE weight;
	vertex<DATA_TYPE,WEIGHT_TYPE> * start;
	vertex<DATA_TYPE,WEIGHT_TYPE> * end;
	edge(WEIGHT_TYPE _weight, Vertex * _start, Vertex * _end)
	{
		weight  =_weight;
		start = _start;
		end = _end;
	}
};

template<typename DATA_TYPE, typename WEIGHT_TYPE = double>
class vertex
{
private:
	typedef edge<DATA_TYPE, WEIGHT_TYPE> Edge;
	typedef vertex<DATA_TYPE, WEIGHT_TYPE> Vertex;
public:
	DATA_TYPE data;
	list<Edge *> edges;
	vertex(DATA_TYPE t)
	{
		data = t;
	}

	void AddEdge(Vertex * v, WEIGHT_TYPE w)
	{
		Edge * tedge = new Edge(w, this, v);
		edges.push_back(tedge);
	}
};


template<typename DATA_TYPE, typename WEIGHT_TYPE = double>
class weightGraph
{
private:
	typedef vertex<DATA_TYPE, WEIGHT_TYPE> Vertex;

	list<Vertex *> vertextList;

	Vertex * findVertex(DATA_TYPE t )
	{
		for each(Vertex * v in vertextList)
			if(v->data == t)
				return v;
		return NULL;
	}

public:
	weightGraph()
	{
	}

	void AddVertex(DATA_TYPE data)
	{
		Vertex * t = new Vertex(data);
		vertextList.push_back(t);

	}

	void AddEdge(DATA_TYPE a, DATA_TYPE b, WEIGHT_TYPE w)
	{
		Vertex * v1 = findVertex(a);
		Vertex * v2 = findVertex(b);
		if(v1 != NULL && v2 != NULL)
		{
			v1->AddEdge(v2,w);
			v2->AddEdge(v1,w);
		}
	}

	void display()
	{
		for each(Vertex * x in vertextList)
		{
			x->display();
		}
	}

};





#endif