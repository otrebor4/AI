#ifndef _A_START_H
#define _A_START_H


#include "pairKeyComp.h"
#include "Graph.h"
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
    EdgeData(EdgeData const & other)
    {
        edgeDistance = other.edgeDistance;
        roadQuality = other.roadQuality;
        riskLevel = other.riskLevel;
    }
	float edgeDistance;
	float roadQuality;
	float riskLevel;
};

typedef Graph<std::string,GraphType::Undirected,EdgeData> MyGraph;
typedef Graph<std::string,GraphType::MutableTree,EdgeData> MyTree;

float Pow(float v, int p)
{
    float pow = 1;
    for(int i = 0; i < p; i++)
        pow *= v;
    return pow;
}

//Callcullate G() value from the start node, to the current node, Vertex = node
float callGValueForNode(MyTree::Vertex * v)
{
	if(v == NULL)
		return 100000000.0f;//something when grong
	if(v->edgesIn.size() > 0){
		MyTree::Edge * edge = v->edgesIn.front();
        //return edge->weight.edgeDistance +          //return edge distance and parrent distance
        //    callGValueForNode(edge->start);
        
		return  (edge->weight.edgeDistance * (100/(Pow(edge->weight.roadQuality,3)+1 )))* (Pow(edge->weight.riskLevel, 5)) +
                callGValueForNode(edge->start);
	}
	return 0;
}

//check if the if the current distance is better that the new one
bool insertToTree(MyTree &tree,std::string start, std::string end, EdgeData weight,float current,float h )
{
	MyTree::Vertex * v = tree.getVertex(end);
	if (v != NULL){
		float newg = callGValueForNode(v)+h;
		if (newg < current){
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
	queue.insert(Pair(distance[from], first));
    std::map<std::string,float> visited;
	MyTree path;
	//start node
	path.AddVertex(first->data);

	bool found = false;
    //shearch for path
	while( !queue.empty() && !found){
		Pair cur = queue.removeSmaller();
		float g = callGValueForNode(path.getVertex(cur.value->data));
        //next best options is the destination node, best path found
		if(cur.value->data == to){
			found = true;
			continue;
		}
        //update  visited node history g+h
		visited[cur.value->data] = g + distance[cur.value->data];
		for( MyGraph::Edge * e : cur.value->edgesOut)
		{
            //calculate new g+h path
			float hg = e->weight.edgeDistance + g + distance[e->end->data];
            //check if node have already visited and if have a better value, if so don't add node to queue
			if( visited.find(e->end->data) != visited.end() && hg >= visited[e->end->data])
				continue;
			if( insertToTree(path, e->start->data,e->end->data,e->weight,hg,distance[e->end->data]))
				queue.insert(Pair(hg,e->end));
			
		}
	}
    //generate Path
	MyTree::Vertex * end = path.getVertex(to);
    float totatDistance = 0;
    float totalRisk = 0;
    float totalQuality = 0;
	while(end != NULL)
	{
		if(end->edgesIn.size() > 0){
			MyTree::Edge * e = end->edgesIn.front();
            std::string line = end->data + "\t "
                    + std::to_string( e->weight.riskLevel)
                    +"\t "+ std::to_string(e->weight.roadQuality);
            totatDistance += e->weight.edgeDistance;
            totalRisk += e->weight.riskLevel*e->weight.edgeDistance;
            totalQuality += e->weight.roadQuality*e->weight.edgeDistance;
			out.push_front(line);
			end = e->start;
		}else{
            std::string line = end->data + "\t 0.00000\t 0.00000";
			out.push_front(line);
			end = NULL;
		}
	}
    std::string pathInfo = "";
    pathInfo += "total Distance: " + std::to_string(totatDistance) +"\n";
    pathInfo += "average Risk: " + std::to_string(totalRisk/totatDistance ) +"\n";
    pathInfo += "average Quality: "+std::to_string(totalQuality/totatDistance)+"\n";
    out.push_back(pathInfo);
    
}



#endif
