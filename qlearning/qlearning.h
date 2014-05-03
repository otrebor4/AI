#ifndef QLEARNING_H
#define QLEARNING_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

float getRandF()
{
	return (((float)rand())/((float)RAND_MAX));
}

int getRand(int min,int max)
{
	int range = (max-min);

	return min + (int)(  (float)(range*rand()) / (float)(RAND_MAX + 1.0));
	//return min + rand()%(max-min);
}

bool Contains(std::vector<std::pair<int,int> > list, std::pair<int,int> value)
{
	for(unsigned int i = 0; i < list.size(); i++){
		if( list[i].first == value.first && list[i].second == value.second)				
			return true;
	}
	return false;
} 

int index(std::vector<std::pair<int,int> > list, std::pair<int,int> value)
{
	for(unsigned int i = 0; i < list.size(); i++){
		if( list[i].first == value.first && list[i].second == value.second)
			return i;
	}
	return -1;
}

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
	float explorationRate;

	//Rewards
	int R_Esc; //reward for escaping
	int R_Pon; //reward for ponies
	int R_Tro; //reward for trolls
	int R_Otl; //reward for open field
	
	//learning parameter
	float Alpha;
	float Gamma;



	//grid size, #of trolls, # of ponies
	int N, T, P;
	//excape location
	loc excape;
	//ponies, trolls locations
	std::vector<loc> ponies, obstructions, trolls;

	std::vector<std::vector<int> > grid;
	std::vector<std::vector<data>> space;

	void initState()
	{
		space.clear();
		data q;
		
		q.Q.resize(MAX_ACTIONS,01.0f);
		q.sumQ = 0.0f;
		q.maxQ = 0.0f;

		for(int y = 0; y < N; y++){
			space.push_back( std::vector<data>(N,q));
		}
	}

	void initGrid()
	{
		grid.empty();
		for(int y = 0; y < N; y++){
			grid.push_back( std::vector<int>(N,0) );
		}
	}

	void loadGrid()
	{
		initGrid();

		for(int y = 0; y < N; y++){
			for(int x = 0; x < N; x++){
				grid[y][x] = getReward(loc(x,y));
			}
		}
	}

	//return what is in the word by the given x,y cordinades
	char getEntity(loc pos)
	{
		//check ponies positions
		if(Contains(ponies,pos))
			return 'P';

		//check trolls positions
		if(Contains(trolls,pos))
			return 'T';
		
		//check obstruction locations
		if(Contains(obstructions, pos))
			return 'X';

		//check of the player position
		//if(B.first == x && B.second == y)
		//	return 'B';
		//check exit
		if(excape.first == pos.first && excape.second == pos.second)
			return 'E';
		//nothing on the current position
		return '-';
	}

	//for the given position return reward value
	int getReward(loc pos)
	{
		char c = getEntity( pos );

		switch(c){
		case 'P':
			return R_Pon;
		case 'T':
			return R_Tro;
		case 'E':
			return R_Esc;
		case '-':
			return R_Otl;
		default:
			return -1;
		}
		return -1;
	}

	//Find the max Q value for a given location
	void findMaxQ(loc pos)
	{
		space[pos.second][pos.first].maxQ = 0;
		for(int i = 0; i < MAX_ACTIONS; i++)
		{
			int x = pos.first + dir[i].first;
			int y = pos.second + dir[i].second;
			if(  ! (( 0 <= x && x < N) && ( 0 <= y && y < N)) )
				space[pos.second][pos.first].Q[i] = -1;

			else if( space[pos.second][pos.first].maxQ < space[pos.second][pos.first].Q[i] )
				space[pos.second][pos.first].maxQ = space[pos.second][pos.first].Q[i];
		}
	}

	//find the sum of Q values for a given location
	void findSumQ(loc pos)
	{
		space[pos.second][pos.first].sumQ = 0;
		for(int i = 0; i < MAX_ACTIONS; i++)
		{
			int x = pos.first + dir[i].first;
			int y = pos.second + dir[i].second;
			if(  (( 0 <= x && x < N) && ( 0 <= y && y < N)) )
				space[pos.second][pos.first].sumQ += space[pos.second][pos.first].Q[i];
		}
	}

	//check if the current action is valid for the the given location
	bool legalMove(loc pos, int action)
	{
		if( action < 0 || action >= MAX_ACTIONS)
			return false;
		int x = pos.first + dir[action].first;
		int y = pos.second + dir[action].second;

		return   ( 0 <= x && x < N) && ( 0 <= y && y < N) && grid[y][x] != -1;
	}

	//return a random legal action to perform if it have one or -1 if no legal move
	int getRandomAction(loc pos)
	{
		int action = -1;
		bool checked [MAX_ACTIONS] = {false,false,false,false,false,false,false,false};
		while(!legalMove(pos,action)){
			action = getRand(0,MAX_ACTIONS);
			
			int i = 0;
			for(i = 0; i < MAX_ACTIONS; i++)
				if(!checked[i])
					break;
			if(i == MAX_ACTIONS)
				return -1;

			checked[action] = true;
		}
		return action;
	}

	//get random position with in the grid 
	loc getRandomPosition()
	{
		loc p;
		p.first  = getRand(0,N);
		p.second = getRand(0,N);
		return p;
	}

public:

	qlearning(std::string filename)
	{
		R_Esc =  11; //reward for escaping
		R_Pon =  10; //reward for ponies
		R_Tro = -11; //reward for trolls
		R_Otl =   1; //reward for open field


		explorationRate = .25f;
		Alpha = 0.75f;
		Gamma = 0.5f;

		readFile(filename,N,T,P,excape,ponies,obstructions,trolls);
		initState();
		loadGrid();
		//initState();
		//resetStates();
	}

	void learningPhase(int steps)
	{
		int step = 0;
		loc pos = getRandomPosition();
		while(step < steps)
		{
			int action = chooseAction(pos,false);
			updateFunction(pos,action);
			pos.first += dir[action].first;
			pos.second += dir[action].second;
			if(pos.first == excape.first && pos.second == excape.second){
				 pos = getRandomPosition();
			}
			step++;
		}
	}

	//return a legal action base on greedy or probalbilistic selection. return-1 if no legal move available 
	int chooseAction( loc pos, bool greedy)
	{
		int action = 0;
		int count = 0;

		if(greedy){
			findMaxQ(pos);
			for(action = 0; action < MAX_ACTIONS; action++){
				if(space[pos.second][pos.first].maxQ == space[pos.second][pos.first].Q[action])
					if( legalMove( pos, action)) return action;
			}
			return -1;
		}else{
			//explore
			action = getRandomAction(pos);
			if(explorationRate <= getRandF() && space[pos.second][pos.first].sumQ != 0){
				float prob;
				for(count = 0; count < MAX_ACTIONS; count++){
					prob = (space[pos.second][pos.first].sumQ / space[pos.second][pos.first].maxQ);
					if( legalMove( pos,action))
						if(prob > getRandF())
							return action;
					action++;
					if(action == MAX_ACTIONS)
						action = 0;
				}
				if(count == MAX_ACTIONS)
					action = getRandomAction(pos);
			}
		}
		return action;
	}

	//for the given location and action, perform q-learing update value
	void updateFunction( loc pos, int action)
	{
		loc npos ( pos.first + dir[action].first, pos.second + dir[action].second);

		float reward = (float)grid[npos.second][npos.first] + 0.1f;
		findMaxQ(npos);
		space[pos.second][pos.first].Q[action] += Alpha * (reward +  (Gamma * space[npos.second][npos.first].maxQ) - space[pos.second][pos.first].Q[action]);
		findSumQ(pos);
	}

	std::vector<loc> getPath( loc start, loc end)
	{
		loc current = start;
		std::vector<loc> path;
		path.push_back(current);
		while(current.first != current.first || current.second != end.second){
			int action = chooseAction( current, true);
			if(action == -1)
				break;
			current.first += dir[action].first;
			current.second += dir[action].second;

			path.push_back(current);
		}
		
		return path;
	}

	void printPath(std::ostream & display)
	{
		initGrid();
		loc s = getRandomPosition();
		std::vector<loc> path = getPath(s,excape);
		
		for(int y = -1; y < N+1; y++){
			for(int x = -1; x < N+1; x++){
				if( y == -1 || x == -1 || y == N || x == N){
					display << "##";
				}else{
					loc p(x,y);
					int i = index(path,p);
					if( i != -1){
						char c = getEntity(p );
						if(c == '-')
							display << std::fixed << std::setw(2) << i;
						else
							display << std::fixed << std::setw(2) << c;
					}
					else
						display << "--";
				}
				display << " ";
			}
			display << "\n";
		}
	}
};



#endif