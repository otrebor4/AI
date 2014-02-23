//
//  ant_op.h
//  AI_Prog1_2
//
//  Created by Roberto Flores on 2/23/14.
//  Copyright (c) 2014 Roberto Flores. All rights reserved.
//

#ifndef ANT_OP_H
#define ANT_OP_H
#include "Graph.h"
#include <list>
#include <random>

float GetFRand()
{
    return (float) ( (float)(rand()%100000)/(100000.0));
}

template <typename T>
bool IsOnList(list<T> _list, T val )
{
    for(T v : _list)
        if( v ==  val)
            return true;
    return false;
}

float ALPHA = 1;
float BETA = 1;
float Q = 100;
float RHO = 1;

class PathData
{
public:
	PathData()
	{
		edgeDistance = 0;
		pheromoneLevel = 0;
	}
	PathData(float dist, float level)
	{
		edgeDistance = dist;
		pheromoneLevel = level;
	}
    PathData(PathData const & other)
    {
        edgeDistance = other.edgeDistance;
        pheromoneLevel = other.pheromoneLevel;
    }
	float edgeDistance;
	float pheromoneLevel;
};

typedef Graph<std::string,GraphType::Undirected,PathData> AntGraph;
typedef Graph<std::string,GraphType::Directed,PathData> AntPath;

void DoEvaporation(AntGraph &graph, float rho)
{
    for(AntGraph::Edge * e : graph.getEdges())
    {
        graph.UpdateEdge(e->start->data, e->end->data, PathData(e->weight.edgeDistance, e->weight.pheromoneLevel *(1.0-rho) ) );
    }
}

float getLend(std::string start, AntPath * path)
{
    float lend = 0;
    AntPath::Vertex * v = path->getVertex(start);
    while(v != NULL)
    {
		if( v->edgesOut.size() > 0){
	        AntPath::Edge * e = v->edgesOut.front();
		    v = e->end;
		    lend += e->weight.edgeDistance;
		}
		else
			v = NULL;
    }
    
    return lend;
}

void UpdateTrails(AntGraph & graph, std::string start, AntPath * path, float pheromone )
{
    float lend = getLend(start,path);
    AntPath::Vertex * v = path->getVertex(start);
    while(v != NULL)
    {
		if (v->edgesOut.size()){
			AntPath::Edge * e = v->edgesOut.front();
            v = e->end;
            float w = e->weight.edgeDistance;
            graph.UpdateEdge( e->start->data, e->end->data, PathData(e->weight.edgeDistance,e->weight.pheromoneLevel + pheromone *(w/lend)));
        }else{
			v = NULL;
		}
    }
    
}

void append(AntPath &from, AntPath& to)
{
    for ( AntPath::Edge * e : from.getEdges())
    {
        to.MakeEdge(e->start->data,e->end->data, e->weight);
    }
}

float antProduct(AntGraph::Edge * edge)
{
    return (( Pow(edge->weight.pheromoneLevel, ALPHA ) *
             Pow( (1.0 / edge->weight.edgeDistance), BETA ) ));
    
}

AntGraph::Edge * GetNext(AntGraph::Vertex * node, list<AntGraph::Edge*>ignores)
{
    float denom = 0.0;
    for(AntGraph::Edge * e : node->edgesOut)
    {
        if( !IsOnList<AntGraph::Edge*>(ignores, e))
            denom += antProduct(e);
    }
    for(AntGraph::Edge * e : node->edgesOut)
    {
        if( !IsOnList<AntGraph::Edge*>(ignores, e)){
            float d = antProduct(e)/denom;
            if (GetFRand()<=d)
                return e;
        }
    }
    return NULL;
}

AntPath * GeneratePath( AntGraph &graph, AntGraph::Vertex * current,AntGraph::Vertex * goal, std::list<AntGraph::Vertex *> *& visited)
{
    if ( visited == NULL)
        visited = new std::list<AntGraph::Vertex *>();
    if(current == goal)
    {
        AntPath * path = new AntPath();
        return path;
    }
    
    list<AntGraph::Edge *> ignores;
    while (true)
    {
        AntGraph::Edge * next = GetNext(current,ignores);
        if (next == NULL)
            return NULL;
        ignores.push_front(next);
        if (!IsOnList<AntGraph::Vertex*>(*visited, next->end))
        {
            AntPath * p;
            visited->push_back(next->end);
            p = GeneratePath(graph, next->end,goal, visited);
            if (p != NULL){
                p->MakeEdge(next->start->data, next->end->data, next->weight);
                return p;
            }
        }
    
    }
    return NULL;
}

void RunColony(AntGraph &graph, std::string start, std::string goal, int num_ant)
{
    list<AntPath *> paths;
    int i = 0;
    while( i < num_ant)
    {
        i++;
        list<AntGraph::Vertex*> *l = NULL;
        AntPath * path = GeneratePath(graph, graph.getVertex(start), graph.getVertex(goal), l );
        if (path != NULL)
        {
            paths.push_front(path);
        }
    }
    DoEvaporation(graph, RHO);
    for(AntPath * path : paths)
    {
        UpdateTrails(graph, start,path, Q);
    }
	
}


#endif
