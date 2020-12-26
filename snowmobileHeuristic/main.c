#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dain.h"


// new idea, we create our own .txt files with a defined number of columns + rows
// we take a random number between 3 and 7 for each row, and put that many trees in it out 
// of a ration of 10 (adjust accordingly)
// add it to the run.sh file, we overwrite the previous created txt file. create a .c file to
// print out such a map, and use >> after first clearing with >


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
// normal point, and negative if it is a point in the set of best coordinates. quick update to this, it would not work if the
// number of collisions at a particular node at its best is naturally 0. look for ways to distinguish between the two. 


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
    // height to easily accessible variable; note, is 0 because we did first row
    // and in while loop it is iterated forward
    inIT->width = strlen(gameBoard[0]) - 1;
    int Height = 0; 

    while(++Height)
    {
        gameBoard[Height] = malloc(malRow);
        if(!fgets(gameBoard[Height], 150, file))
            break;
        // printf("Line %d is %s", Height, gameBoard[Height]); // still learning debugger oki?
    }

    inIT->height = Height - 1; // note, Height has an extra value so we must subtract it
    // printf("The length of the file is %d\n", Height); // evaluate with file used

    fseek(file, 0, SEEK_SET);
    fclose(file);

    return gameBoard;
}


// quick note to self, remember the ghost initial layer, might need to modify this function
// slightly to have a layer of all none-trees with which to move into the proper position 
// best strategy is probably to take a buffer string with the first line, use it's length to
// do a for loop with which to define and populate an initial string with '.', and then strcpy 
// the first string into the first thingie; y'all know what it is 



int** createMap(struct gameValues* inIT)
{
    // same logic here as before; 
    int malPointer = sizeof(int*) * (inIT->height - 1); 
    int malRow = sizeof(int) * inIT->width;
    int Width = inIT->width;

    int** bestMap = malloc(malPointer);
    int i = inIT->height;

    for(; i > 0; --i)
    {
        bestMap[i] = malloc(malRow);
        memset(bestMap[i], -351, Width);
        // all values are being defaulted to -351 because setting them to zero would make
        // distinguishing between unexplored nodes and nodes at which #col is zero imp.
        // setting them to the negative of the maximum number of rows ensures that unexplored
        // paths can be distinguished from the best one; can also set to -(inIT->height + 1) 
        // instead of the hardcoded maximum number of rows in the board 2d array
    }

    return bestMap;
}



struct pathWay* findInitial(char** gameBoard, int** bestMap, struct gameValues* inIT)
{
    struct pathWay* initialPath = malloc(sizeof(struct pathWay));
    int Height = inIT->height; int numCol = 0; 
    int w_pos = Height * (inIT->speed - 1); int w_trans = inIT->speed - 1;

    for(int h_pos = 0; h_pos < Height; h_pos++)
    {
        w_pos += w_trans;
    }

}


// this also might need to be modified to reflect the initial ghost layer. we'll see tomorrow,
// it's call time. EEGLAB drains the soul from me, lordie lordie lord


int main()
{
    struct gameValues* gV = initializeGameValues();
    createBoard("input.txt", gV);
    createMap(gV);
}
       