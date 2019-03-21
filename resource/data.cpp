#include<fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include"../header/data.h"


using namespace std;

//Factory center[center + 1];

void readFactory()
{

}

bool cmp(Data a, Data b){
    return a.distace < b.distace;
}

void initData(int centerStockNum, int itermCompanyNum, vector<Data> &data, vector<vector<int>> distanceData){

    int k = 0;
    for (int i = 0; i < centerStockNum; ++i) {
        for (int j = 0; j < itermCompanyNum; ++j) {
            data[k].distace = distanceData[i][j];
            data[k].row = i;
            data[k].col = j;
            ++k;
        }
    }
    // 根据距离排序，决定配送的顺序。
    sort(data.begin(), data.end(), cmp);
}

int algOne(int centerStockNum, int itermCompanyNum, vector<vector<int>> distanceData, bool isRepeatDelivery) {

    int totalDistance = 0;
    int stockLoca = centerStockNum+itermCompanyNum - 1;
    //初始化数据
    int len = centerStockNum*itermCompanyNum;
    vector<Data> data(len);
    initData(centerStockNum, itermCompanyNum, data, distanceData);
    //算法二中被配送过的中心不再被配送。
    vector<int> center(centerStockNum);
    for (int i = 0; i < len; i++) {
        Data temp = data[i];
        //判断该公司是否还需要配送
        if ((!isRepeatDelivery && center[temp.col] == 1) ||distanceData[centerStockNum][temp.col] <= 0 || distanceData[temp.row][stockLoca] <= 0)
            continue;
        //配送，并更新距离、库存和需求量
        totalDistance += temp.distace;
        center[temp.col] = 1;
        cout << temp.distace << " ";
        int stockAmount = distanceData[temp.row][stockLoca]; //中心仓库库存量
        int needAmount = distanceData[centerStockNum][temp.col];               //公司需求量
        //需求量小于库存量
        if (needAmount < stockAmount) {
            distanceData[temp.row][stockLoca] -= needAmount;
            distanceData[centerStockNum][temp.col] = 0;
        } else {
            distanceData[centerStockNum][temp.col] -= stockAmount;
            distanceData[temp.row][stockLoca] = 0;
        }
    }
    cout << endl;
    cout << totalDistance << endl;
    return totalDistance;

}

void algTwo() {

    int totalDistance = 0;
    int stockLoca = centerStockNum + itermCompanyNum - 1;

    //
    int company[itermCompanyNum] = {0};
    int center[centerStockNum];
    //初始化数据
    int len = centerStockNum * itermCompanyNum;
    Data data[len];
    int k = 0;
    for (int i = 0; i < centerStockNum; ++i) {
        for (int j = 0; j < itermCompanyNum; ++j) {
            data[k].distace = distData[i][j];
            data[k].row = i;
            data[k].col = j;
            ++k;
        }
    }
    // 根据距离排序
    sort(data, data + len, cmp);

    k = 0; //项目公司的编号
    for (int l = 0; l < len; ++l) {
        Data temp = data[l];
        //项目公司暂时没有分配仓库中心
        if (company[temp.col] == 0) {
            totalDistance += distData[temp.row][temp.col];
            distData[temp.row][stockLoca] -= distData[centerStockNum][temp.col];
            company[temp.col] = 1;
        } else
            continue;
    }
    // 计算中心仓库的需求量和剩余量
    vector<int> rowArr;
    vector<int> colArr;
    for (int i = 0; i < centerStockNum; i++) {
        int amount = distData[i][stockLoca];
        if (amount >= 0) {
            rowArr.push_back(i);
            center[i] = amount;
        } else {
            colArr.push_back(i);
            center[i] = amount;
        }
    }
    //构建供需距离矩阵
    int n =rowArr.size() + 1;
    int m = rowArr.size()+colArr.size();
    vector<vector<int>> newDist(n);
    for (int i = 0; i < newDist.size(); i++)
        newDist[i].resize(m);

    for (int i = 0; i < n; i++) {
        if (i == rowArr.size()) {
            for (int j = 0; j < colArr.size(); j++)
                newDist[i][j] = 0 - center[colArr[j]];
        } else {
            for (int j = 0; j < m; j++) {
                if (j == m - 1){
                    newDist[i][j] = center[rowArr[i]];
                }else if(j >= colArr.size()) {
                    newDist[i][j] = 0;
                }else{
                    int dist = MAX_NUM;
                    if (rowArr[i] != 0 && colArr[j] == 0)
                        dist = distData[colArr[j]][rowArr[i]+ - 1];
                    else
                        dist = distData[rowArr[i]][colArr[j]+itermCompanyNum - 1];
                    newDist[i][j] = dist;
                }
            }
        }
    }

    totalDistance += algOne(rowArr.size(), colArr.size(), newDist, false);
    cout << totalDistance << endl;
}


