#ifndef QLEARNING_H
#define QLEARNING_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

float getRandF()
{
	return (((float)rand())/((float)RAND_MAX));
}

int getRand(int min,int max)
{
	return min + rand()%(max-min);
}



struct data
{
	std::vector<float> Q;
	float sumQ;
	float maxQ;
	float visits;
};

struct loc{
	int x;
	int y;
};


bool Contains(std::vector<loc> list, loc value)
{
	for(unsigned int i = 0; i < list.size(); i++){
		if( list[i].x == value.x && list[i].y == value.y)
			return true;
	}
	return false;
}

const int MAX_ACTIONS = 8;
const loc DIRECTIONS[MAX_ACTIONS] = {
	{ -1,  0}, 	/*  N  */
	{ -1,  1},	/*  NE */
	{  0,  1},	/*  E  */
	{  1,  1},	/*  SE */
	{  1,  0},	/*  S  */
	{  1, -1},	/*  SW */
	{  0, -1},	/*  W  */
	{ -1, -1},	/*  NW */
};

class QLearning
{
private:
	//Rewards
	float R_Esc;
	float R_Pon;
	float R_Tro;
	float R_Oth;
	float R_Obs;

	//explore rate
	float Beta; 
	//learnign rate
	float Alpha; 
	//discount rate
	float Gamma;
	
	//grid zise
	int Size;
	
	//muber of ponies
	int NumPonies;
	//number of trolls
	int NumTrolls;

	//location of the excape
	loc excape;

	//the grid environment
	std::vector< std::vector<float> > grid;
	
	//locations for ponies, trolls,a nd obstructions
	std::vector<loc> ponies;
	std::vector<loc> trolls;
	std::vector<loc> obstructions;

	//Q values for each location on the grid
	std::vector<std::vector< data > > states;

	int logError(std::string msg, int i)
	{
		std::cout << msg;
		return i;
	}

	//load data from give file
	int LoadFile(std::string filename)
	{
		std::ifstream file(filename.c_str());
		if(file.is_open())
		{
			bool error = false;
			std::string line;
			// line 1:  N T P // N = size of the environment, T: number of trolls, P: number of ponies
			if(std::getline(file,line))
			{
				std::stringstream linein(line);
				error = !(linein >> Size >> NumTrolls >> NumPonies);
			}else{error = true;}
			if(error)
				return logError("Error reading line 1\n", 1);

			//line 2: read excape location
			if(std::getline(file, line))
			{
				std::stringstream linein(line);
				error = !(linein >> excape.x >> excape.y);
			}else{error = true;}
			if(error)
				return logError("Error reading excape location, line 2.\n",2);
			
			//line 3: read ponies locations
			if(std::getline(file,line))
			{
				std::stringstream linein(line);
				
				while(!linein.eof())
				{
					loc pony;
					error = !(linein >> pony.x >> pony.y);
					if(!error)
						ponies.push_back(pony);
				}
			}else{error = true;}
			if(error)
				return logError("Error reading ponies locations\n", 3);

			//line 4: obstruction locations
			if(std::getline(file,line))
			{
				std::stringstream linein(line);
				while(!linein.eof())
				{
					loc obs;
					error = !(linein >> obs.x >> obs.y);
					if(!error && obs.x != -1 && obs.y != -1)
						obstructions.push_back(obs);
				}
			}else{error = true;}
			if(error)
				return logError("Error reading obstruction locations\n", 4);

			//line 5: trolls locations
			if(std::getline(file,line))
			{
				std::stringstream linein(line);
				while(!linein.eof())
				{
					loc pos;
					error = !(linein >> pos.x >> pos.y);
					if(!error)
						trolls.push_back(pos);
				}
			}else{error = true;}
			if(error)
				return logError("Error reading trolls locations\n", 5);

		}else{
			return logError("Error: unable to open file\n", -1);
		}
		file.close();
		return 0;
	}

	//initialize
	void initialize()
	{
		initStates();
		resetGrid();
		setGrid();
	}

	//clear grid values
	void resetGrid()
	{
		grid.clear();
		for(int y = 0; y < Size; y++)
		{
			grid.push_back(std::vector<float>(Size,R_Oth));
		}
	}

	//set ponies,trolls, obstructions, and excape values to grid
	void setGrid()
	{
		//set ponies
		for( int i =0 ; i < ponies.size(); i++)
		{
			grid[ponies[i].y][ponies[i].x] = R_Pon;
		}
		//set trolls
		for( int i = 0; i < trolls.size(); i++)
		{
			grid[trolls[i].y][trolls[i].x] = R_Tro;
		}
		//set obstructions
		for(int i = 0; i < obstructions.size(); i++)
		{
			grid[obstructions[i].y][obstructions[i].x] = R_Obs;
		}
		grid[excape.y][excape.x] = R_Esc;
	}
	
	//initialize q values for the grid
	void initStates()
	{
		data init;
		init.Q.resize(MAX_ACTIONS, 0.0f);
		init.maxQ = 0;
		init.sumQ = 0;
		init.visits = 0;
		for(int i = 0; i < Size; i++)
		{
			states.push_back( std::vector<data>(Size,init));
		}
		//states.resize(Size, std::vector<data>(Size,init));
	}

	//chose random action to perform from given positoin
	int getRandomAction(loc pos)
	{
		int action;
		std::vector<bool> checked(MAX_ACTIONS, false);
		do{
			int i = 0;
			action = getRand(0,MAX_ACTIONS);
			checked[action] = true;
			for(i = 0; i < MAX_ACTIONS; i++)
				if(checked[i] == false)
					break;
			if( i == MAX_ACTIONS)
				return -1;
		}while(!validMove(pos,action));
		return action;
	}

	//chose action to perform from the given positoin
	int chooseAction(loc pos,bool greedy)
	{
		findMaxQ(pos);
		findSumQ(pos);
		int action = 0;
		int count = 0;
		if(greedy){
			for(action = 0; action  < MAX_ACTIONS; action++){
				if(states[pos.y][pos.x].Q[action] == states[pos.y][pos.x].maxQ)
					if(validMove( pos, action))
						return action;

			}
			return -1;
		}else{
			float prob;
			action = getRandomAction(pos);
			if(action == -1){
				return action;
			}
			for( count = 0; count < MAX_ACTIONS; count++){
				prob = (states[pos.y][pos.x].Q[action]/states[pos.y][pos.x].sumQ);
				if(validMove(pos,action))
					if(prob > getRandF())
						return action;
				action++;
				if(action == MAX_ACTIONS)
					action = 0;
			}
		}
		if (count == MAX_ACTIONS) 
			action = getRandomAction(pos);
		return action;
	}

	//calculate max Q value for the given position
	void findMaxQ(loc pos)
	{
		states[pos.y][pos.x].maxQ = 0.0f;
		for(int i = 0; i < MAX_ACTIONS; i++)
		{
			if( states[pos.y][pos.x].Q[i] > states[pos.y][pos.x].maxQ )
				states[pos.y][pos.x].maxQ = states[pos.y][pos.x].Q[i];
		}
	}

	//sum the Q value for the position
	void findSumQ(loc pos)
	{
		states[pos.y][pos.x].sumQ = 0.0f;
		for(int i = 0; i < MAX_ACTIONS; i++)
		{
			states[pos.y][pos.x].sumQ = states[pos.y][pos.x].Q[i];
		}
	}

	//check if from the given position can perform the action
	bool validMove(loc pos, int action)
	{
		int x = DIRECTIONS[action].x+pos.x;
		int y = DIRECTIONS[action].y+pos.y;
		
		//grid with value -99 are obstacles 
		return x >= 0 && y >= 0 && x < Size && y < Size && !( grid[y][x] == -99);
		//return !(Contains(obs, position(x,y))) && x >= 0 && y >= 0 && x < N && y < N && !(memory[position(x,y)].visided);
	}

	//get a random location with in the grid range
	loc getRandomPosition()
	{
		loc p;
		p.x = getRand(0,Size);
		p.y = getRand(0,Size);
		return p;
	}
	

	char getEntity(int x, int y)
	{
		if(x >= 0 && x < Size && y >= 0 && y < Size){
			loc l;
			l.x = x;
			l.y = y;
			if(Contains(ponies, l))
				return 'P';
			if(Contains(trolls, l))
				return 'P';
			if(Contains(obstructions, l))
				return '#';
			if( excape.x == x && excape.y == y)
				return 'E';
			return '-';
			//if(Contains(ponies, l))
			//	return 'P';
			//if(Contains(ponies, l))
			//	return 'P';
			//the position is the excape
			//if(grid[y][x] == R_Esc)
			//	return 'E';
			//the position is a pony
			//if(grid[y][x] == R_Pon)
			//	return 'P';
			//the position is a Troll
			//if(grid[y][x] == R_Tro)
			//	return 'T';
			//the position is open field
			//if(grid[y][x] == R_Oth)
			//	return '-';

		}
		//the position is a obstruction or is out of range
		return '#';
	}

	std::vector<loc> GeneratePath()
	{
		int MAXSTEPS = 1000000;
		int count = 0;
		int action = -1;
		loc pos = getRandomPosition();

		std::vector<loc> path;
		do{
			action = chooseAction(pos,true);
			if(action == -1){
				resetGrid();
				setGrid();
				path.clear();
				pos = getRandomPosition();
				
			}else{
				path.push_back(pos);
				updateFunction(pos, action);
				pos.x += DIRECTIONS[action].x;
				pos.y += DIRECTIONS[action].y;
				//if( getEntity(pos.first, pos.second) == 'E'){
				//	resetVisited();
				//	pos = getRandomPosition();
				//}
			}

			count++;
		}while(getEntity(pos.x,pos.y) != 'E' && count < MAXSTEPS);
		return path;
	}

public:
	QLearning(std::string filename)
	{
		Gamma = .5f;
		Alpha = .75f;
		Beta = 1;

		R_Esc =  11;
		R_Pon =  10;
		R_Tro = -11;
		R_Oth =   1;
		R_Obs = -99;

		LoadFile(filename);
		initialize();
	}

	QLearning(std::string filename, float alpha, float gamma, float beta)
	{
		Gamma = gamma;
		Alpha = alpha;
		Beta = beta;

		R_Esc =  11;
		R_Pon =  10;
		R_Tro = -11;
		R_Oth =   1;
		R_Obs = -99;

		LoadFile(filename);
		initialize();
	}

	void learningPhase(int Steps)
	{
		int step = 0;
		loc pos = getRandomPosition();
		int action = -1;

		while( step < Steps)
		{
			action = chooseAction(pos,false );
			if(action == -1)
			{
				resetGrid();
				setGrid();
				pos = getRandomPosition();

			}else{
				updateFunction(pos, action);
				pos.x += DIRECTIONS[action].x;
				pos.y += DIRECTIONS[action].y;

				if(getEntity(pos.x,pos.y) == 'E')
				{
					resetGrid();
					setGrid();
					pos = getRandomPosition();

				}
			}
			step++;
		}
	}

	void updateFunction(loc pos, int action)
	{
		loc npos;
		npos.x = pos.x + DIRECTIONS[action].x;
		npos.y = pos.y + DIRECTIONS[action].y;
		float reward = grid[npos.y][npos.y] + 0.1f;

		//mark it as wall to prevent goin back 
		grid[pos.y][pos.x] = R_Obs;

		findMaxQ(npos);

		float var_learn = 1.0f/pow(1.01f, states[npos.y][npos.x].visits);

		states[pos.y][pos.x].Q[action] += var_learn * Alpha *(reward + (Gamma * states[npos.y][npos.x].maxQ) - states[pos.y][pos.x].Q[action]);
	
		states[pos.y][pos.x].visits++;

		findSumQ(pos);
	}

	//draw current environment state to a display ostream
	void printState(std::ostream & display)
	{
		for(int y = -1; y < Size+1; y++){
			for(int x = -1; x < Size+1; x++){
				display << getEntity(x,y);
				display << " ";
			}
			display << "\n";
		}
	}

	void printQState(std::ostream & display)
	{
		//resetVisited();
		for(int y = -1; y < Size+1; y++){
			for(int x = -1; x < Size+1; x++){
				if( y == -1 || x == -1 || y == Size || x == Size){
					display << "#####";
				}else{
					loc p;
					p.x = x;
					p.y = y;
					findMaxQ(p);
					display << std::setw(2) << DIRECTIONS[chooseAction(p,true)].x<< "," << std::setw(2)<<DIRECTIONS[chooseAction(p,true)].y;
				}
				display << " ";
			}
			display << "\n";
		}
	}
	
	void printPath(std::ostream & display)
	{
		std::vector<loc> path = GeneratePath();
		//resetVisited();
		for(int y = -1; y < Size+1; y++){
			for(int x = -1; x < Size+1; x++){
				if( y == -1 || x == -1 || y == Size || x == Size){
					display << "#";
				}else{
					loc p;
					p.x = x;
					p.y = y;
					if(Contains(path,p))
						display << "+";
					else
						display << "-";
				}
				display << " ";
			}
			display << "\n";
		}
	}

	
	







};

#endif