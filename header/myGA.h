//
// Created by 王慧 on 2019-03-11.
//

#ifndef PRODUCTIONANDDISTRIBUTION_MYGA_H
#define PRODUCTIONANDDISTRIBUTION_MYGA_H

namespace GAOneSpace {
    static const int centerNum = 4;
    static const int companyNum = 4;
    static const int max_num = 200;
    static const int distance1[][companyNum + 1] = {{4, 12, 8,  11},
                                                    {2, 10, 5,  9},
                                                    {8, 5,  11, 6},
                                                    {7, 9,  7,  3}};

    static const int distance2[][centerNum] = {{0,  6, 11, 9},
                                               {6,  0, 9,  7},
                                               {11, 9, 0,  7},
                                               {9,  7, 7,  0}};

    static const int demandedAmount[] = {8, 14, 12, 14};
    static const int totalAmount[] = {14, 6, 12, 16};

    static const int populationSize = 100;
    static const int Generations = 1000;
    static const int elite = 0;
    static const double PC = 0.6;
    static const double PM = 0.05;
}
using namespace GAOneSpace;
typedef struct {
    int distance;
    int row;
    int col;
    int realRow;
    int realCol;
} DistanceInfo;

struct Individual
{
    int fitness;
    int cost;
    int transportAmount[centerNum][companyNum+centerNum];
//    Individual operator=(Individual& temp)
//    {
//        fitness = temp.fitness;
//        cost = temp.cost;
//        memcpy(transportSign, temp.transportSign, centerNum*(centerNum+companyNum) * sizeof(int));
//        return *this;
//    };
};

class Function//test function
{
//attribute
private:
    Individual population[populationSize];
    Individual offspring[2 * populationSize];//all the offspring
    int numOfOffspring;//the number of offspring

//operation
public:
    void evolve();//genetic algothrim

    void initialize();

    void crossover();
    void mutate();
    void select();

    void print();//print the bset solution on the screen
    void printPopulation(int);
    void printIndividual(Individual);
    void checkPopulation();
    void roulette(int rouletteNum);
private:
    void calculateFitness(Individual&);//calculate fitness
    void calculateCost(Individual&);//calculate cost

    void TPX(Individual&, Individual&);//Two-Point Crossover
    void formateTransport(Individual&);
};


#endif //PRODUCTIONANDDISTRIBUTION_MYGA_H
