/*
Name: Mark Lagunez
Course: CSCI 4350 AI
Assignment: Program #4 - Q-Learning
Date: 20 April, 2013
Description: This program implements a Q-learning method for an agent to
  traverse a 2D grid, evade trolls, capture ponies and reach the escape. We
  did not work as hard on it compared to earlier assignments  because in the
  words of Sweet Brown (nee Kimberly Wilkins) "Ain't nobody got time for that".
  Hey, what can we say, it's the end of the semester. IT WORKS -- THAT'S ALL
  THAT MATTERS!

  Moving right along, this program is based on our textbook's implementation.
  One particular pitfall with that implementation was that "action loops" could
  form, given certain inputs. When following the greedy approach (versus prob-
  abilistic greedy or "p_greedy"), the agent could get stuck pursuing local
  maxima based on the q-values of certain actions between two states. To avoid
  this terrible fate, our program prevents the agent from visiting previous
  states. Additionally, we use a multiplier for the reward that increases after
  each pony is caught, as well as when the burglar reaches the escape. This is
  to encourage the burglar to go after ponies rather than make a beeline to the
  escape. It also mitigates the issues with local maxima to a degree.

  We use various parameter combinations as per the handout assignment. The
  effect of changes to the learning, exploration, and discount rates are
  detailed in our attached paper. These can be changed via the constructor of
  the qLearner class.

  Additionally, we have implemented moving trolls and a variable learning rate.
  The variable learning rate is inversely exponentially proportional to the
  number of times a state (i.e. a location on the grid) has been visited. So, 
  the learning rate (which affects the q-value update for the action taken)
  will decrease as the number of times a state has been visited increases.

  One problem: trolls cornering burglar, capturing etc. 

*/

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <cmath>

using namespace std;

#define GREEDY		0	/* Choose best Q */
#define P_GREEDY	1	/* Probabilistically choose best Q */

#define getSRand()	((double)rand() / (double)RAND_MAX)
#define getRand(x)	(int)((double)(x)*rand()/(RAND_MAX+1.0))

const int MAX_ACTIONS = 8;	// 8 possible directions to move in
int actionSelection = P_GREEDY;

// Q values for all possible actions at a state
//    up to 8 actions exist at a point in the grid, 1 for each direction
typedef struct {
	vector<double> Q;
	double sumQ;
	double maxQ;
    int visits;
} stateAction_t;

// note these locations are initialized {y,x} (row, col)!
typedef struct loc_t {
	int y;
	int x;
} loc_t;

// movement within the grid
const loc_t dir[MAX_ACTIONS] = {
	{ -1,  0}, 	/*  N  */
	{ -1,  1},	/*  NE */
	{  0,  1},	/*  E  */
	{  1,  1},	/*  SE */
	{  1,  0},	/*  S  */
	{  1, -1},	/*  SW */
	{  0, -1},	/*  W  */
	{ -1, -1},	/*  NW */
};

class qLearner {
	const double EXPLORE_RATE;
	const double LEARNING_RATE;
	const double DISCOUNT_RATE;


    // for the current implementation, X_MAX = Y_MAX = N
	int X_MAX;
	int Y_MAX;

	// start and end coordinates
	loc_t start;
	loc_t end;

    double multiplier;
    int ponyCount;

	// the grid world / environment
	vector< vector<int> > grid;

    // locations for ponies, trolls and obstacles
    vector<loc_t> ponyLocs;
    vector<loc_t> trollLocs;
	// used for troll movement, reset when burglar escapes
    vector<loc_t> newTrollLocs; 
    vector<loc_t> obstacleLocs;

	// Q values for each location in the grid
	vector< vector<stateAction_t> > stateSpace;

	// creates an empty grid with an impassable border
	void initEmptyGrid();

	// places things on the grid
    void setGrid();

    // output the values stored in grid
    void outputGrid();

	// moves trolls around on the grid
	// uses troll locations in newTrollLocs
    void moveTroll();

    // BOOK FUNCTIONS
	void findMaxQ( int y, int x );
	
	void updateSum( int y, int x );

	int legalMove( int y_state, int x_state, int action );

    int getRandomAction( int y, int x );

	int chooseAction( int y, int x );

	// outputs the maxQ values in the state space
    void showSpace();

	void updateFunction( int y, int x, int action );

	public:

		// default constructor
		qLearner():EXPLORE_RATE(0.2), LEARNING_RATE(0.6),
			DISCOUNT_RATE(0.5), X_MAX(0), Y_MAX(0) {};

		// fancy constructor
		qLearner(double e_rate, double l_rate, double d_rate):
			EXPLORE_RATE(e_rate), LEARNING_RATE(l_rate),
			DISCOUNT_RATE(d_rate), X_MAX(0), Y_MAX(0) {};

		// prompts for input file, then reads in values
		//    to initialise the grid environment  
		//    places trolls, ponies, obstacles,
		//    start, and end values on grid
		void initialiseGrid();

		// set all Q values to 0
		void initialiseStateSpace();

		// P Greedy action selection
		// an epoch is a single step (action selection)
		// if the agent reaches the end, it is placed back at start
		void learnPhase(int max_epochs);

		// Greedy action selection
		void showPath();

}; // class qLearner


int main()
{
	int max_epochs = 100000;    // 100 000
    qLearner Q1;

    // explore, learning, discount
	//qLearner Q2(0.2, 0.75, 0.5);
    //qLearner Q2(0.2, 0.1, 0.5);
    //qLearner Q2(0.2, 0.5, 0.5);
    //qLearner Q2(0.2, 0.9, 0.5);
    qLearner Q2(0.2, 0.6, 0.1);   // bad
    //qLearner Q2(0.2, 0.6, 0.5);
    //qLearner Q2(0.2, 0.6, 0.9);


	Q2.initialiseGrid();
	Q2.initialiseStateSpace();
	Q2.learnPhase(max_epochs);
	Q2.showPath();

	return 0;
}

/*	qLearner Public Member Functions	*/
void qLearner::initialiseGrid()
{
	string fname, line;

	cout << "\nPlease enter the name of the input file: ";
	cin >> fname;
	
	ifstream inf(fname.c_str());
    // NOTE: row = y, col = x
	int N, numTrolls, numPonies, row, col, i;


	try {
		// Input 6 lines from file

		// Line 1: N (grid size), # of trolls, # of ponies
		if(getline(inf,line)) {
            stringstream ss(line);
            ss >> N >> numTrolls >> numPonies;
			Y_MAX = X_MAX = N;
        }
		else
			throw 42;

		// Line 2: Escape location (x, y)
		if(getline(inf,line)) {
            stringstream ss(line);
            ss >> col >> row;
			start.x = col;
			start.y = row;
		}
		else
			throw 42;

		// Line 3: Troll locations
		i = 0;
        if(getline(inf,line)) {
            stringstream ss(line);
		    while( i < numTrolls ) {
                ss >> col >> row;
			    loc_t loc = {row, col};
			    trollLocs.push_back(loc);
            	++i;
			}
        }
		else
			throw 42;

		// Line 4: Pony locations
		i = 0;
        if(getline(inf,line)) {
            stringstream ss(line);
		    while( i < numPonies ) {
                ss >> col >> row;
			    loc_t loc = {row, col};
			    ponyLocs.push_back(loc);
            	++i;
			}
        }
		else
			throw 42;

		// Line 5: Obstacle locations
		if(getline(inf, line)) {
            stringstream ss(line);
            while( ss >> col >> row ) {
                loc_t loc = {row, col};
                if( row != -1 && col != -1)
                    obstacleLocs.push_back(loc);
            }
		}
		else
			throw 42;

		// Line 6: Start location (x, y)
        if(getline(inf,line)) {
            stringstream ss(line);
            ss >> col >> row;
			end.x = col;
			end.y = row;
		}
		else
			throw 42;
		
		inf.close();

        // create an empty NxN grid
    	initEmptyGrid();

        // place things on the grid
        setGrid();

        //outputGrid();
        
	}
	catch (int e) {
    
        cout << "\nThere was an error reading the file.\n";
        return;
	}

} // end initialiseGrid()

void qLearner::setGrid()
{
    int i;

    // place ponies, trolls, obstacles, start and end
    for(i = 0; i < ponyLocs.size(); ++i)
        grid[ponyLocs[i].y][ponyLocs[i].x] = 100;
    for(i = 0; i < trollLocs.size(); ++i)
        grid[trollLocs[i].y][trollLocs[i].x] = -999;
    for(i = 0; i < obstacleLocs.size(); ++i)
        grid[obstacleLocs[i].y][obstacleLocs[i].x] = -1;
    grid[start.y][start.x] = 0;
    grid[end.y][end.x] = 1000;

} // end setGrid()

// MODIFIED BOOK CODE
void qLearner::initialiseStateSpace()
{
	if(grid.empty())
		return;

    int x_state, y_state, action;
    stateAction_t tempAction;

    tempAction.Q.resize(MAX_ACTIONS, 0.0);
    tempAction.sumQ = 0.0;
    tempAction.maxQ = 0.0;
    tempAction.visits = 0;

    vector<stateAction_t> tempActionVector(Y_MAX, tempAction);

    for ( y_state = 0 ; y_state < Y_MAX ; y_state++ )
        stateSpace.push_back(tempActionVector);
 
} // end initialiseStateSpace()

void qLearner::learnPhase(int max_epochs)
{
	int epochs = 0, x = start.x, y = start.y, action;
    multiplier = 1;
    srand(time(NULL));

	while (epochs < max_epochs) {

        moveTroll();
        action = chooseAction( y, x );

        if(action == -1) {
            //cout << "\nAgent got stuck.\n";
		// reset the agent
            ();
            setGrid();
            multiplier = 1;
            x = start.x; y = start.y;
        }


        else {
            updateFunction( y, x, action );

            x += dir[action].x;
            y += dir[action].y;

		// reset the agent
            if ((x == end.x) && (y == end.y)) {
              initEmptyGrid();
              setGrid();
              multiplier = 1;
              x = start.x; y = start.y;
            }

        }
	    ++ epochs;
	}

    cout << "\nLearning phase complete.\n";

} // end learnPhase()

void qLearner::showPath()
{
	if(grid.empty())
		return;


    initEmptyGrid();
    setGrid();

    /*** BOOK CODE ***/

  int x, y, action;
  int ttlPony = ponyLocs.size();

  actionSelection = GREEDY;

  outputGrid();  

  showSpace();

  x = start.x; y = start.y;
  ponyCount = 0;

  while (1) {

    grid[y][x] = 0;

    if ((x == end.x) && (y == end.y)) break;

    moveTroll();
    action = chooseAction( y, x );


  //  cout << action << " " << y << " " << x << "\n";

    x += dir[action].x;
    y += dir[action].y;

    if(grid[y][x] == 100 || grid[y][x] == -900) {
       ++ponyCount;
    }
  }

  outputGrid();

  cout << "\n  ";
  cout << ponyCount << " of ";
  cout << ttlPony;
  cout << " ponies caught" << endl;
	// a bizarre segmentation fault has a chance of occurring
	//   unless the output is flushed (endl)

} // end showPath()

/*	qLearner Private Member Functions	*/

void qLearner::initEmptyGrid()
{
    grid.clear();

    // create a grid with a border of -1's and interior of 1's
    grid.push_back(vector<int> (X_MAX, -1));
    for(int i = 1; i < (X_MAX - 1); ++i) {
        grid.push_back(vector<int> (X_MAX, 1));
        grid[i][0] = -1;
        grid[i][(X_MAX - 1)] = -1;
    }
   grid.push_back(vector<int> (X_MAX, -1));

   newTrollLocs = trollLocs;

} // end initEmptyGrid()

void qLearner::outputGrid()
{
    char z;
  
    for(int k = 0; k < X_MAX; ++k) {
        cout << "\n";
        for(int j = 0; j < X_MAX; ++j) {
            switch(grid[k][j]) {
                case -1: z = '*';
                    break;

                // possible values for a troll on the grid
                case -900:
                case -1000:
                case -1001:
                case -999: z = 'T';
                    break;
                case 1000: z = 'E';
                    break;
                case 100: z = 'P';
                    break;
                case 1: z = '-';
                    break;
                case 0: z = '0';
                    break;
                default: z = 'x';
                    break;
            }
            cout << setw(3) << z;
        }
    }

   cout << endl; 

}

void qLearner::moveTroll()
{
    int numTrolls = trollLocs.size(), action, gridVal, trollY, trollX;
    double trollMoveProb = 0.5;

    for(int i = 0; i != numTrolls; ++i) {
        // 50% chance a troll will *try* to move
        if( getSRand() < trollMoveProb ) {
            trollY = newTrollLocs[i].y;
            trollX = newTrollLocs[i].x;
            action = rand() % MAX_ACTIONS;
            if( legalMove(trollY, trollX, action) ) {

  //  outputGrid();

                // update the grid
                grid[trollY][trollX] += 1000;
                grid[trollY + dir[action].y][trollX + dir[action].x] -= 1000;

  //  outputGrid();

                // update the stored troll location
                newTrollLocs[i].y += dir[action].y;
                newTrollLocs[i].x += dir[action].x;
            }
        }
    }

}

void qLearner::showSpace()
{
    ofstream outf("out.txt");

    for(int i = 0; i < Y_MAX; ++i) {
        outf << "\n";
        for(int j = 0; j < Y_MAX; ++j)
            outf << setprecision(1) << fixed << setw(8)
                << stateSpace[j][i].maxQ;
    }

    outf << "\n";

    outf.close();

} // end showSpace()

/***    !!!COPIOUS AMOUNTS OF BOOK CODE BELOW!!!    ***/
void qLearner::findMaxQ( int y, int x )
{

  int i;

  stateSpace[y][x].maxQ = 0.0;

  for (i = 0 ; i < MAX_ACTIONS ; i++) {

    if (grid[y + dir[i].y][x + dir[i].x] != 0
            && grid[y + dir[i].y][x + dir[i].x] != -1)
    if (stateSpace[y][x].Q[i] > stateSpace[y][x].maxQ) {

      stateSpace[y][x].maxQ = stateSpace[y][x].Q[i];

    }

  }

} // end findMaxQ()

void qLearner::updateSum( int y, int x )
{

  int i;

  stateSpace[y][x].sumQ = 0.0;

  for (i = 0 ; i < MAX_ACTIONS ; i++) {

    stateSpace[y][x].sumQ += stateSpace[y][x].Q[i];

  }

} // end updateSum()

int qLearner::legalMove( int y_state, int x_state, int action )
{

  int y = y_state + dir[action].y;
  int x = x_state + dir[action].x;

  if (grid[y][x] < 0) return 0;
  else return 1;
  
} // end legalMove()

int qLearner::getRandomAction( int y, int x )
{
	// added a tabu list, adds overhead, but avoids action loops
  int action, i;
    vector<bool> tabu(X_MAX, false);

  do {
    action = getRand( MAX_ACTIONS );
    tabu[action] = true;
    for(i = 0; i < MAX_ACTIONS; ++i)
        if(tabu[i] == false)
            break;

    if(i == MAX_ACTIONS)
        return -1;

  } while (!legalMove(y, x, action));

  return action;
} // end getRandomAction()

int qLearner::chooseAction( int y, int x )
{
  int action;
  int count;

  if (actionSelection == GREEDY) {

    findMaxQ(y, x);

    for (action = 0 ; action < MAX_ACTIONS ; action++) {

    // change stuff

      if (stateSpace[y][x].maxQ == stateSpace[y][x].Q[action]) {
        if (legalMove(y, x, action)) return(action);
      }

    }

    cout << endl << "The burglar was eaten by a troll!" << endl;
    abort();

  } else if (actionSelection == P_GREEDY) {

    if ((EXPLORE_RATE > getSRand()) ||
        (stateSpace[y][x].sumQ == 0.0)) {

      /* Explore! */
      action = getRandomAction(y, x);

    } else {

      double prob;

      /* Exploit! */
      action = getRandomAction(y, x);

      for (count = 0 ; count < MAX_ACTIONS ; count++) {

        prob = (stateSpace[y][x].Q[action] / stateSpace[y][x].sumQ);

        if (legalMove(y, x, action)) {
          if (prob > getSRand()) {
            return(action);
          }
        }

        if (++action == MAX_ACTIONS) action = 0;

      }

      /* Went through the possible actions, but did not pick one.  At
       * this point, we'll just pick one randomly.
       */
      if (count == MAX_ACTIONS) {
        action = getRandomAction(y, x);
      }

    }

  } else {
    assert(0);
  }

  return action;


} // end chooseAction()

void qLearner::updateFunction( int y, int x, int action )
{
  int newy = y + dir[action].y;
  int newx = x + dir[action].x;
  double reward;

  reward = (double)grid[newy][newx] * multiplier + 0.1;


  // multipliers create more interesting behavior
  if(grid[newy][newx] > 1)
    multiplier *= 2;

  grid[y][x] = -1;  // don't revisit previous states

/*
    for(int i = 0; i < ponyLocs.size(); ++i) {
        if(ponyLocs[i].x == newx && ponyLocs[i].y == newy)
            grid[newy][newx] = 0;   // take pony off grid
    }
*/

  /* Update the maxQ value for the state */
  findMaxQ( newy, newx );


  /* Q_Learning */

  // variable learning rate: 1/(1.01^x)
  double var_learn = 1.0 / pow(1.01, stateSpace[y][x].visits);

  stateSpace[y][x].Q[action] += var_learn * 
    LEARNING_RATE * (reward + (DISCOUNT_RATE * stateSpace[newy][newx].maxQ) -
                      stateSpace[y][x].Q[action]);

  stateSpace[y][x].visits += 1;


  /* Update the payoff sum */
  updateSum( y, x );

} // end updateFunction()

// Obligatory book code copyright disclaimer below. . .
// Copyright (c) 2003-2005 Charles River Media.  All rights reserved.
