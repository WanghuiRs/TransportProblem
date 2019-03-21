
#ifndef _DATA_H_
#define  _DATA_H_

#define MAX_NUM 99999;
static const int centerStockNum = 4;   //中心库个数
static const int itermCompanyNum = 4;  //项目公司个数
static int distData[centerStockNum + 1][centerStockNum + itermCompanyNum ] =
        {{4, 12, 8, 11, 6, 11, 9, 14},
         {2,10,5,9,0,9,7,5},
         {8,5,11,6,9,0,7,8},
         {7,9,7,3,7,7,0,16},
         {8,14,12,14}
         };

typedef struct {

    int distace;
    int row;
    int col;
    int no;
    int storage;

} Data;

typedef struct {

    int no;
    int storage;
    int distance;

} centerInfo;

void readFromFile();
#endif