#ifndef QLearning_H
#define QLearning_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <iomanip>

double getRandF()
{
	return (((double)rand())/((double)RAND_MAX));
}

int getRand(int min,int max)
{
	return min + (int)((double)(max-min)*rand()/(RAND_MAX+1.0));
	//return min + rand()%(max-min);
}

typedef std::pair<int,int> position;

const int MAX_DIR = 8;
const position DIRECTIONS[MAX_DIR]={
	//position(  x,  y),  /* NM */
	position(   0, -1),	/* N  */
	position(   1, -1),	/* NE */
	position(   1,  0),	/* E  */
	position(   1,  1),	/* SE */
	position(   0,  1),	/* S  */
	position(  -1,  1),	/* SW */
	position(  -1,  0),	/* W  */
	position(  -1, -1)	/* NW */
};

struct data
{
	double Q[MAX_DIR];
	double maxQ;
	double sumQ;
	double avgQ;

	double initialReward;
	double reward;

	bool visided;
	int visits;
	//double reward;
};


int readFile( std::string filename, 
	int & N, int & T, int & P,						//file line 1: gridsize, # of trolls, #of ponies 
	std::pair<int,int> & excape,					//excape location
	std::vector<std::pair<int,int> > & ponies,		//ponies locations
	std::vector<std::pair<int,int> > & obstructions,//obstructoins locations
	std::vector<std::pair<int,int> > & trolls)		//trolls locatoins
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
				position pos;
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
				position pos;
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
				position pos;
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
	

class QLearning 
{
private:
	//Rewards
	double R_Esc; //reward for escaping
	double R_Pon; //reward for ponies
	double R_Tro; //reward for trolls
	double R_Otl; //reward for open field
	
	//learning parameter
	double Alpha;
	double Gamma;
	double Beta; //explore rate
	int N; //QLearning size
	int T; //number of trolls
	int P; // number of ponies

	// Excape cordinate, X,Y
	position E;

	//ponies positions x y
	std::vector<position> ponies;

	//obstruction locations
	std::vector<position> obs;

	//trolls positions
	std::vector<position> trolls;

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

	//learning variable
	double multiplier;

	//pares file and build the QLearning from the data readed
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
			// N T P // N = size of the QLearning, G: number of trolls, P: number of ponies
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
					memory[pos].Q[i] = .125;
				}
				memory[pos].maxQ = .125;
				memory[pos].sumQ = 1;
				
				memory[pos].initialReward = GetReward(pos);
				memory[pos].reward = memory[pos].initialReward;
				memory[pos].visided = false;
				memory[pos].visits = 0;
			}
		}
	}

	//reset visited state
	void resetStates()
	{
		B = position(-1,-1);
		for(int y = 0; y < N; y++){
			for(int x = 0; x < N; x++){
				position pos(x,y);
				memory[pos].visided = false;
				memory[pos].reward = memory[pos].initialReward;
			}
		}
		multiplier = 1;
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
		for(unsigned int i = 0; i < list.size(); i++){
			if( list[i].first == value.first && list[i].second == value.second)
				return true;
		}
		return false;
	}

	int index(std::vector<position> list, position value)
	{
		for(unsigned int i = 0; i < list.size(); i++){
			if( list[i].first == value.first && list[i].second == value.second)
				return i;
		}
		return -1;
	}
	
	//check if from pos it can move in the direction
	bool validMove(position pos, int dir){
		int x = DIRECTIONS[dir].first+pos.first;
		int y = DIRECTIONS[dir].second+pos.second;
		return !(Contains(obs, position(x,y))) && x >= 0 && y >= 0 && x < N && y < N && !(memory[position(x,y)].visided);
	}

	bool validPosition(position pos)
	{
		int x = pos.first;
		int y = pos.second;
		return !(Contains(obs, position(x,y))) && x >= 0 && y >= 0 && x < N && y < N && !(memory[position(x,y)].visided);
	}


	//chose a random action
	int getRandomAction( position pos )
	{
		int action;
		std::vector<bool> checked(MAX_DIR,false);
		do {
			int i= 0;
			action = getRand(0,MAX_DIR);
			
			for(i = 0; i < MAX_DIR; i++)
				if(checked[i] ==false)
					break;
			if( i == MAX_DIR)
				return -1;
			checked[action] = true;
		} while (!validMove(pos, action));
		return action;
	}

	//
	int chooseAction( position pos, bool greedy)
	{
		findMaxQ(pos);
		//updateSum(pos);
		int action;
		int count = 0;
		if(greedy){
			double qmax = 0;
			int qaction = -1;
			for(action = 0; action < MAX_DIR; action++)
			{
				if(memory[pos].Q[action] >= qmax && validMove(pos, action)){
					qaction = action;
					qmax = memory[pos].Q[action];
				}
			}
			return qaction;
		}else{
			double prob;
			action = getRandomAction(pos);
			if(memory[pos].sumQ == 0 || Beta > getRandF())
				return action;
			for (count = 0 ; count < MAX_DIR ; count++) {
				prob = (memory[pos].Q[action] / memory[pos].sumQ);
				if (validMove(pos, action)) {
					if (prob >= getRandF()) {
						return action;
					}
				}
				if (action++ == MAX_DIR) 
					action = 0;
			 }
		}
		if (count == MAX_DIR) 
			action = getRandomAction(pos);
		return action;
	}

	//
	void updateReward(position pos)
	{
		if(memory[pos].reward > 1)
			memory[pos].reward  = 1;
		else
			memory[pos].reward /= 2;
	}

	//for the given position return reward value
	double GetReward(position pos)
	{
		char c = getEntity(pos.first, pos.second);

		switch(c){
		case 'P':
			return R_Pon;	//ponies
		case 'T':
			return R_Tro;	//trolls
		case 'E':
			return R_Esc;	//escape
		case '-':
		case 'B':
			return R_Otl;	//open field
		default:
			return -1;
		}
		return -1;
	}


	void finAvgQ(position pos)
	{
		memory[pos].avgQ = 0.0f;
		for(int i = 0; i < MAX_DIR; i++)
		{
			memory[pos].avgQ += memory[pos].Q[i];
		}
		memory[pos].avgQ /= MAX_DIR;
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

	//
	position getRandomPosition()
	{

		position p(-1,-1);
		while(!validPosition(p) || getEntity(p.first,p.second) != '-'){
			p.first = getRand(0,N);
			p.second = getRand(0,N);
		}
		return p;
	}

public:

	QLearning(std::string filename)
	{
		R_Esc = 11; //reward for escaping
		R_Pon = 10; //reward for ponies
		R_Tro = -11; //reward for trolls
		R_Otl = 1; //reward for open field

		Alpha = 0.75f;
		Gamma = 0.5f;
		Beta  = 0.4f;
		readFile(filename);
		initState();
		resetStates();
	}
	
	QLearning(std::string filename, double alpha, double gamma)
	{
		R_Esc =  11; //reward for escaping
		R_Pon =  10; //reward for ponies
		R_Tro = -11; //reward for trolls
		R_Otl =   1; //reward for open field

		Alpha = alpha;
		Gamma = gamma;
		Beta  = 0.4f;
		int e = readFile(filename);
		if( e != 0)
			std::cout <<"ERROR READING FILE: " << e;
		initState();
		resetStates();
	}

	QLearning(std::string filename, double alpha, double gamma, int esc, int pon, int tro, int otl)
	{
		R_Esc =  esc; //reward for escaping
		R_Pon =  pon; //reward for ponies
		R_Tro =  tro; //reward for trolls
		R_Otl =  otl; //reward for open field

		Alpha = alpha;
		Gamma = gamma;
		Beta  = 0.4f;
		int e = readFile(filename);
		if( e != 0)
			std::cout <<"ERROR READING FILE: " << e;
		initState();
		resetStates();
	}

	QLearning(std::string filename, double alpha, double gamma,double beta, int esc, int pon, int tro, int otl)
	{
		R_Esc =  esc; //reward for escaping
		R_Pon =  pon; //reward for ponies
		R_Tro =  tro; //reward for trolls
		R_Otl =  otl; //reward for open field

		Alpha = alpha;
		Gamma = gamma;
		Beta  = 0.2f;
		int e = readFile(filename);
		if( e != 0)
			std::cout <<"ERROR READING FILE: " << e;
		initState();
		resetStates();
	}

	//draw current QLearning state to a display ostream
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

	void printQState(std::ostream & display)
	{
		resetStates();
		for(int y = -1; y < N+1; y++){
			for(int x = -1; x < N+1; x++){
				if( y == -1 || x == -1 || y == N || x == N){
					display << "##";
				}else{
					int i = chooseAction(position(x,y),true);
					switch (i){
					case 0:
						display << std::setw(2) << " N";
						break;
					case 1:
						display << std::setw(2) << "NE";
						break;
					case 2:
						display << std::setw(2) << " E";
						break;
					case 3:
						display << std::setw(2) << "SE";
						break;
					case 4:
						display << std::setw(2) << "S";
						break;
					case 5:
						display << std::setw(2) << "SW";
						break;
					case 6:
						display << std::setw(2) << "W";
						break;
					case 7:
						display << std::setw(2) << "NW";
						break;
					default:
						display << std::setw(2) << "##";
					}
					//position(   0,  1),	/* N  */
					//position(   1,  1),	/* NE */
					//position(   1,  0),	/* E  */
					//position(   1, -1),	/* SE */
					//position(   0, -1),	/* S  */
					//position(  -1, -1),	/* SW */
					//position(  -1,  0),	/* W  */
					//position(  -1,  1)	/* NW */
					//position p(x,y);
					//findMaxQ(p);
					//display << std::setw(2) << DIRECTIONS[chooseAction(position(x,y),true)].first<< "," << std::setw(2)<<DIRECTIONS[chooseAction(position(x,y),true)].second;
				}
				display << " ";
			}
			display << "\n";
		}
	}
	
	void printPath(std::ostream & display)
	{
		resetStates();
		std::vector<position> path = GeneratePath();
		for(int y = -1; y < N+1; y++){
			for(int x = -1; x < N+1; x++){
				if( y == -1 || x == -1 || y == N || x == N){
					display << "##";
				}else{
					position p(x,y);
					int i = index(path,p);
					if( i != -1){
						char c = getEntity(p.first, p.second);
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

	//
	void learningPhase(int steps){
		resetStates();
		int count = 0;
		int action = -1;
		position pos = getRandomPosition();
		//position ipos = pos;
		while(count < steps)
		{
			action = chooseAction(pos,false);
			if(action == -1){
				resetStates();
				pos = getRandomPosition();
				//pos = ipos;
			}else{
				updateFunction(pos, action,true);
				pos.first += DIRECTIONS[action].first;
				pos.second+= DIRECTIONS[action].second;
				char e = getEntity(pos.first, pos.second);
				if( e == 'E' || e == 'T' ){
					resetStates();
					pos = getRandomPosition();
					//pos = ipos;
				}
			}
			count++;
		}
	}



	//update learning 
	void updateFunction(position pos, int action, bool update )
	{
		position npos (pos.first + DIRECTIONS[action].first, pos.second + DIRECTIONS[action].first);
		
		//
		memory[pos].visided = true;
		if(update){
			double reward = memory[npos].reward*(1+.5*this->multiplier);// + 0.1f;
			if(memory[npos].reward == R_Pon)
			{
				memory[npos].reward = R_Otl;
				multiplier++;
			}
			findMaxQ( npos);

			memory[pos].Q[action] += Alpha * (reward + (Gamma *( memory[npos].maxQ) )- memory[pos].Q[action]);

			updateSum(pos);
		}
	}



	void printQs(std::ostream & display)
	{
		//std::vector<position> path = GeneratePath();
		resetStates();
		for(int y = -1; y < N+1; y++){
			for(int x = -1; x < N+1; x++){
				if( y == -1 || x == -1 || y == N || x == N){
					display << "#####";
				}else{
					position p(x,y);
					findMaxQ(p);
					display  << std::fixed << std::setw(5) << std::setprecision(2) << memory[p].maxQ;
				}
				display << " ";

			}
			display << "\n";

		}

	}


	std::vector<position> GeneratePath()
	{
		resetStates();
		std::vector<position> path;

		int MAXSTEPS = 100000;
		int count = 0;
		int action = -1;
		position pos = getRandomPosition();
		path.push_back(pos);
		
		do{
			action = chooseAction(pos,true);
			if(action == -1){
				resetStates();
				path.clear();
				pos = getRandomPosition();
				path.push_back(pos);
			}else{
				
				updateFunction(pos, action,false);
				pos.first += DIRECTIONS[action].first;
				pos.second+= DIRECTIONS[action].second;
				path.push_back(pos);
				if( getEntity(pos.first, pos.second) == 'T'){
					resetStates();
					path.clear();
					pos = getRandomPosition();
					path.push_back(pos);
				}
			}

			count++;
		}while(getEntity(pos.first,pos.second) != 'E'  && count < MAXSTEPS);
		return path;
	}

	void printAgentRun(std::ostream & display)
	{
		resetStates();
		std::vector<position> path = GeneratePath();
		B = path[0];
		//print inital state
		printState(display);
		B = path[path.size()-1];
		//print final state
		int pcount = 0;
		for(int y = -1; y < N+1; y++){
			for(int x = -1; x < N+1; x++){
				if( y == -1 || x == -1 || y == N || x == N){
					display << "#";
				}else{
					char c = getEntity(x,y);
					if( Contains(path, position(x,y))){
						if(c == 'P'){
							pcount++;
						}
						if( c == 'T')
							std::cout << "Death";
						c = 'O';
					}
					display << c;
				}
				display << " ";
			}
			display << "\n";
		}
		display << "Rescues: " << ((double)pcount/(double)ponies.size())*100 << "%\n";

	}
};


#endif


