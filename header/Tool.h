//
// Created by 王慧 on 2019-03-18.
//

#ifndef PRODUCTIONANDDISTRIBUTION_TOOL_H
#define PRODUCTIONANDDISTRIBUTION_TOOL_H

#include <cstdlib>
#include <random>
#include <ctime>
#include "../resource/GATwo.h"

using namespace GATwoSpace;
static int NUM = 9999;
static std::default_random_engine random_engine((unsigned int)time(0));

class Tool {
private:

public:
    static int randomIntervalTwo(unsigned int, unsigned int);
    static double random0To1Two();
    static bool cmpIndividualFitness(IndividualTwo, IndividualTwo);
};


#endif //PRODUCTIONANDDISTRIBUTION_TOOL_H
