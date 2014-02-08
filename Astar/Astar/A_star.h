#ifndef _A_START_H
#define _A_START_H

#include "Graph.h"
#include "pairKeyComp.h"
#include "AVLTree.h"
#include <map>
#include <list>
#include <string>




class EdgeData{
public:
	EdgeData()
	{
		edgeDistance = 0;
		roadQuality = 0;
		riskLevel = 0;
	}
	EdgeData(float dist, float road, float risk)
	{
		edgeDistance = dist;
		roadQuality = road;
		riskLevel = risk;
	}
	float edgeDistance;
	float roadQuality;
	float riskLevel;
};

typedef Graph<std::string,GraphType::Undirected,EdgeData> MyGraph;
typedef Graph<std::string,GraphType::MutableTree,EdgeData> MyTree;

typedef map<MyGraph::Vertex *, MyGraph::DATA> table;


//distance from start to current vertex
float callDistanceFromParent(MyTree::Vertex * v)
{
	if(v == NULL)
		return 100000000.0f;
	if(v->edgesIn.size() > 0){
		MyTree::Edge * edge = v->edgesIn.front();
		return edge->weight.edgeDistance + callDistanceFromParent(edge->start);
	}
	return 0;
}
bool insertToTree(MyTree &tree,std::string start, std::string end, EdgeData weight,float hg,float h )
{
	MyTree::Vertex * v = tree.getVertex(end);
	if (v != NULL){
		float currentg = callDistanceFromParent(v)+h;
		if (currentg < hg){
			return false;
		}
	}
	tree.MakeEdge(start,end,weight);
	return true;
}



void heoristic(MyGraph graph, std::string from,std::string to, std::map<std::string,float> distance, std::list<std::string> &out)
{
	out.clear();
	typedef pairKeyComp<float, MyGraph::Vertex *> Pair;
	AVLTree<Pair> queue;
	MyGraph::Vertex * first = graph.getVertex(from);

	if (first == NULL){
		return;
	}
	queue.insert(Pair(distance[from],first));
	map<std::string,float> visited;
	MyTree path;
	//start node
	path.AddVertex(first->data);

	bool found = false;
	while( !queue.empty() && !found){
		Pair cur = queue.removeSmaller();
		float g = callDistanceFromParent(path.getVertex(cur.value->data));
		if(cur.value->data == to){
			found = true;
			continue;
		}
		visited[cur.value->data] = g;
		for( MyGraph::Edge * e : cur.value->edgesOut)
		{
			float hg = e->weight.edgeDistance + g;
			if( visited.find(e->end->data) != visited.end() && hg >= visited[e->end->data])
				continue;
			hg += distance[e->end->data];
			if( insertToTree(path, e->start->data,e->end->data,e->weight,hg,distance[e->end->data]))
				queue.insert(Pair(hg,e->end));
			
		}
	}
	MyTree::Vertex * end = path.getVertex(to);
	while(end != NULL)
	{
		if(end->edgesIn.size() > 0){
			MyTree::Edge * e = end->edgesIn.front();
			out.push_front(end->data);
			end = e->start;
		}else{
			out.push_front(end->data);
			end = NULL;
		}
	}
}



#endif
