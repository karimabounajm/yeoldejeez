#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// want to create test cases to check speed and make sure that the code is functional
// run these test cases with traditional recursive methods and the heuristics
// identify an expected output in advance
// thank you Kaloti my man, ecs 32b ss2 coming in clutch with the "setting ourselves apart" recommendation

struct gameValues
{
    // from the txt file
    int height, width; 
    char** board;

    // from the user
    int numTrans, speed;
    int* arrTrans;
    
    // array of least number of collisions by point
    int bestCol[][];
    
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

}


struct gameValues* getTransformations()
{ 
    struct gameValues* values = malloc(sizeof(struct gameValues));

    int maxVal;
    printf("How fast do you want to go?\n Write here:");
    scanf("%d", maxVal);

    values->numTrans = 2 * maxVal - 1;

    int arrNeed = values->numTrans * 2; 
    values->arrTrans = malloc(sizeof(int) * arrNeed);

    int horiShift = 1 - maxVal;
    int vertiShift = -1;

    for (int i = 0; i < arrNeed; i++)
    {
        values->arrTrans[i] = horiShift;
        values->arrTrans[i+1] = vertiShift;

        horiShift++;
        vertiShift = -(maxVal - abs(horiShift));
    }
    
    return values;

}


int main()
{
    int* potato;
}       
