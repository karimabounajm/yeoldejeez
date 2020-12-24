#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include "dain.h"


struct gameValues
{
    // from the txt file
    int height, width; 
    char** board;

    // from the user
    int numTrans, speed;
    int* arrTrans;
    
    // array of least number of collisions by point
    int** bestCol;
    
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

// struct pa

// void getTransformations(struct gameValues* gV)
// { // get speed spit out array of possible transformations

//     printf("Speed: ");
//     scanf("%d", gV->speed);

//     gV->numTrans = 2 * (gV->speed) - 1; 
//     int arrNeed = gV->numTrans * 2; //
//     gV->arrTrans = malloc(sizeof(int) * arrNeed);

//     int horiShift = 1 - maxVal;
//     int vertiShift = -1;

//     for (int i = 0; i < arrNeed; i++)
//     {
//         gV->arrTrans[i] = horiShift;
//         gV->arrTrans[i+1] = vertiShift;

//         horiShift++;
//         vertiShift = -(maxVal - abs(horiShift));
//     }
    
//     return gV;

// }


struct gameValues* initializeGameValues(char* filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    struct gameValues* gV = malloc(sizeof(struct gameValues));

    gV->width = 0; gV->height = 0;
    char* bufLine; 

    fgets(bufLine, 150, file); gV->width = strlen(bufLine);

    (gV->board[gV->height]) = malloc(sizeof(char) * gV->width);
    strcpy(bufLine, gV->board[gV->height]); 

    (gV->bestCol[gV->height]) = malloc(sizeof(int) * gV->width);
    memset(gV->bestCol[gV->height], 0, gV->width);
    
    (gV->height)++; 

    while(fgets(bufLine, 150, file))
    {
    (gV->board[gV->height]) = malloc(sizeof(char) * gV->width);
    strcpy(bufLine, gV->board[gV->height]); 

    (gV->bestCol[gV->height]) = malloc(sizeof(int) * gV->width);
    memset(gV->bestCol[gV->height], 0, gV->width);
    
    (gV->height)++; 
    }

    printf("Speed: ");
    scanf("%d", &(gV->speed));

    gV->numTrans = 2 * (gV->speed) - 1; 
    gV->arrTrans = malloc(sizeof(int) * gV->numTrans);

    for (int i = 1 - gV->speed; i < gV->speed; i++)
    {
        gV->arrTrans[gV->speed - i] = i;
    }

    return gV;
}

// remember, file must be read in a way that allows it to be printed in a presentable way
// determined that it must be rectangular, with width 2* the height


// void getTransformations(struct gameValues* inIT);
// {

// }






// write initial for loop for going down extrema. this will record: number of collisions at each point, number of collisions in total, and path
//  note, remember to update current best path, but take it initially as the extrema path
// create another for loop, decremented from the original for loop, and for each node going up the path (because for loop is used, first coordinate
//      is not last, instead last is last)
// while going up the path in the decremeneting for loop, each node has its paths recursively gone down. implement heuristic described below

// create a recursive helper function without all the setup involved in the function that encapsulates it

// for the 2d array for collisions at each point, it does not need to be square like the gameboard, instead having length @row of row*(speed - 1)


// recursive function in a for loop, with a starting node being at the new ghost one and it iterating
// through every possible transformation; the base case to end is if the y coord is 0, but there must be speed - 1 base cases
// if position is x = speed - 1 away from the bottom, then there are (speed  * 2) - 1 transformation that can't be performed


// case 1 if aat bottom
// case 2 is if current collisions exceeds previous minimum

int main()
{
    int* potato;
}       
