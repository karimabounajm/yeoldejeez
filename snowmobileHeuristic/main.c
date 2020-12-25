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

    return inIT;
}


    // for (int i = 1; i <= inIT->numTrans; i++)
    // {
    //     printf("This transformation is %d\n", inIT->arrTrans[i]);
    // }



char** createBoard(char* filename, struct gameValues* inIT)
{
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    // finding the sizes of arrays to avoid having to calculate every time
    int malPointer = sizeof(char*) * 350; 
    int malRow = sizeof(char) * 200;

    // defining the board array and its first row, allocating memory for each
    char** gameBoard = malloc(malPointer);
    gameBoard[inIT->height] = malloc(malRow);

    // reading the first line from the file
    fgets(gameBoard[inIT->height], 150, file); 

    // recording the number of characters in each line of the file, and setting 
    // height to easily accessible variable; note, is one because we did first row
    inIT->width = strlen(gameBoard[0]) - 1;
    int Height = 1; 

    while(42)
    {
        gameBoard[Height] = malloc(malRow);
        if(!fgets(gameBoard[Height], 150, file))
            break;
        printf("Line %d is %s", Height, gameBoard[Height]);
        Height++;    
    }

    inIT->height = Height;
    fclose(file);

    return gameBoard;
}


int main()
{
    // printf("help me out here bro\n");

    struct gameValues* gV = initializeGameValues();
    createBoard("input.txt", gV);
}       
