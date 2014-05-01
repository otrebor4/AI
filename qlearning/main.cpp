#include <iostream>
#include <time.h>
//#include "QLearning.h"
#include "Environment.h"


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {

	QLearning e("trolls1.txt");
	std::srand(std::time(NULL));
	e.printState(std::cout);

	e.learningPhase(100000);
	
	//e.printQState(std::cout);
	for(int i = 0; i < 10; i++)
		e.printPath(std::cout);

	system("pause");
	return 0;
}
