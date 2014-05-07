#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
//#include "QLearning.h"
#include "Environment.h"
//#include "qlearning.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void Execute(std::string filename, std::string outfile, double learning_rate, double discount_factor, double exploration_rate)
{
	QLearning e(filename, learning_rate, discount_factor,exploration_rate, 11,10,-11, 1);
	std::ofstream  out(outfile.c_str());
	e.printState(std::cout);
	std::cout << "Learning...\n";
	e.learningPhase(500000);
	std::cout << "Learning phase finish.\n";
	for(int i = 0; i < 25; i++)
		e.printAgentRun(out);
	out.close();
}


void Data()
{
	std::string infile = "trolls1.txt";
	//default
	std::string outfile = "out.txt";
	float alpha = .75f;
	float gamma = .50f;
	float exploration = .4f;
	Execute(infile, outfile, alpha, gamma, exploration);

	outfile = "out1_5.txt";
	alpha = .1f;
	gamma = .5f;
	Execute(infile, outfile, alpha, gamma, exploration);

	outfile = "out5_5.txt";
	alpha = .5f;
	gamma = .5f;
	Execute(infile, outfile, alpha, gamma, exploration);

	outfile = "out9_5.txt";
	alpha = .9f;
	gamma = .5f;
	Execute(infile, outfile, alpha, gamma, exploration);

	outfile = "out6_1.txt";
	alpha = .6f;
	gamma = .1f;
	Execute(infile, outfile, alpha, gamma, exploration);

	outfile = "out6_5.txt";
	alpha = .6f;
	gamma = .5f;
	Execute(infile, outfile, alpha, gamma, exploration);

	outfile = "out6_9.txt";
	alpha = .6f;
	gamma = .9f;
	Execute(infile, outfile, alpha, gamma, exploration);


}

int main(int argc, char** argv) {
	std::srand(std::time(NULL));

	Data();



	/*
	QLearning e(file, alpha, gamma, 11,10,-11, 1);
	
	std::ofstream  out("data.txt");
	e.printState(std::cout);
	std::cout << "Learning...\n";
	e.learningPhase(100000);
	std::cout << "Learning phase finish.\n";
	e.printQState(std::cout);
	int STEPS = 10;
	for(int i = 0; i < STEPS; i++){
		e.printPath(std::cout);
		//std::cout << (float)i/(float)STEPS << "%\n";
	}
	out.close();
	//*/





	system("pause");
	return 0;
}


