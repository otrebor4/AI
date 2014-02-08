//================================
//Roberto Flores
//CMPE 3333
//================================

#ifndef BASICGRAPH_H
#define BASICGRAPH_H
#include<list>

using namespace std;
template<class V>
class vertex
{
public:
	V data;
	list<vertex<V>*> adListTo;
	list<vertex<V>*> adListFrom;
	vertex(V t)
	{
		data = t;
	}
	
	~vertex()
	{
		for(vertex<V> * v : adListTo)
		{
			v->adListFrom.remove(this);
		}
		for(vertex<V> * v : adListFrom)
		{
			v->adListTo.remove(this);
		}
		adListFrom.clear();
		adListFrom.clear();
	}

	void display()
	{
		for each(vertex<V>* x in adList)
		{
			cout << data << " " << x->data << "\n";
		}

	}

	void conectTo(vertex<V> * to)
	{
		adListTo.push_back(to);
		to->adListFrom.push_back(this);
	}
};

template<class T>
class Graph
{
protected:
	list<vertex<T>*> vertextList;

	vertex<T> * findVertex(T t )
	{
		for each(vertex<T> * v in vertextList)
			if(v->data == t)
				return v;
		return NULL;
	}

	

public:
	Graph()
	{
	}
	Graph( Graph<T> & other)
	{
		for(vertex<T> * v : other.vertextList)
		{
			AddVertex(v->data);
		}
		for(std::pair<T,T> e : other.getEdges())
		{
			PushEdge(e.first, e.second);
		}
	}

	void AddVertex(T data)
	{
		if(findVertex(data) == NULL)
		{
			vertex<T> * t = new vertex<T>(data);
			vertextList.push_back(t);
		}

	}

	void AddEdge(T a, T b)
	{
		vertex<T> * v1 = findVertex(a);
		vertex<T> * v2 = findVertex(b);
		v1->conectTo(v2);
	}

	void PushEdge(T a, T b)
	{
		vertex<T> * v1 = getVertex(a);
		vertex<T> * v2 = getVertex(b);
		v1->conectTo(v2);
	}

	void remove(T a)
	{
		vertex<T> * v = findVertex(a);
		vertextList.remove(v);
		delete v;
	}

	void display()
	{
		for each(vertex<T> * x in vertextList)
		{
			x->display();
		}
	}

	list<vertex<T>*> GetVertexs()
	{
		return vertextList;
	}
	list<std::pair<T,T>> getEdges()
	{
		list<std::pair<T,T>> edges;
		for(vertex<T> * from : vertextList)
		{
			for(vertex<T> * to : from->adListTo)
			{
				std::pair<T,T> e(from->data,to->data);
				edges.push_back(e);
			}
		}
		return edges;
	}

	vertex<T> * getVertex(T t)
	{
		vertex<T> * v = findVertex(t);
		if(v == NULL)
		{
			v = new vertex<T>(t);
			vertextList.push_back(v);
		}
		return v;
	}
};


#endif