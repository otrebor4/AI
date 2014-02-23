#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include "A_star.h"
#include "Graph.h"
#include "ant_op.h"
//string tokenized functions
std::vector<std::string> split( std::string sin, char targ)
{
	std::vector<std::string> tokens;
	std::string token;
	for(unsigned int i = 0; i < sin.length(); i++)		
	{
		if(sin[i] == targ)
		{
			tokens.push_back(token);
			token = "";
		}else{
			token += sin[i];
		}
	}
	if(!token.empty())
		tokens.push_back(token);
	return tokens;
}
//load distance data map.
//file forma
//<name>[tab]<distance>
std::map<std::string,float> loadDistanceData(std::string fileName)
{
	std::map<std::string,float> data;
	std::ifstream file = std::ifstream(fileName.c_str());
	std::string line;
	while(std::getline(file,line)){
		std::vector<std::string> tokens = split(line,'\t');
		if(tokens.size() >= 2){
			std::string name = tokens[0];
			float f = (float)std::atof( tokens[1].c_str());
			data[name] = f;
		}
	}
	return data;
}
//load graph data from file
//file format to load
//<from>[tab]<to>[tab]<distance>[]<roadQuality>[tab]<riskLeve>
MyGraph loadGraphData(std::string fileName)
{
	MyGraph _graph;
	std::ifstream file = std::ifstream(fileName.c_str());
	std::string line;
	while(std::getline(file,line)){
		std::vector<std::string> tokens = split(line,'\t');
		if(tokens.size() >= 5){
			std::string from = tokens[0];
			std::string to = tokens[1];
			float dist = (float)std::atof(tokens[2].c_str());
			float road = (float)std::atof(tokens[3].c_str());
			float risk = (float)std::atof(tokens[4].c_str());
			_graph.MakeEdge(from,to,EdgeData(dist,road,risk));
		}
		
	}
	return _graph;
}

AntGraph loadAntGraph(std::string fileName)
{
    AntGraph _graph;
    std::ifstream file = std::ifstream(fileName.c_str());
	std::string line;
	while(std::getline(file,line)){
		std::vector<std::string> tokens = split(line,'\t');
		if(tokens.size() >= 5){
			std::string from = tokens[0];
			std::string to = tokens[1];
			float dist = (float)std::atof(tokens[2].c_str());
			//float road = (float)std::atof(tokens[3].c_str());
            //float risk = (float)std::atof(tokens[4].c_str());
			_graph.MakeEdge(from,to, PathData(dist,10));
		}	
	}
	return _graph;
}

void print(AntPath * path, std::string start,std::string end)
{
    std::cout << "Path from " << start << " to " << end << " lend " << getLend( start, path) << "\n";
    AntPath::Vertex * v = path->getVertex(start);
    while(v != NULL)
    {
		if (v->edgesOut.size() > 0){
			AntPath::Edge * e = v->edgesOut.front();
            v = e->end;
            std::cout << e->start->data << " - " << e->end->data << "\n";
		}else
			v = NULL;
    }
    
}
void AntOp(float alpha, float beta, float rho, float q, std::string file_name, std::string start, std::string end,int n_ants, int runs)
{
	std::cout << "ALPHA: " << alpha<<" BETA: " << beta << " RHO: "  <<rho<< " Q "<<q <<std::endl;
	ALPHA = alpha;
    BETA = beta;
    RHO = rho;
    Q = q;
    AntGraph a_graph = loadAntGraph(file_name);
    for( int i = 0; i < runs; i++)
    {
        RunColony(a_graph, start,end, n_ants);
    }
    list<AntGraph::Vertex *> *visited = NULL;
    AntPath * path = GeneratePath(a_graph, a_graph.getVertex(start), a_graph.getVertex(end), visited);
    if (path != NULL)
        print(path,start,end);
}

int main(){
	std::string start = "Blue Mountains";
	std::string end = "Iron Hills";
	std::string filename = "data2.txt";
	int ants_num = 5;
	int runs = 10;

	AntOp(0.4,1.0,0.65,100,filename,start,end,ants_num,runs);

	AntOp(1.0,0.4,0.65,100,filename,start,end,ants_num,runs);

	AntOp(1.0,1.0,0.65,100,filename,start,end,ants_num,runs);
	
	AntOp(1.0,1.0,0.40,100,filename,start,end,ants_num,runs);
    
	AntOp(1.0,1.0,0.95,100,filename,start,end,ants_num,runs);

	std::map<std::string,float> data = loadDistanceData("data1.txt");
	MyGraph graph = loadGraphData("data2.txt");
	//std::cout << "places:";
    //for(std::map<std::string,float>::iterator c = data.begin(); c != data.end(); c++)
    //{
    //    std::cout << c->first <<"\n";
    //}
    //std::cout << "Enter start node: ";
    //std::string from;
    //std::getline(std::cin,from);
	std::list<std::string> list;
	heoristic(graph, start, end,data,list);
	for( std::string s : list){
		std::cout << s << "\n";
	}
	//std::system("pause");
    //*/
	std::system("pause");
	return 0;
}