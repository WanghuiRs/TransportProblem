//
// Created by 王慧 on 2019-03-18.
//

#include "../header/Tool.h"
/*
int MyTools::minFunc(int a, int b) {
    return a < b ? a:b;
}
**/
int Tool::randomIntervalTwo(unsigned int left, unsigned int right) {
    std::uniform_int_distribution<unsigned> u(left, right);
    return u(random_engine);
}
double Tool::random0To1Two() {
    double temp = randomIntervalTwo(0,NUM);
    return temp / (double)(NUM + 1);
}

bool Tool::cmpIndividualFitness(IndividualTwo o, IndividualTwo t) {
    return o.fitness > t.fitness;
}