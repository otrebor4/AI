#include <iostream>
#include "Environment.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	environment e("test.txt");
	e.printState(std::cout);
	system("pause");
	return 0;
}
