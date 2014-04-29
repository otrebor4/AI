#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>


int getRand(int min,int max)
{
	return min + rand()%(max-min);
}

typedef std::pair<int,int> position;

const int MAX_DIR = 9;
const position DIRECTIONS[MAX_DIR]={
	position(  0,  0),  /* NM */
	position( -1,  0),	/* N  */
	position( -1,  1),	/* NE */
	position(  0,  1),	/* E  */
	position(  1,  1),	/* SE */
	position(  1,  0),	/* S  */
	position(  1, -1),	/* SW */
	position(  0, -1),	/* W  */
	position( -1, -1)	/* NW */
};

struct data
{
	float Q[MAX_DIR];
	float maxQ;
	float sumQ;
};

class environment 
{
private:
	//Rewards
	float R_Esc; //reward for escaping
	float R_Pon; //reward for ponies
	float R_Tro; //reward for trolls
	float R_Otl; //reward for open field
	
	//learning parameter
	float Alpha;
	float Gamma;

	int N; //environment size
	int T; //number of trolls
	int P; // number of ponies

	// Excape cordinate, X,Y
	position E;

	//trolls positions
	std::vector<position> trolls;

	//ponies positions x y
	std::vector<position> ponies;

	//obstruction locations
	std::vector<position> obs;

	//burglar position
	position B;
	
	//grid knowlege data
	std::map< position, data> memory;
	
	//print msg and return code
	int logError( std::string msg, int code)
	{
		std::cout << msg;
		return code;
	}

	//pares file and build the environment from the data readed
	//the data is writed to the private variables
	//N,T,P,E,trolls,ponies,obs,B,
	int readFile( std::string filename)
	{
		std::ifstream file(filename.c_str());
		if(file.is_open())
		{
			bool error = false;
			std::string line;
			//read first line
			// N T P // N = size of the environment, G: number of trolls, P: number of ponies
			if(std::getline(file,line))
			{
				std::stringstream linein(line);
				error = !(linein >> N >> T >> P);
			}else{error = true;}
			if(error)
				return logError("Error reading line 1\n", 1);

			//read excape location
			if(std::getline(file, line))
			{
				std::stringstream linein(line);
				error = !(linein >> E.first >> E.second);
			}else{error = true;}
			if(error)
				return logError("Error reading excape location, line 2.\n",2);
			
			//read ponies locations
			if(std::getline(file,line))
			{
				std::stringstream linein(line);
				
				while(!linein.eof())
				{
					position pos;
					error = !(linein >> pos.first >> pos.second);
					if(!error)
						ponies.push_back(pos);
				}
			}else{error = true;}
			if(error)
				return logError("Error reading ponies locations\n", 3);

			//read obstruction locations
			if(std::getline(file,line))
			{
				std::stringstream linein(line);
				while(!linein.eof())
				{
					position pos;
					error = !(linein >> pos.first >> pos.second);
					if(!error && pos.first != -1 && pos.second != -1)
						obs.push_back(pos);
				}
			}else{error = true;}
			if(error)
				return logError("Error reading obstruction locations\n", 4);

			//read trolls locations
			if(std::getline(file,line))
			{
				std::stringstream linein(line);
				
				while(!linein.eof())
				{
					position pos;
					error = !(linein >> pos.first >> pos.second);
					if(!error)
						trolls.push_back(pos);
				}
			}else{error = true;}
			if(error)
				return logError("Error reading trolls locations\n", 5);

			

			
			/*
			//read burglar position
			if(std::getline(file,line))
			{
				std::stringstream linein(line);
				error = !(linein >> B.first >> B.second);		
			}else{error = true;}
			if(error)
				return logError("Error reading burglar position\n", 6);
			//*/
		}
		file.close();
		return 0;
	}
	
	//set all data from memory grid to 0
	void initState()
	{
		for(int y = 0; y < N; y++){
			for(int x = 0; x < N; x++){
				position pos(x,y);
				memory[pos] = data();
				for(int i = 0; i < MAX_DIR; i++){
					memory[pos].Q[i] = 0;
				}
				memory[pos].maxQ = 0;
				memory[pos].sumQ = 0;
			}
		}
	}

	//return what is in the word by the given x,y cordinades
	char getEntity(int x, int y)
	{
		//check ponies positions
		if(Contains(ponies,position(x,y)))
			return 'P';

		//check trolls positions
		if(Contains(trolls,position(x,y)))
			return 'T';
		
		//check obstruction locations
		if(Contains(obs,position(x,y)))
			return 'X';

		//check of the player position
		if(B.first == x && B.second == y)
			return 'B';
		//check exit
		if(E.first == x && E.second == y)
			return 'E';
		//nothing on the current position
		return '-';
	}

	//check if a pair value exist on vecto list
	bool Contains(std::vector<position> list, position value)
	{
		for(unsigned int i = 0; i < ponies.size(); i++){
			if( ponies[i].first == value.first && ponies[i].second == value.second)
				return true;
		}
		return false;
	}

	//check if from pos it can move in the direction
	bool validMove(position pos, int dir){
		int x = DIRECTIONS[dir].first+pos.first;
		int y = DIRECTIONS[dir].second+pos.second;
		return !(Contains(obs, position(x,y)));
	}

	//chose a random action
	int getRandomAction( position pos )
	{
		int action;	
		do {
			action = getRand(0,MAX_DIR);
		} while (!validMove(pos, action));
		return action;
	}

	//for the given position return reward value
	float GetReward(position pos)
	{
		char c = getEntity(pos.first, pos.second);

		switch(c){
		case 'P':
			return R_Pon;
		case 'T':
			return R_Tro;
		case 'E':
			return R_Esc;
		default:
			return R_Otl;
		}
	}

	//update learning 
	void updateFunction(position pos, int action )
	{
		position npos (pos.first + DIRECTIONS[action].first, pos.second + DIRECTIONS[action].first);

		float reward = GetReward(pos);

		//update maxQ value for the position given
		findMaxQ( npos);

		//Q learning equation
		memory[pos].Q[action] += Alpha * (reward + (Gamma * memory[npos].maxQ) - memory[pos].Q[action]);
		
		updateSum(pos);
	}

	//find max Q value for the given position
	void findMaxQ(position pos)
	{
		memory[pos].maxQ = 0.0f;
		for(int i = 0; i < MAX_DIR; i++)
		{
			if( memory[pos].Q[i] > memory[pos].maxQ )
				memory[pos].maxQ = memory[pos].Q[i];
		}
	}

	//update SumQ for the given position,
	//sumQ add all Q for all posible directions
	void updateSum(position pos)
	{
		memory[pos].sumQ = 0.0f;
		for(int i = 0; i < MAX_DIR; i++)
		{
			memory[pos].sumQ += memory[pos].Q[i];
		}
	}

public:

	environment(std::string filename)
	{
		readFile(filename);
		initState();
	}
	
	//draw current environment state to a display ostream
	void printState(std::ostream & display)
	{
		for(int y = -1; y < N+1; y++){
			for(int x = -1; x < N+1; x++){
				if( y == -1 || x == -1 || y == N || x == N){
					display << "#";
				}else{
					display << getEntity(x,y);
				}
				display << " ";
			}
			display << "\n";
		}
	}

	void Update()
	{
		//std::pair<int, int> nextPos = getNextPos();

	}
};


#endif


