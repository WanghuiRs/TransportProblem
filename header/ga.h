#ifndef _GA_H_
#define _GA_H_

#include <bitset>   

using namespace std;

enum ChoiceError {Normal, Exceed};//sign used for judging whether choice is legal

//operational parameter
static const int knapsack = 1000;//content of  knapsack
static const int ItemNum = 50;//num of  items
static const int populationSize = 100;
static const int Generations = 1000;
static const int Elite = 5;
static const double PC = 0.6;//probability of Crossover
static const double PM = 0.01;//probability of Swap Mutation 

//data structure
struct Item 
{
	int no;
	int value;
	int cost;
	double density;
};

struct Individual
{
	bitset<ItemNum> choice;//whether pick or not for each item
	int fitness;
	int cost;
};

class Function//test function
{
//attribute
private:
	Individual population[populationSize + 1];// all the individuals
	Individual offspring[2 * populationSize + 1];//all the offspring
	int numOfOffspring;//the number of offspring

//operation
public:
	void evolve();//genetic algothrim

	void initialize();//Initialize the  population

	void crossover();
	void mutate();
	void select();

	void print();//print the bset solution on the screen

private:
	void calculateFitness(Individual&);//calculate fitness  
	void calculateCost(Individual&);//calculate cost

	void TPX(Individual&, Individual&);//Two-Point Crossover
	void BM(Individual&);//Bit Mutation
	void repair(Individual&);//local research

	void calculateProbability(Individual*, int);//Calculate  cumulative  probability in roulette
};//end for class Function

#endif