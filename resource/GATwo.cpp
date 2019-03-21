//
// Created by 王慧 on 2019-03-18.
//

#include <array>
#include "GATwo.h"
#include "../header/Tool.h"
#include <Eigen/LU>
#include <Eigen/Dense>
#include <iostream>

using namespace GATwoSpace;
using namespace Eigen;

void GATwo::evolve() {
    initialize();
    for (int i = 0; i < generation; ++i) {
        crossover();
        mutate();
        select();
        std::cout <<  "generation-" << i << "  ";
    }
    std::cout << "========over=========" << std::endl;
    std::cout << population[0].cost << std::endl;
    printIndividualTwo();
    soluteEquation(offspring[0]);
}
void GATwo::initialize() {
    for (int i = 0; i < populationSize; ++i) {
        for (int j = 0; j < centerNum; ++j) {
            for (int k = 0; k < companyNum + centerNum; ++k) {
                //对角位置只有一个为1
                if (Tool::random0To1Two() < 0.5){
                    population[i].transportSign[j][k] = 0;
                } else {
                    if (k - companyNum == j) {
                        population[i].transportSign[j][k] = 0;
                    } else if (population[i].transportSign[k-companyNum][j+companyNum] == 1){
                        population[i].transportSign[j][k] = 0;
                    } else {
                        population[i].transportSign[j][k] = 1;
                    }
                }
            }
        }
    }
}
void GATwo::crossover() {
    offspringNum = 0;
    for (int i = 0; i < populationSize; ++i) {
        offspring[offspringNum] = population[i];
        ++offspringNum;
    }
    IndividualTwo tempA, tempB;
    for (int j = 0; j < populationSize; ++j) {
        int random1 = Tool::randomIntervalTwo(0, populationSize-1);
        int random2 = Tool::randomIntervalTwo(0, populationSize-1);
        while (random1 == random2) {
            random2 = Tool::randomIntervalTwo(0, populationSize - 1);
        }
        if (Tool::random0To1Two() <= PC) {
            tempA = population[random1];
            tempB = population[random2];
//            IndividualTwo child;
//            for (int i = 0; i < centerNum; ++i) {
//                for (int k = 0; k < companyNum + centerNum; ++k) {
//                    child.transportSign[i][k] = tempA.transportSign[i][k]^tempB.transportSign[i][k];
//                }
//            }
            twoPointCross(tempA, tempB);
            offspring[offspringNum] = tempA;
            ++offspringNum;
            offspring[offspringNum] = tempB;
            ++offspringNum;
        }
    }
}
void GATwo::twoPointCross(IndividualTwo &t1, IndividualTwo &t2) {
    int randomRow1 = Tool::randomIntervalTwo(0, centerNum);
    int randomRow2 = Tool::randomIntervalTwo(0, centerNum);
    if (randomRow1 > randomRow2) {
        int temp = randomRow1;
        randomRow1 = randomRow2;
        randomRow2 = temp;
    }
    int randomCol1 = Tool::randomIntervalTwo(0, companyNum);
    int randomCol2 = Tool::randomIntervalTwo(0, companyNum);
    if (randomCol1 > randomCol2) {
        int temp = randomCol1;
        randomCol1 = randomCol2;
        randomCol2 = temp;
    }
    for (int i = randomRow1; i <= randomRow2 ; ++i) {
        for (int j = randomCol1; j <= randomCol2; ++j) {
            int temp = t1.transportSign[i][j];
            t1.transportSign[i][j] = t2.transportSign[i][j];
            t2.transportSign[i][j] = temp;
        }
    }
    //交换转运部分
    for (int i = 0; i < centerNum; ++i) {
        for (int j = companyNum; j < centerNum+companyNum; ++j) {
            int temp = t1.transportSign[i][j];
            t1.transportSign[i][j] = t2.transportSign[i][j];
            t2.transportSign[i][j] = temp;
        }
    }
}
void GATwo::mutate() {
    for (IndividualTwo& individual : offspring) {
        for (int i = 0; i <  centerNum ; ++i) {
            for (int j = 0; j < centerNum+companyNum; ++j) {
                if( Tool::random0To1Two() < PM) {
                    if(individual.transportSign[i][j] == 0){
                        if (j >= companyNum && population[i].transportSign[j-companyNum][i+companyNum] == 1) {
                            continue;
                        }
                        else if (i == (j-companyNum)) {
                            individual.transportSign[i][j] = 0;
                        }
                        else {
                            individual.transportSign[i][j] = 1;
                        }
                    } else {
                        if (j >= companyNum && population[i].transportSign[j-companyNum][i+companyNum] == 0) {
                            continue;
                        } else {
                            individual.transportSign[i][j] = 0;
                        }
                    }
                }
            }
        }
    }
}

void GATwo::caculateCost() {
    for (auto& individual : offspring) {
        individual.cost = 0;
        int rowNum = companyNum + centerNum;
        int transRowNum = rowNum;
        int colNum = centerNum*(centerNum + companyNum);
        MatrixXd m(rowNum, colNum);
        VectorXd v(rowNum);
        for (int i = 0; i < centerNum; ++i) {
            v(i) = totalAmount[i];
        }
        for (int j = centerNum; j < transRowNum; ++j) {
            v(j) = demandedAmount[j-centerNum];
        }
        MatrixXd augmentedMatrix(rowNum, colNum+1);
        for (int i = 0; i < rowNum; ++i) {
            if (i < centerNum) {
                for (int j = 0; j < centerNum; ++j) {
                    for (int k = 0; k < transRowNum; ++k) {
                        if (i == j) {
                            m(i, j*transRowNum+k) = individual.transportSign[j][k];
                        } else {
                            if (k == i + companyNum) {
                                m(i, j*transRowNum+k) = -individual.transportSign[j][k];
                            } else {
                                m(i, j*transRowNum+k) = 0;
                            }
                        }
                    }
                }
            } else {
                for (int j = 0; j < centerNum; ++j) {
                    for (int k = 0; k < transRowNum; ++k) {
                        if ((i-centerNum) == k) {
                            m(i, j*transRowNum+k) = individual.transportSign[j][k];
                        } else {
                            m(i, j*transRowNum+k) = 0;
                        }
                    }
                }
            }
        }
        augmentedMatrix << m, v;
        FullPivLU<MatrixXd> lu_decomp1(m);
        auto rank1 = lu_decomp1.rank();
        FullPivLU<MatrixXd> lu_decomp2(augmentedMatrix);
        auto rank2 = lu_decomp2.rank();
        //计算运输距离
        for (int i = 0; i < centerNum; ++i) {
            for (int j = 0; j < companyNum; ++j) {
                if (individual.transportSign[i][j] == 1) {
                    individual.cost += distance1[i][j];
                }
            }
        }
        for (int i = 0; i < centerNum; ++i) {
            for (int j = companyNum; j < transRowNum; ++j) {
                if (individual.transportSign[i][j] == 1) {
                    individual.cost += distance2[i][j-companyNum];
                }
            }
        }
        //判断是否有解
        if (rank1 != rank2) {
            individual.cost += penalty;
        } else {
            MatrixXd x(colNum,1);
            x = m.fullPivHouseholderQr().solve(v);
            x.resize( rowNum,centerNum);
            x.transpose();
            for (int i = 0; i < centerNum; ++i) {
                for (int j = 0; j < rowNum; ++j) {
                    if(j < companyNum && x(i,j) < 0)  {
                        individual.cost += 6;
                    }
                }
            }
            int k = 0;
            for (int i = 0; i < colNum; ++i) {
//                if (x(i) < 0) {
//                    individual.cost += 3;
//                    break;
//                }


                //TODO 动态计算该罚值，1->a->2则需要加上1->2距离和a->2距离的差值（即罚值）。
                int dynamicPenalty = 3;
                if(i < (k*rowNum+ companyNum)) {
                    if (x(i) < 0){
                        individual.cost += 3;
                        break;
                    }
                } else {
                    i += centerNum;
                    ++k;
                }
            }
        }
        individual.fitness = max_num - individual.cost;
    }
}
void GATwo::select() {
    caculateCost();
    //将elite直接加入下代
    std::sort(offspring, offspring+offspringNum, Tool::cmpIndividualFitness);
    for (int i = 0; i < elite; ++i) {
        population[i] = offspring[i];
    }
    std::cout << population[0].cost << std::endl;
    rouletteAlg(populationSize - elite);
}

void GATwo::rouletteAlg(int rouletteNum) {

    int sum = 0;
    double proportion[offspringNum];
    for (int j = 0; j < offspringNum; ++j) {
        sum += offspring[j].fitness;
    }
    //计算每个个体占轮盘的比重。
    proportion[0] = (double)offspring[0].fitness/sum;
    for (int k = 1; k < offspringNum; ++k) {
        proportion[k] = proportion[k-1] + (double)offspring[k].fitness/sum;
    }
    for (int i = 0; i < rouletteNum; ++i) {
        double randomNum = Tool::random0To1Two();
        int k = 0;
        while( k < offspringNum ) {
            if ( randomNum < proportion[k]) {
                population[elite+i] = offspring[i];
                break;
            }
            ++k;
        }
    }
}

void GATwo::printIndividualTwo() {
    for (int i = 0; i < centerNum; ++i) {
        for (int j = 0; j < centerNum + companyNum; ++j) {
            std::cout << offspring[0].transportSign[i][j] << "   ";
        }
        std::cout << std::endl;
    }
}

void GATwo::soluteEquation(IndividualTwo individual) {
    int rowNum = companyNum + centerNum;
    int transRowNum = rowNum;
    int colNum = centerNum*(centerNum + companyNum);
    MatrixXd m(rowNum, colNum);
    VectorXd v(rowNum);
    for (int i = 0; i < centerNum; ++i) {
        v(i) = totalAmount[i];
    }
    for (int j = centerNum; j < transRowNum; ++j) {
        v(j) = demandedAmount[j-centerNum];
    }
    for (int i = 0; i < rowNum; ++i) {
        if (i < centerNum) {
            for (int j = 0; j < centerNum; ++j) {
                for (int k = 0; k < transRowNum; ++k) {
                    if (i == j) {
                        m(i, j*transRowNum+k) = individual.transportSign[j][k];
                    } else {
                        if (k == i + companyNum) {
                            m(i, j*transRowNum+k) = -individual.transportSign[j][k];
                        } else {
                            m(i, j*transRowNum+k) = 0;
                        }
                    }
                }
            }
        } else {
            for (int j = 0; j < centerNum; ++j) {
                for (int k = 0; k < transRowNum; ++k) {
                    if ((i-centerNum) == k) {
                        m(i, j*transRowNum+k) = individual.transportSign[j][k];
                    } else {
                        m(i, j*transRowNum+k) = 0;
                    }
                }
            }
        }
    }

    ColPivHouseholderQR<MatrixXd> dec(m);
    MatrixXd x(32,1);
    x = dec.solve(v);
    x.resize(8,4);
    std::cout << x.transpose() << std::endl;

//    MatrixXd y(32,1);
//    y = m.fullPivHouseholderQr().solve(v);
//    y.resize(4,8);
//    std::cout << y << std::endl;
}
















