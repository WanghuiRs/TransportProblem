//
// Created by 王慧 on 2019-03-13.
//

#ifndef PRODUCTIONANDDISTRIBUTION_TOOL_H
#define PRODUCTIONANDDISTRIBUTION_TOOL_H

#include <cstdlib>
#include <cmath>
#include <random>
#include <ctime>
#include <algorithm>

#include "myGA.h"
//to generate pure random decimal
static const int N = 9999;
static const double PI = 3.1415926;

static double random0To1();//interval [ 0, 1 ]
static int randomInterval(unsigned int, unsigned int);//interval [ lowerBound, upperBound ]
static void LowerFirst(int&, int&);//make sure lower first
static double BiToDe(int, int, double, double);//Transfrom Binary To Decimal
static void cmp(Individual, Individual);
static std::default_random_engine e((unsigned int)time(0));


static int minFunc(int a, int b) {
    return a < b ? a:b;
}

static int randomInterval(unsigned int left,unsigned int  right) {
    std::uniform_int_distribution<unsigned> u(left, right);
    return u(e);
}

bool cmmp(DistanceInfo d1, DistanceInfo d2) {
    return d1.distance < d2.distance;
}
bool cmmmp(Individual o, Individual t) {
    return o.fitness > t.fitness;
}
void sortDistance(std::vector<DistanceInfo> &distInfoVector) {

    for (int i = 0; i < centerNum; ++i) {
        for (int j = 0; j < companyNum; ++j) {
            DistanceInfo distInfo;
            distInfo.distance = distance1[i][j];
            distInfo.row = i;
            distInfo.col = j;
            distInfoVector.push_back(distInfo);
        }

    }
    std::sort(distInfoVector.begin(), distInfoVector.end(), cmmp);
}

void sortDistance2(std::vector<DistanceInfo> &distInfoVector, int rowFrom , int rowLen) {

    for (int i = 0; i < rowLen; ++i) {
        for (int j = 0; j < companyNum; ++j) {
            DistanceInfo distInfo;
            distInfo.distance = distance1[rowFrom+i][j];
            distInfo.row = i;
            distInfo.col = j;
            distInfoVector.push_back(distInfo);
        }

    }
    std::sort(distInfoVector.begin(), distInfoVector.end(), cmmp);
}


static double random0To1(){
    double temp = randomInterval(0,N);
    return temp / (double)(N + 1);
}

static void LowerFirst(int& start, int& end)//make sure start less than stop
{
    int temp;
    if (start > end)
    {
        temp = start;
        start = end;
        end = temp;
    }
}//end for LowerFirst

static double BiToDe(int binary, int bits, double lowerBonud, double higherBonud)//Transfrom Binary To Decimal
{
    return lowerBonud + (higherBonud - lowerBonud) * (double)binary / (double)(pow(2, bits) - 1);
}//end fro BiToDe




#endif //PRODUCTIONANDDISTRIBUTION_TOOL_H
