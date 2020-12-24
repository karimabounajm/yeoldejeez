#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct gameValues
{
    int height, width, speed;

    int numTrans;
    int* arrTrans;
    
    char board[height][width];
    char** bestCol;
};

struct pathWay
{

}

// add an extra layer to the start of the string that contains the array of the map. that way, we can transition over to
// beginning of the real map, that we can avoid making the recursive function more complicated

// phase out struct, use global variable
// speed can be used to use 1d arraya for coordinate changes
// y = y - abs(max - dx)

// struct pa

struct gameValues* getTransformations()
{ // get speed spit out array of possible transformations
    struct gameValues* values = malloc(sizeof(struct gameValues));

    int maxVal; // this means the number of units moved per instance
    printf("How fast do you want to go?\n Write here:");
    scanf("%d", maxVal);

    values->numTrans = 2 * maxVal - 1; // # of possible transformation increases by 2 for every additional unit;
    // only 1 possible for first
    int arrNeed = values->numTrans * 2; //
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
