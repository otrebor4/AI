#ifndef QLEARNING_H
#define QLEARNING_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

int readFile( std::string filename, 
	int & N, int & T, int & P,						//file line 1: gridsize, # of trolls, #of ponies 
	std::pair<int,int> & excape,					//file line 2: excape location
	std::vector<std::pair<int,int> > & ponies,		//file line 3: ponies locations
	std::vector<std::pair<int,int> > & obstructions,//file line 4: obstructoins locations
	std::vector<std::pair<int,int> > & trolls)		//file line 5: trolls locatoins
{
	std::ifstream file(filename.c_str());
	if(file.is_open())
	{
		bool error = false;
		std::string line;
		//read first line
		// N T P // N = size of the QLearning, G: number of trolls, P: number of ponies
		if(std::getline(file,line))
		{
			std::stringstream linein(line);
			error = !(linein >> N >> T >> P);
		}else{error = true;}
		if(error)
			return 1;

		//read excape location
		if(std::getline(file, line))
		{
			std::stringstream linein(line);
			error = !(linein >> excape.first >> excape.second);
		}else{error = true;}
		if(error)
			return 2;
			
		//read ponies locations
		if(std::getline(file,line))
		{
			std::stringstream linein(line);
				
			while(!linein.eof())
			{
				std::pair<int,int> pos;
				error = !(linein >> pos.first >> pos.second);
				if(!error)
					ponies.push_back(pos);
			}
		}else{error = true;}
		if(error)
			return 3;

		//read obstruction locations
		if(std::getline(file,line))
		{
			std::stringstream linein(line);
			while(!linein.eof())
			{
				std::pair<int,int> pos;
				error = !(linein >> pos.first >> pos.second);
				if(!error && pos.first != -1 && pos.second != -1)
					obstructions.push_back(pos);
			}
		}else{error = true;}
		if(error)
			return 4;

		//read trolls locations
		if(std::getline(file,line))
		{
			std::stringstream linein(line);
			while(!linein.eof())
			{
				std::pair<int,int> pos;
				error = !(linein >> pos.first >> pos.second);
				if(!error)
					trolls.push_back(pos);
			}
		}else{error = true;}
		if(error)
			return 5;
	}
	file.close();
	return 0;
}

typedef std::pair<int,int> loc;

struct data
{
	std::vector<float> Q;
	float maxQ;
	float sumQ;
};

const int MAX_ACTIONS = 8;
const loc dir[MAX_ACTIONS] ={
	//	loc(x, y)
	loc(  0,  1),		//N
	loc(  1,  1),		//NE
	loc(  1,  0),		//E
	loc(  1, -1),		//SE
	loc(  0, -1),		//S
	loc( -1, -1),		//SW
	loc( -1,  0),		//W
	loc( -1,  1)		//NW
};

class qlearning
{
private:
	//grid size, #of trolls, # of ponies
	int N, T, P;
	//excape location
	loc excape;
	//ponies, trolls locations
	std::vector<loc> ponies, obstructions, trolls;

	std::vector<std::vector<int> > grid;

public:

	int chooseAction( loc pos, bool greedy)
	{
		int action = 0;
		int count = 0;

		if(greedy){
			findMaxQ(pos);
			for(action = 0; i < MAX_ACTIONS; action++){
				if(scape[pos.second][pos.first].maxQ == space[pos.second][pos.first].Q[action])
					if( legalMove( pos, action)) return action;
			}
			return -1;
		}else{



		}

		return action;
	}


};



#endif