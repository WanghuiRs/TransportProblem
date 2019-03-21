#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "../header/myGA.h"
#include "../header/tools.h"
#include <list>

void Function::evolve() {
    initialize();

    for (int t = 0; t < Generations; ++t) {
        crossover();
        mutate();
        //检查是否有分配错误的
        checkPopulation();
        select();
    }
    print();
}

void Function::initialize() {
    int tempTotalAmount[centerNum] = {0};
    int tempDemandeAmount[companyNum] = {0};


    for (int i = 0; i < populationSize; ++i) {
        //初始化个体的运输值
        //初始化每个中心仓库的总量
        for (int l = 0; l < centerNum; ++l) {
            tempTotalAmount[l] = totalAmount[l];
        }
        for (int n = 0; n < companyNum; ++n) {
            tempDemandeAmount[n] = demandedAmount[n];
        }
        //随机分配转运的
        list<int> rowList;

        for (int r = 0; r < centerNum; ++r) {
            rowList.push_back(r);
        }
        for (int j = 0; j < centerNum; ++j) {
            int randomRow = randomInterval(0, (unsigned int)rowList.size() - 1);
            auto rowIter = rowList.begin();
            advance(rowIter,randomRow);
            list<int> colList;
            for (int c = 0; c < centerNum; ++c) {
                colList.push_back(c+companyNum);
            }
            for (int k = companyNum; k < centerNum + companyNum; ++k) {
                int randomCol = randomInterval(0, (unsigned int)colList.size() - 1);
                auto colIter = colList.begin();
                advance(colIter, randomCol);
                if (distance2[*rowIter][*colIter - companyNum] == 0) {
                    population[i].transportAmount[*rowIter][*colIter] = 0;
                    colList.erase(colIter);
                    continue; //不为自己分配运输量
                }
                int temp = randomInterval(0, (unsigned int)tempTotalAmount[*rowIter]);
                population[i].transportAmount[*rowIter][*colIter] = temp;
                //更新余量
                tempTotalAmount[*rowIter] -= temp;
                tempTotalAmount[*colIter - companyNum] += temp;
                colList.erase(colIter);
            }
            rowList.erase(rowIter);
        }
        //格式化转运方案
        for (int j = 0; j < centerNum; ++j) {
            for (int k = companyNum; k < companyNum + centerNum; ++k) {
                if (population[i].transportAmount[j][k] > population[i].transportAmount[k - companyNum][j+companyNum]) {
                    population[i].transportAmount[j][k] -= population[i].transportAmount[k - companyNum][j+companyNum];
                    population[i].transportAmount[k - companyNum][j+companyNum] = 0;
                } else {
                    population[i].transportAmount[k - companyNum][j+companyNum] -= population[i].transportAmount[j][k];
                    population[i].transportAmount[j][k] = 0;
                }
            }
        }
        //运输方案
        vector<DistanceInfo> distInfoVector;
        sortDistance(distInfoVector);
        for (int k = 0; k < distInfoVector.size(); ++k) {
            DistanceInfo distInfo = distInfoVector.at(k);
            int row = distInfo.row;
            int col = distInfo.col;
            int minVal = minFunc(tempTotalAmount[row], tempDemandeAmount[col]);
            population[i].transportAmount[row][col] = minVal;
            tempDemandeAmount[col] -= minVal;
            tempTotalAmount[row] -= minVal;
        }
//        printPopulation(i);
    }
}

void Function::crossover() {

    numOfOffspring = 0;
    for (int i = 0; i < populationSize; ++i) {//copy parent to offspring
        offspring[numOfOffspring] = population[i];
        ++numOfOffspring;
    }

    //crossover
    Individual tempA, tempB;
    for (int j = 0; j < populationSize; ++j) {
        int random1 = randomInterval(0, populationSize-1);
        int random2 = randomInterval(0, populationSize-1);
        while (random1 == random2) {
            random2 = randomInterval(0, populationSize - 1);
        }
        if (random0To1() <= 1) {
            tempA = population[random1];
            tempB = population[random2];
//            printPopulation(random1);
//            printPopulation(random2);
            TPX(tempA, tempB);
        }
    }
}

void Function::mutate() {
    for (int i = 0; i < numOfOffspring; ++i) {
        if(random0To1() < PM ) {

//            printIndividual(offspring[i]);

            int row1 = randomInterval(0, centerNum - 1);
            int row2 = randomInterval(0, centerNum - 1);
            int rowFrom = 0, rowTo = 0;
            if (row1<row2) {
                rowFrom =  row1;
                rowTo = row2;
            } else{
                rowFrom  =  row2;
                rowTo = row1;
            }
            int rowLen = rowTo - rowFrom + 1;
            int partialTotalAmount[rowLen];
            for (int j = 0; j < rowLen; ++j) {
                //计算转运中心总存储量
                int rowAmount = 0;
                for (int k = 0; k < companyNum; ++k) {
                    rowAmount += offspring[i].transportAmount[rowFrom+j][k];
                }
                partialTotalAmount[j] = rowAmount;
            }
            int partialDemandAmount[companyNum];
            for (int l = 0; l < companyNum; ++l) {
                int currentAmount = 0;
                for (int j = 0; j < centerNum; ++j) {
                    if (j >= rowFrom && j<= rowTo) continue;
                    else {
                        currentAmount += offspring[i].transportAmount[j][l];
                    }
                }
                partialDemandAmount[l] = demandedAmount[l] - currentAmount;
            }
            //重新分配运输方案
            vector<DistanceInfo> distInfoVector;
            sortDistance2(distInfoVector, rowFrom, rowLen);
            for (int k = 0; k < distInfoVector.size(); ++k) {
                DistanceInfo distInfo = distInfoVector.at(k);
                int row = distInfo.row;
                int col = distInfo.col;
                int minVal = minFunc(partialTotalAmount[row], partialDemandAmount[col]);
                offspring[i].transportAmount[row+rowFrom][col] = minVal;
                partialDemandAmount[col] -= minVal;
                partialTotalAmount[row] -= minVal;
            }
//            printIndividual(offspring[i]);
        }
    }
}

void Function::select() {
    for (Individual& temp : offspring) {
        calculateCost(temp);
        calculateFitness(temp);
    }
    //将elite直接加入下代
    sort(offspring, offspring+numOfOffspring, cmmmp);
    for (int i = 0; i < elite; ++i) {
        population[i] = offspring[i];
//        printIndividual(offspring[i]);
    }
    roulette(populationSize - elite);
}//end for select

void Function::print() {
    int minValue = 999;
    for (Individual temp : population) {
        if(temp.cost < minValue) minValue = temp.cost;
//        cout << "总距离: " << temp.cost  << endl;
//        cout << "运输方案: " << endl;
//        for (int j = 0; j < centerNum; ++j) {
//            for (int k = 0; k < centerNum + companyNum; ++k) {
//                cout << temp.transportSign[j][k] << " ";
//            }
//            cout << endl;
//        }
//        cout << "===============" << endl;
    }
    cout << "最短距离: " << minValue <<endl;
}//end for print
void Function::printPopulation(int i) {
    for (int j = 0; j < centerNum; ++j) {
        for (int k = 0; k < centerNum + companyNum; ++k) {
            cout << population[i].transportAmount[j][k] << "   ";
        }
        cout << endl;
    }
    cout << "===============" << endl;
}
void Function::printIndividual(Individual individual) {
    cout<<"Individual======"<<endl;
    for (int j = 0; j < centerNum; ++j) {
        for (int k = 0; k < centerNum + companyNum; ++k) {
            cout << individual.transportAmount[j][k] << "   ";
        }
        cout << endl;
    }
    cout << "Individual END=============== cost: " << individual.cost << endl;
}

void Function::calculateFitness(Individual &target) {
    target.fitness = max_num - target.cost;

}

void Function::calculateCost(Individual &target) {
    target.cost = 0;
    for (int i = 0; i < centerNum; ++i) {
        for (int j = 0; j < centerNum + companyNum; ++j) {
            if (target.transportAmount[i][j] > 0) {
                if(j<companyNum)
                    target.cost += distance1[i][j];
                else
                    target.cost += distance2[i][j-companyNum];
            }
        }
    }
}


void Function::TPX(Individual &parentA, Individual &parentB) {
    Individual child;
    for (int m = 0; m < centerNum; ++m) {
        for (int i = 0; i < centerNum + companyNum; ++i) {
            child.transportAmount[m][i] = 0;
        }
    }
    int flag1[centerNum]; // true为向下取整
    int preflag = 0;// -1为向下取整，1为向上取整，0为可以整除
    for (int k = 0; k < centerNum; ++k) {
        flag1[k] = 0;
    }
    for (int i = 0; i < centerNum; ++i) {
        preflag = 0;
        for (int j = 0; j < companyNum; ++j) {
            int tempA = parentA.transportAmount[i][j];
            int tempB = parentB.transportAmount[i][j];
            if((tempA + tempB ) % 2 == 1) {
                if (flag1[j] == 0) {
                    if (preflag == 0 || preflag == 1) {
                        child.transportAmount[i][j] = (int) ceil(((double)tempA+tempB)/2);
                        flag1[j] = -1;
                        preflag = -1;
                    } else {
                        flag1[j] = 1;
                        preflag = 1;
                        child.transportAmount[i][j] = (int) floor(((double)tempA+tempB)/2);
                    }
                } else if (flag1[j] == -1){
                    flag1[j] = 1;
                    preflag = 1;
                    child.transportAmount[i][j] = (int) floor(((double)tempA+tempB)/2);
                } else if (flag1[j] == 1) {
                    child.transportAmount[i][j] = (int) ceil(((double)tempA+tempB)/2);
                    flag1[j] = -1;
                    preflag = -1;
                }
            } else {
                child.transportAmount[i][j] = (tempA + tempB) / 2;
            }
        }

    }
    // 贪心重新求解转运量
    int nowAmount[centerNum] = {0}; //记录每个转运中心的现有量。确定供需关系。
    for (int k = 0; k < centerNum; ++k) {
        nowAmount[k] += totalAmount[k];
        for (int j = 0; j < companyNum; ++j) {
            nowAmount[k] -= child.transportAmount[k][j];
        }
    }
    vector<int> requirement; //有需求的中心库
    vector<int> supplyment; //供应的中心库
    for (int l = 0; l < centerNum; ++l) {
        if (nowAmount[l] < 0) {
            requirement.push_back(l);
            nowAmount[l] = 0-nowAmount[l];
        } else {
            supplyment.push_back(l);
        }
    }
    // 根据distance2实现一个距离矩阵
    vector<DistanceInfo> distanceInfo;
    int tempDistance[supplyment.size()][requirement.size()];
    for (int n = 0; n < supplyment.size(); ++n) {
        for (int j = 0; j < requirement.size(); ++j) {
            int row = supplyment.at(n);
            int col = requirement.at(j);
            tempDistance[n][j] = distance2[row][col];
            DistanceInfo tempInfo;
            tempInfo.distance = distance2[row][col];
            tempInfo.row = n;
            tempInfo.col = j;
            tempInfo.realRow = row;
            tempInfo.realCol = col;
            distanceInfo.push_back(tempInfo);
        }
    }
    //贪心分配转运
    sort(distanceInfo.begin(), distanceInfo.end(), cmmp);
    for ( DistanceInfo temp : distanceInfo) {
        int minVal = minFunc(nowAmount[supplyment.at(temp.row)] ,nowAmount[requirement.at(temp.col)]);
        nowAmount[supplyment.at(temp.row)] -= minVal;
        nowAmount[requirement.at(temp.col)] -= minVal;
        child.transportAmount[temp.realRow][temp.realCol+companyNum] = minVal;
    }
    //格式化转运信息。
    formateTransport(child);
    offspring[numOfOffspring] = child;
    ++numOfOffspring;
//    printIndividual(child);
}


void Function::checkPopulation() {
    for (int i = 0; i < numOfOffspring; ++i) {
        Individual temp = offspring[i];

        int rowAmount[centerNum];
        int amount = 0;
        for (int j = 0; j < centerNum; ++j) {
            for (int l = 0; l < centerNum; ++l) {
                amount = amount + temp.transportAmount[l][j + companyNum] - temp.transportAmount[j][l + companyNum];
            }
            rowAmount[j] = amount + totalAmount[j];
            amount = 0;
        }

        for (int m = 0; m < centerNum; ++m) {
            int transAmout= 0;
            for (int j = 0; j < companyNum; ++j) {
                transAmout += offspring[i].transportAmount[m][j];
            }
            if(rowAmount[m] < transAmout) {
                cout<<"distribution ERROR" << endl;
                printIndividual(offspring[i]);
            }
        }
        for (int n = 0; n < companyNum; ++n) {
            int transAmount = 0;
            for (int j = 0; j < centerNum; ++j) {
                transAmount += offspring[i].transportAmount[j][n];
            }
//            if (demandedAmount[n] != transAmount) {
//                cout<<"distribution ERROR" << endl;
//                printIndividual(offspring[i]);
//            }
        }
    }
}

//other operation
void Function::roulette(int rouletteNum) {

    int sum = 0;
    double proportion[numOfOffspring];
    for (int j = 0; j < numOfOffspring; ++j) {
        sum += offspring[j].fitness;
    }
    //计算每个个体占轮盘的比重。
    proportion[0] = (double)offspring[0].fitness/sum;
    for (int k = 1; k < numOfOffspring; ++k) {
        proportion[k] = proportion[k-1] + (double)offspring[k].fitness/sum;
    }
    for (int i = 0; i < rouletteNum; ++i) {
        double randomNum = random0To1();
        int k = 0;
        while( k < numOfOffspring ) {
            if ( randomNum < proportion[k]) {
                population[elite+i] = offspring[i];
                break;
            }
            ++k;
        }
    }
}
void Function::formateTransport (Individual &individual) {
    for (int j = 0; j < centerNum; ++j) {
        for (int k = companyNum; k < companyNum + centerNum; ++k) {
            if (individual.transportAmount[j][k] > individual.transportAmount[k - companyNum][j+companyNum]) {
                individual.transportAmount[j][k] -= individual.transportAmount[k - companyNum][j+companyNum];
                individual.transportAmount[k - companyNum][j+companyNum] = 0;
            } else {
                individual.transportAmount[k - companyNum][j+companyNum] -= individual.transportAmount[j][k];
                individual.transportAmount[j][k] = 0;
            }
        }
    }
}


