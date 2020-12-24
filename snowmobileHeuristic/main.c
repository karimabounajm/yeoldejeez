#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct inIt
{
    int numTrans;
    int* arrTrans;
};

// add an extra layer to the start of the string that contains the array of the map. that way, we can transition over to 
// beginning of the real map, that we can avoid making the recursive function more complicated

// phase out struct, use global variable
// speed can be used to use 1d arraya for coordinate changes 
// y = y - abs(max - dx)

// struct pa

struct inIt* getTransformations()
{ // get speed spit out array of possible transformations
    struct inIt* values = malloc(sizeof(struct inIt));

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

int main()
{
    int* potato;
}