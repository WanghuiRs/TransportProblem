#include <iostream>
#include <ctime>
#include"../header/ga.h"

using namespace std;

int main()
{
	srand((unsigned)time(NULL));

	//details of Genetic Algorithm parameters
	cout << "Genetic Algorithm parameters:" << endl;
	cout << "\tGenerations:\t" << Generations << endl;
	cout << "\tPopulation Size:\t" << populationSize << endl;
	cout << "\tElites:\t" << Elite << endl;
	cout << "\tPC(Two Point Crossover):\t" << PC << endl;
	cout << "\tPM(Bit Mutation):\t" << PM << endl;
	cout << endl;

	Function task;
	clock_t start = clock();

	for (int i = 1; i <= 1; ++i)
	{
		cout << "Execution Times:\t" << i << endl;
		task.evolve();
		cout << endl;
	}

	clock_t ends = clock();
	printf("Running Time:%.2lf s\n", (double)(ends - start) / CLOCKS_PER_SEC);

	system("pause");
	return 0;
}