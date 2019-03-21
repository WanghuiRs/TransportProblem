//
// Created by 王慧 on 2019-03-18.
//

#ifndef PRODUCTIONANDDISTRIBUTION_GATWO_H
#define PRODUCTIONANDDISTRIBUTION_GATWO_H

#include <Eigen/Dense>
namespace GATwoSpace {
    static const int centerNum = 4;
    static const int companyNum = 4;
    static const int rowNum = companyNum + centerNum;
    static const int colNum = centerNum*(centerNum + companyNum);
    static const int max_num = 500;
    static const int distance1[][companyNum] = {{4, 12, 8,  11},
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
    static const int generation = 100;
    static const int elite = 3;
    static const double PC = 0.6;
    static const double PM = 0.02;
    static const int penalty = max_num/2;
}
using namespace GATwoSpace;
using namespace Eigen;
typedef struct {
    int number;
    int distance;
} dist;

struct IndividualTwo {
    int fitness;
    int cost;
    int transportSign[centerNum][companyNum + centerNum];
    int transportAmount[centerNum][companyNum + centerNum];
};

class GATwo {
private:
    IndividualTwo population[populationSize];
    IndividualTwo offspring[3 * populationSize];
    int offspringNum;

public:
    void evolve();
    void initialize();
    void crossover();
    void mutate();
    void select();
    void caculateCost();
    void printIndividualTwo();
    void soluteEquation(IndividualTwo);
private:
    void twoPointCross(IndividualTwo &,IndividualTwo &);
    void rouletteAlg(int );
};


#endif //PRODUCTIONANDDISTRIBUTION_GATWO_H
