#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
//#include "QLearning.h"
#include "Environment.h"
//#include "qlearning.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	std::srand(std::time(NULL));

	std::string file = "trolls1.txt";
	//std::cout << "Enter filename: ";
	//std::cin >> file;
	float alpha = .75f;
	float gamma = .50f;

	/*
	std::cout << "Alpha: ";
	std::cin >> alpha;

	std::cout << "Gamma: ";
	std::cin >> gamma;

	//*/

	QLearning e(file, alpha, gamma, 11,10,-11, 1);
	
	std::ofstream  out("data.txt");
	e.printState(std::cout);
	//std::cout << "Learning...\n";
	//e.learningPhase(100000);
	//std::cout << "Learning phase finish.\n";
	e.printQs(std::cout);
	//e.printQState(std::cout);
	int STEPS = 10000;
	for(int i = 0; i < STEPS; i++){
		e.printPath(out);
		std::cout << (float)i/(float)STEPS << "%\n";
	}
	out.close();
	system("pause");
	return 0;
}
