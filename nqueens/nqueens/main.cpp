#include <iostream>
#include <cstdlib>
#include "nqueens.h"

int* getVals()
{
	int * arr = new int[10];
	for( int i = 0; i < 10; i++)
	{
		arr[i] = i;
	}
	return arr;
}

int run(int N)
{
	NQueens q(N, 0.15f);
	genotype best(N);
	bool found = false;
	int i = 0;
	while( !found && i < 1000)
	{
		best = q.populationUpdate();
		if( best.conflicts() == 0)
			found = true;
		i++;
	}
	std::cout << ((found) ? "" : "solution no found\n");
	std::cout << "Generation: "<< i << "\n";
	std::cout << best.str() << "\n";
	std::cout << best.draw() << "\n";
	//system("pause");
	return 0;
}

int main()
{
	int N  = 8;
	for( int i = 0; i < 20; i++)
	{
		run(N);
	}
	system("pause");
	return 0;
}