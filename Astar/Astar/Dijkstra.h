#ifndef DIJKSTRA_H
#define DIHKSTRA_H
#include <map>
#include "Graph.h"
#include "AVLTree.h"
#include "pairKeyComp.h"

#define FINF 0xFFFFFFFFFFFFFFFF

using namespace std;
template<typename data, GraphType type, typename weight >
weight dijkstra(Graph<data,type,weight> G, data _first, data _end, Graph<data,type,weight> & out)
{
	out.clear();

	typedef edge<data,weight> Edge;
	typedef vertex<data, weight> Vertex;
	typedef std::pair<weight,data> DATA;
	typedef map<Vertex *, DATA> table;
	typedef pairKeyComp<weight,Vertex *> Pair;
	AVLTree<Pair> queue;

	table vals;

	Vertex * first = G.getVertex(_first);
	Vertex * end = G.getVertex(_end);

	if(first == NULL || end == NULL){
		weight w =NULL;
		return w;
	}
	for(Vertex * v : G.getVertexs()){
		DATA d;
		d.first = FINF;
		vals[v] = d;
	}
	vals[first].first = 0;
	for(Edge * e : first->edgesOut)
	{
		queue.insert(Pair(e->weight,e->end));
		vals[e->end] = DATA(e->weight, e->start->data);
	}
	while(!queue.empty())
	{
		Pair u = queue.removeSmaller();
		for( Edge * e : u.value->edgesOut)
		{
			weight we = u.key + e->weight;
			if(vals[e->end].first > we)
			{
				weight t = vals[e->end].first;
				queue.remove(Pair(t, e->start));
				queue.insert(Pair(we, e->end));
				vals[e->end] = DATA(we,e->start->data);
			}
		}
	}
	
	bool error = false;
	Vertex * current = end;
	while(current != first && !error)
	{
		DATA d = vals[current];
		if(d.first == FINF){
			error = true;
		}else{
			Edge * e = G.getEdges(d.second,current->data);
			if(e == NULL)
				error = true;
			else{
				out.MakeEdge(e->start->data, e->end->data, e->weight);
				current = e->start;
			}
		}
	}
	if(error)
		out.clear();
	DATA d = vals[end];
	return d.first;
}


#endif