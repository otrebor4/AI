#ifndef GRIDGRAPH_H
#define GRIDGRAPH_H
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Graph.h"
#include "Dijkstra.h"


using std::vector;
using std::pair;

class gridGraph
{
private:
	typedef Graph<std::pair<int,int>,GraphType::Directed,double> graph;
	graph m_graph;
	std::pair<int,int> m_size;

	std::pair<int,int> randPos(std::pair<int,int> l)
	{
		std::pair<int,int> out;
		out.first  = 1+rand()%l.first;
		out.second = 1+rand()%l.second;
		return out;
	}

	void getLine(std::istream & in, std::stringstream & out)
	{
		std::string line = "";
		std::getline(in, line);
		out = std::stringstream(line);
	}

	void loadGraph(vector<vector<double>> grid)
	{
		if(grid.size() == 0)
			return;
		m_size.first = grid.size();
		m_size.second = grid[0].size();
		m_graph.clear();
		for( int y = 0; y < grid.size(); y++)
		{
			for(int x = 0; x < grid[y].size(); x++)
			{
				for(int i = -1; i <= 1; i += 2)
				{
					if( y + i >= 0 && y + i < grid.size())
						m_graph.MakeEdge(pair<int,int>(x+1,y+1), pair<int,int>(x+1, y+1+i), grid[y+i][x]);
					if( x + i >= 0 && x + i < grid[y].size())
						m_graph.MakeEdge(pair<int,int>(x+1,y+1), pair<int,int>(x+1+i, y+1), grid[y][x+i]);
				}
			}
		}
	}

	vector<vector<double>> parseMap(std::istream & in, pair<int,int> & start, pair<int,int> & end)
	{
		vector<vector<double>> grid;
		std::stringstream ss;
		getLine(in, ss);
		ss >> start.first;//x
		ss >> start.second;//y
		getLine(in, ss);
		ss >> end.first;
		ss >> end.second;

		while(!in.eof())
		{
			getLine(in, ss);
			vector<double> row;
			while(!ss.eof())
			{
				double d;
				ss >> d;
				row.push_back(d);
			}
			grid.push_back(row);
		}
		return grid;
	}

	double makePath(pair<int,int> from, pair<int,int> to, graph & out)
	{
		return dijkstra<pair<int,int>, GraphType::Directed, double>(m_graph,from,to, out);
	}

	string drawMap(graph p)
	{
		vector<string> lines;
		for(int y = 0; y < m_size.first; y++)
		{
			for(int x = 0; x < m_size.second; x++)
			{
				if(lines.size() <= y)
					lines.push_back("");
				lines[y].append("x");
			}
		}
		for( vertex<pair<int,int>,double> * v : p.getVertexs())
		{
			if(v->data.second-1 >= 0 && v->data.second-1 < lines.size() && v->data.first-1 >= 0 && v->data.first-1 < lines[v->data.second-1].size())
				lines[v->data.second-1][v->data.first-1] = '@';
		}
		string out ="";
		for(int i = 0; i < lines.size(); i++)
		{
			out += lines[i] + " \n";
			//cout << out;
		}
		return out;
	}

	void savePath(graph p)
	{
		
	}

public:

	void load(std::string fileName)
	{
		std::ifstream in (fileName);
		pair<int,int> start;
		pair<int,int> end;
		loadGraph(parseMap(in, start, end));
		in.close();
	}
	
	std::string findPath(pair<int,int> start,pair<int,int> end)
	{
		graph g;
		double d = makePath(start,end,g);
		return to_string(d)+"\n"+drawMap(g);
	}

	std::string run(std::string fileName)
	{
		std::ifstream in (fileName);
		pair<int,int> start;
		pair<int,int> end;
		loadGraph(parseMap(in, start, end));
		in.close();

		graph g;
		double d = makePath(start,end,g);
		return to_string(d)+"\n"+drawMap(g);
	}

	std::pair<int,int> randPos()
	{
		return randPos(m_size);
	}

};

#endif