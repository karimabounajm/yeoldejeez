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

// so because we are using a 2d array of ints to describe the current best collisions and an array of 
// transformations of the x value alone to describe the current best path, it is difficult to identify 
// what coordinates are on the current best path for the heuristic to reject the path. because of this, 
// we will set all values in the current best path's coordinates in the bestCol 2d array to -(best value), which can
// easily be checked at every node. if a node is negative, the algorithm will know that it is on the best path. then 
// we can very simply just compare the current #collisions in the current path with the absolute value of the #collisions
// in the best path. replace accordingly
    // a minor problem that pops up from this is nmaking the coordinates of the current path negative if it becomes a new 
    // best path. we will have to iterate backwards from here, probably make a helper function to do so. make all points
    // no longer in the best path positive, make all new points negative
// so in practice, bestnum is 0 if it hasn't been reached yet, ie hasn't been explored down a path; positive if it is just a
// normal point, and negative if it is a point in the set of best coordinates


struct pathWay
{
    int curHeight; // well be modified based on the x-transformation, given that ∆height is difference between speed and x change
    int path[]; // plan is to have x transformations in order of transformation, as from the height of the previous instance and the x transformation
    // we can find out how much the height changed (explained just above)
};



// add an extra layer to the start of the string that contains the array of the map. that way, we can transition over to
// beginning of the real map, that we can avoid making the recursive function more complicated

// phase out struct, use global variable
// speed can be used to use 1d arraya for coordinate changes
// y = y - abs(max - dx)


struct gameValues* initializeGameValues()
{
    struct gameValues* inIT = malloc(sizeof(struct gameValues));

    inIT->width = 0; inIT->height = 0; // set to zero so that they can be modified in a helper function

    printf("Speed: ");
    scanf("%d", &(inIT->speed));

    inIT->numTrans = 2 * (inIT->speed) - 1; 
    inIT->arrTrans = malloc(sizeof(int) * inIT->numTrans);

    for (int i = 1 - inIT->speed; i < inIT->speed; i++)
    {
        // printf("The index is %d and the transformation is %d\n", inIT->speed - i - 1, i);
        inIT->arrTrans[inIT->speed - i] = i;
    }

    // for (int i = 1; i <= inIT->numTrans; i++)
    // {
    //     printf("This transformation is %d\n", inIT->arrTrans[i]);
    // }

    return inIT;
}


char** createBoard(char* filename, struct gameValues* inIT)
{
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    char gameBoard[][150];
    printf("we have entered the rage modewww\n");
    fgets(gameBoard[0], 150, file); 
    printf("we have entered the rage modewww\n");
    inIT->width = strlen(gameBoard[0]) - 1;

    int sizeLine = (sizeof(char) * (inIT->width + 1));
    int Height = inIT->height; int Width = inIT->width;

    gameBoard[0][inIT->width - 1] = '\0';


    
    printf("we have entered the rage modewww\n");
    // gameBoard[Height] = malloc(sizeof(char) * Width);
    printf("we have entered the rage mode\n");
    // gameBoard[0] = "this should work";
    printf("String is supposed to be: %s\n", gameBoard[0]);

    // strcpy(bufLine, (gameBoard)[Height]); 

    // printf("number of bytes is %lu\n", (sizeof(char) * inIT->width));

    // (gameBoard[0]) = malloc(sizeLine);

    // strcpy(bufLine, gameBoard[Height]); 
    // printf("number of bytes is %lu\n", (sizeof(char) * inIT->width));

    // (inIT->bestCol[Height]) = malloc(sizeof(int) * inIT->width);
    // memset(inIT->bestCol[Height], 0, inIT->width);
    
    // (Height)++; 


    // while(fgets(bufLine, 150, file))
    // {
    // (gameBoard[Height]) = malloc(sizeof(char) * inIT->width);
    // bufLine[inIT->width - 1] = '\0';

    // strcpy(bufLine, gameBoard[Height]); 
    // printf("The current string is %s\n", bufLine);

    // (inIT->bestCol[Height]) = malloc(sizeof(int) * inIT->width);
    // memset(inIT->bestCol[Height], 0, inIT->width);
    
    // (Height)++; 
    // }

    // inIT->height = Height;
    fclose(file);

    return gameBoard;
}


int main()
{
    // printf("help me out here bro\n");

    struct gameValues* gV = initializeGameValues();
    createBoard("input.txt", gV);
}       
