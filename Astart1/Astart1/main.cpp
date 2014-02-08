#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include "A_star.h"

//string tokenized functions
std::vector<std::string> split( string sin, char targ)
{
	std::vector<std::string> tokens;
	string token;		
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

int main(){
	std::map<std::string,float> data = loadDistanceData("data1.txt");
	MyGraph graph = loadGraphData("data2.txt");
	std::list<std::string> list;
	heoristic(graph,"Blue Mountains", "Iron Hills",data,list);
	for( std::string s : list){
		cout << s << "\n";
	}
	std::system("pause");
	return 0;
}