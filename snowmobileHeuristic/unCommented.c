#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dain.h"



struct gameValues
{
    // from the txt file
    int height, width; 

    // from the user
    int numTrans, speed;
    int* arrTrans;
};



void deallocateGameValues(struct gameValues* gV)
{
    free(gV->arrTrans);
    free(gV);
}



struct pathWay
{
    int curHeight, numCollisions;
    int path[350]; 
};



void deallocatePathWay(struct pathWay* pathInst)
{
    free(pathInst->path);
    free(pathInst);
}



struct gameValues* initializeGameValues()
{
    struct gameValues* inIT = malloc(sizeof(struct gameValues));

    inIT->width = 0; inIT->height = 0; 

    printf("Speed: ");
    scanf("%d", &(inIT->speed));

    inIT->numTrans = 2 * (inIT->speed) - 1; 
    inIT->arrTrans = malloc(sizeof(int) * inIT->numTrans);

    for (int i = 1 - inIT->speed; i < inIT->speed; i++)
    {
        inIT->arrTrans[inIT->speed - i] = i;
    }

    return inIT;
}



char** createBoard(char* filename, struct gameValues* inIT)
{
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    int malPointer = sizeof(char*) * 350; 
    int malRow = sizeof(char) * 50;

    char** gameBoard = malloc(malPointer);
    gameBoard[inIT->height] = malloc(malRow);

    fgets(gameBoard[inIT->height], 150, file); 

    inIT->width = strlen(gameBoard[0]) - 1;
    int Height = 0; 

    while(++Height)
    {
        gameBoard[Height] = malloc(malRow);
        if(!fgets(gameBoard[Height], 150, file))
            break;
    }

    inIT->height = Height - 1; 

    fseek(file, 0, SEEK_SET);
    fclose(file);

    return gameBoard;
}



void deallocateBoard(char** gameBoard, int numRows)
{
    for(int i = 0; i <= numRows; i++) free(gameBoard[i]);
    free(gameBoard);
}



int** createMap(struct gameValues* inIT)
{
    int numRows = inIT->height;

    int** bestMap = malloc(sizeof(int*) * numRows);

    int speed = inIT->speed; int widthRow = 1;
    for(int i = 0;i <= numRows; i++)
    {
        bestMap[i] = malloc(sizeof(int) * widthRow);
        memset(bestMap[i], -351, widthRow);
        widthRow += 2 * (speed - 1);
    }

    return bestMap;
}



void deallocateMap(int** bestMap, int numRows)
{
    for(int i = 0;i <= numRows; i++) free(bestMap[i]);
    free(bestMap);
}



int translateCoordArr(int heightCur, int widthCur, int speed, int numRows)
{
    int mapIndex = widthCur + (numRows - heightCur) * (speed - 1);
    return mapIndex;
}



int translateCoordBoard(int boardWidth, int widthCur)
{

    if(widthCur >= boardWidth) return (widthCur % boardWidth);
    else return ((widthCur % boardWidth) + boardWidth);
}



struct pathWay* findInitial(char** gameBoard, int** bestMap, struct gameValues* inIT)
{
    struct pathWay* initialPath = malloc(sizeof(struct pathWay));

    int numRows = inIT->height; int extremaTrans = inIT->speed - 1;
    int numCollisions = 0; int curWidth = 0;

    for(int curHeight = 0; curHeight <= numRows; curHeight++)
    {
        if(gameBoard[curHeight][curWidth] == '#') numCollisions++;
        bestMap[curHeight][curWidth] = numCollisions;
        curWidth += extremaTrans;
        (initialPath->path)[curHeight] = curWidth;
    }

    initialPath->numCollisions = numCollisions;
    initialPath->curHeight = numRows;

    return initialPath;
}



int main()
{
    struct gameValues* gV = initializeGameValues();
    char** board = createBoard("input.txt", gV);
    int** map = createMap(gV);
    findInitial(board, map, gV);
}

