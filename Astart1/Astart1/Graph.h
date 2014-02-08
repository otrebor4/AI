#ifndef GRAPH_H
#define GRAPH_H

#include <list>
using std::list;


template<typename DATA_TYPE, typename EDGE_DATA>
class edge;
template<typename DATA_TYPE, typename EDGE_DATA>
class vertex;

template<typename DATA_TYPE, typename EDGE_DATA>
class edge
{
private:
	typedef edge<DATA_TYPE, EDGE_DATA> Edge;
	typedef vertex<DATA_TYPE, EDGE_DATA> Vertex;
	
public:
	EDGE_DATA weight;
	vertex<DATA_TYPE,EDGE_DATA> * start;
	vertex<DATA_TYPE,EDGE_DATA> * end;
	edge(EDGE_DATA _weight, Vertex * _start, Vertex * _end)
	{
		weight  =_weight;
		start = _start;
		end = _end;
	}

	~edge()
	{
		start->edgesOut.remove(this);
		end->edgesIn.remove(this);
	}
	bool equals(Edge other)
	{
		return (other.start->data == start->data && other.end->data == end->data);
	}
};

template<typename DATA_TYPE, typename EDGE_DATA = double>
class vertex
{
private:
	typedef edge<DATA_TYPE, EDGE_DATA> Edge;
	typedef vertex<DATA_TYPE, EDGE_DATA> Vertex;
	void AddEdgeIn(Edge * tedge)
	{
		//Edge * tedge = new Edge(w,v,this);
		if(! have(edgesIn,tedge))
			edgesIn.push_back(tedge);
	}
	bool have( list<Edge *> list, Edge * check)
	{
		for(Edge * e : list)
			if((*e).equals((*check)))
				return true;
		return false;
	}
public:
	DATA_TYPE data;
	list<Edge *> edgesOut;
	list<Edge *> edgesIn;

	vertex(){}
	vertex(DATA_TYPE t)
	{
		data = t;
	}

	~vertex()
	{
		vector<Edge *> es;
		for(Edge * e : edgesIn){
			es.push_back(e);
		}
		for(Edge * e : edgesOut){
			es.push_back(e);
		}
		
		for(int i = 0; i < es.size(); i++)
		{
			Edge * e = es[i];
			delete e;
		}
		edgesOut.clear();
		edgesIn.clear();
		
	}
	void clearEdgesOut()
	{
		list<Edge *> toRemove;
		for(Edge * e : edgesOut)
			toRemove.push_back(e);
		for(Edge * e : toRemove)
			delete e;
	}

	void clearEdgesIn()
	{
		list<Edge *> toRemove;
		for(Edge * e : edgesIn)
			toRemove.push_back(e);
		for(Edge * e : toRemove)
			delete e;
	}

	void AddEdge(Vertex * v, EDGE_DATA w)
	{
		Edge * tedge = new Edge(w, this, v);
		if( !have(edgesOut,tedge))
			edgesOut.push_back(tedge);
		v->AddEdgeIn(tedge);
	}
};

enum GraphType
{
	Undirected,
	Directed,
	MutableTree,
	Tree
};

template<typename DATA_TYPE, GraphType GRAPHTYPE = GraphType::Undirected, typename EDGE_DATA = double>
class Graph
{
public:
	typedef vertex<DATA_TYPE, EDGE_DATA> Vertex;
	typedef edge<DATA_TYPE, EDGE_DATA> Edge;
	typedef std::pair<EDGE_DATA,DATA_TYPE> DATA;
private:
	
	list<Vertex *> vertextList;

	Vertex * findVertex(DATA_TYPE t )
	{
		for(Vertex * v : vertextList)
			if(v->data == t)
				return v;
		return NULL;
	}

	//===================================================
	void AddEdgeUndirected(DATA_TYPE a,DATA_TYPE b, EDGE_DATA w)
	{
		Vertex * v1 = findVertex(a);
		Vertex * v2 = findVertex(b);
		if(v1 != NULL && v2 != NULL)
		{
			v1->AddEdge(v2,w);
			v2->AddEdge(v1,w);
		}
	}

	void AddEdgeDirected(DATA_TYPE a, DATA_TYPE b, EDGE_DATA w)
	{
		Vertex * v1 = findVertex(a);
		Vertex * v2 = findVertex(b);
		if(v1 != NULL && v2 != NULL)
		{
			v1->AddEdge(v2,w);
		}
	}

	void AddEdgeTree(DATA_TYPE a, DATA_TYPE b, EDGE_DATA w, bool replace = false)
	{
		Vertex * v1 = findVertex(a);
		Vertex * v2 = findVertex(b);
		if( !replace && (v1->edgesIn.size() > 0 || v2->edgesOut.size() > 0))
			return;
		//if(replace && v1->edgesIn.size() > 0)
		//	v1->clearEdgesIn();
		if(replace && v2->edgesIn.size() > 0)
			v2->clearEdgesIn();
		v1->AddEdge(v2,w);

	}

public:
	Graph(){}
	
	Graph(Graph & other)
	{
		for(Vertex * v : other.vertextList)
		{
			AddVertex( v->data);
		}
		for(Edge * e : other.getEdges())
		{
			MakeEdge( e->start->data, e->end->data, e->weight);
		}
	}

	~Graph()
	{
		clear();
	}

	void AddVertex(DATA_TYPE data)
	{
		Vertex * t = new Vertex(data);
		vertextList.push_back(t);
	}

	template<GraphType TYPE>
	void AddEdge(DATA_TYPE a, DATA_TYPE b, EDGE_DATA w = default(EDGE_DATA))
	{
		switch (TYPE)
		{
		case Undirected:
			AddEdgeUndirected(a,b,w);
			break;
		case Directed:
			AddEdgeDirected(a,b,w);
			break;
		case Tree:
			AddEdgeTree(a,b,w,false);
		case MutableTree:
			AddEdgeTree(a,b,w,true);
		default:
			break;
		}
	}

	void AddEdge(DATA_TYPE a, DATA_TYPE b, EDGE_DATA w = default(EDGE_DATA))
	{
		switch (GRAPHTYPE)
		{
		case Undirected:
			AddEdgeUndirected(a,b,w);
			break;
		case Directed:
			AddEdgeDirected(a,b,w);
			break;
		case Tree:
			AddEdgeTree(a,b,w,false);
		case MutableTree:
			AddEdgeTree(a,b,w,true);
		default:
			break;
		}
	}
	
	template<GraphType TYPE>
	void MakeEdge(DATA_TYPE a, DATA_TYPE b, EDGE_DATA w )
	{
		Vertex * v = findVertex(a);
		if(v == NULL)
			AddVertex(a);
		v = findVertex(b);
		if(v == NULL)
			AddVertex(b);
		AddEdge<TYPE>(a,b,w);
	}

	void MakeEdge(DATA_TYPE a, DATA_TYPE b, EDGE_DATA w )
	{
		Vertex * v = findVertex(a);
		if(v == NULL)
			AddVertex(a);
		v = findVertex(b);
		if(v == NULL)
			AddVertex(b);
		AddEdge(a,b,w);
	}

	Edge * getEdges(DATA_TYPE a, DATA_TYPE b)
	{
		Vertex * _a = getVertex(a);
		for(Edge * e : _a->edgesOut)
		{
			if(e->end->data == b)
				return e;
		}
		return NULL;
	}

	Vertex * getVertex(DATA_TYPE a)
	{
		return findVertex(a);
	}
	list<Vertex *> getVertexs()
	{
		return vertextList;
	}
	list<Edge *> getEdges()
	{
		list<Edge *> edges;
		for(Vertex * v : vertextList)
		{
			for(Edge * e : v->edgesOut)
				edges.push_back(e);
		}
		return edges;
	}

	void clear()
	{
		for(Edge * e : getEdges())
			delete e;
		for(Vertex * v : vertextList)
			delete v;
		vertextList.clear();
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