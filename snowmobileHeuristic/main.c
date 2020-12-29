#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dain.h"


// NOTE: use memcopy when updating best path, as they are unlikely to share
// the same number of transformations



// a struct that contains essential pieces of information, like the width of the board
// based on the txt file used, the number of possible transformations
struct gameValues
{
    // from the txt file, used to allocate memrory for board 2d array
    int height, width; 

    // from the user, used in iterating over the recursive possibilities and importantly to 
    // determine the leftmost horizontal movement based on the speed so that we can iterate
    // over the possible transformation between itself and its positive self;
    int minHoriTrans, speed;
};



// so because we are using a 2d array of ints to describe the current best collisions and an array of 
// transformations of the x value alone to describe the current best path, it is difficult to identify 
// what coordinates are on the current best path for the heuristic to reject the path. 

// because of this, we will set all values in the current best path's coordinates in the bestCol 2d array to -(best value),  
// which can easily be checked at every node. if a node is negative, the algorithm will know that it is on the best path. 
// then we can very simply just compare the current #collisions in the current path with the absolute value of the #collisions
// in the best path. replace accordingly
    // a minor problem that pops up from this is nmaking the coordinates of the current path negative if it becomes a new 
    // best path. we will have to iterate backwards from here, probably make a helper function to do so. make all points
    // no longer in the best path positive, make all new points negative
// so in practice, bestnum is 0 if it hasn't been reached yet, ie hasn't been explored down a path; positive if it is just a
// normal point, and negative if it is a point in the set of best coordinates. quick update to this, it would not work if the
// number of collisions at a particular node at its best is naturally 0. look for ways to distinguish between the two. 

// i am fairly certain that writing this using a method similar to the binary tree implementation is possible, as we could write
// a function or a class using code based on the number of inputs by the user, but honestly I have no idea how to do this yet;
// think I might revisit the way I do this sometime in the future, to see if using higher order functions is possible.
// I'm getting this from Silicon valley, dinesh, who said something about writing functions that spit out other functions. he used
// Java though, so I might have to wait until I learn that and gain some fluency in it.

// the above is (obviously) in reference to the way I am implementing the recursive search. I was just relooking through ecs32b notes
// and saw the way that the binary tree was searched for using look left/right, and I wonder if writing functions to do something 
// similar based on the speed the user inputs would be worth looking into; said it would run in n time, but that's probably only if
// we have a finite and defined number of branches coming out of each node

// add an extra layer to the start of the string that contains the array of the map. that way, we can transition over to
// beginning of the real map, that we can avoid making the recursive function more complicated

// phase out struct, use global variable
// speed can be used to use 1d arraya for coordinate changes
// y = y - abs(max - dx)

struct pathWay
{
    // the current width and height can be used with the array of previous transformations and the speed from the 
    // game value to backtrack into the previous points; when backtracking, we update the number of collisions at 
    // node by checking if the node we were at before backtracking one instance was a collision, meaning we reduce
    // the number at that point by one; by using the curWidth and curHeight in conjunction the array of transformations
    // (itself indexed by the number of transfromations), we can go backwards

    // going forwards is a simple matter of translating the current horizontal transformation with the speed from the 
    // game values to evaluate exactly which coordinates will be transformed into

    // used as coordinates
    int curHeightBest, curWidthBest;
    int curHeightCur, curWidthCur;

    // used to give a value to the current node and serve as an index to the array of horizontal transfromations 
    // respectively, both serving essential functions
    int numCollisionsBest, numCollisionsCur;
    int numTransformBest, numTransformCur; 

    // array of horizontal transformations, which is indexed by the numTransformations and is used to store the path 
    // from the starting point of each root
    int bestPath[350]; 
    int curPath[350]; 
    
};



// deallocating the structs
void deallocateGameValues(struct gameValues* gV)
{
    free(gV);
}

void deallocatePathWay(struct pathWay* pathInst)
{
    free(pathInst->bestPath);
    free(pathInst->curPath);
    free(pathInst);
}



struct gameValues* initializeGameValues()
{
    struct gameValues* inIT = malloc(sizeof(struct gameValues));

    inIT->width = 0; inIT->height = 0; // set to zero so that they can be modified in a helper function

    printf("Speed: ");
    scanf("%d", &(inIT->speed));

    inIT->minHoriTrans = -((inIT->speed) - 1); 

    return inIT;
}



char** createBoard(char* filename, struct gameValues* inIT)
{
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    // finding the sizes of arrays to avoid having to calculate every time
    // setting max 350 rows and 50 character per row (is pattern so repeats)
    int malPointer = sizeof(char*) * 350; 
    int malRow = sizeof(char) * 50;

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

    inIT->height = Height - 1; // note, Height has extra value from while so -1
    // printf("The length of the file is %d\n", inIT->height); // evaluate with file used

    fseek(file, 0, SEEK_SET);
    fclose(file);

    return gameBoard;
}



// quick note to self, remember the ghost initial layer, might need to modify this function
// slightly to have a layer of all none-trees with which to move into the proper position 
// best strategy is probably to take a buffer string with the first line, use it's length to
// do a for loop with which to define and populate an initial string with '.', and then strcpy 
// the first string into the first thingie; y'all know what it is 


// IMPORTANT UPDATE:
// array will no longer be rectangular, just realized that we can avoid this memory sink by 
// allocating memory by array with only the coordinates that are possible, and nothing more, 
// thus saving a lot of memory; plan to also strip away lines of coordinates when they can no 
// longer be reached and if they don't include a coordinate of best path travel;

int** createMap(struct gameValues* inIT)
{
    // allocating memory for each pointer to an array of integers, or allocating
    // memory for each row
    int numRows = inIT->height;
    int** bestMap = malloc(sizeof(int*) * numRows);

    // we must account for both directions extrema in creating a non-rectangular array
    // note: because we are creating a map of possible values, we can avoid wasting 
    // memory by only allocating memory to coordinates that are possible, and from 
    // there translating coordinates from the gameboard to indices in arrays of the map
    // NOTE: this will probably be slower because of more operations, but also uses half
    // the memory (I'm thinking geometrically, triangle instead of rectangle)

    // starting with an initial width of 1, and increasing the possible width to the range
    // of possible horizontal values as we go along, allocating memory accordingly
    int speed = inIT->speed; int widthRow = 1;
    for(int i = 0;i <= numRows; i++)
    {
        // printf("The row number is %d, and it has a width of %d\n", i, widthRow);
        bestMap[i] = malloc(sizeof(int) * widthRow);
        memset(bestMap[i], -351, widthRow);
        widthRow += 2 * (speed - 1);

        // all values are being defaulted to -351 because setting them to zero would make
        // distinguishing between unexplored nodes and nodes at which #col is zero imp.
        // setting them to the negative of the maximum number of rows ensures that unexplored
        // paths can be distinguished from the best one; can also set to -(height + 1) 
        // instead of the hardcoded maximum number of rows in the board 2d array
    }

    return bestMap;
}



// deallocating the 2d arrays
void deallocateBoard(char** gameBoard, int numRows)
{
    for(int i = 0; i <= numRows; i++) free(gameBoard[i]);
    free(gameBoard);
}

void deallocateMap(int** bestMap, int numRows)
{
    for(int i = 0; i <= numRows; i++) free(bestMap[i]);
    free(bestMap);
}


// this function can be based on two different ways of organizing coordinates in the tree; if 
// we choose to organize tree cordinates by having the initial point be at the midpoint of the
// range , so that the entire tree has positive coordinates, then we can solve the problem by
//      (width - starting_width) + ( (numRows - height) * (speed - 1) )
//      or ( width - (numRows * (speed - 1)) ) + ( (numRows - height) * (speed - 1) ) 

// if we instead choose the root to start at a horizontal component of zero, we would simply 
// need to add to the node coordinate half of the range of possible values in the current row,
// or more simply put
//      ( width + (numRows - height) * (speed - 1) )

// I'm going for the latter, with root of node being (0, numRows); yay
int translateCoordMap(int heightCur, int widthCur, int speed, int numRows)
{
    // this transformation is if the coord of the root node is (0, numRows)
    int mapIndex = widthCur + (numRows - heightCur) * (speed - 1);
    return mapIndex;

    // for coord of root node is (numRows * (speed - 1), numRows)
    // note, easier to include in struct the starting value 
    //      int mapIndex = (widthCur -  nodeWidth) + (numRows - heightCur) * (speed - 1);
}



int translateCoordBoard(int boardWidth, int widthCur)
{
    // translate to the board given repeating biome
    // remember, root node has starting width of 0
    if(widthCur >= boardWidth) return (widthCur % boardWidth);
    else return ((widthCur % boardWidth) + boardWidth);
}



struct pathWay* findInitial(char** gameBoard, int** bestMap, struct gameValues* inIT)
{
    // this returns an array of nodes, so should have coordinates instead of
    // transformations in its path; these nodes are used as the roots of the 
    // recursive functions, and based on the strategy the height should be set 
    // to the bottom 

    // initialize the struct, setting values to 0
    struct pathWay* initialPath = malloc(sizeof(struct pathWay));

    // remember, we take (0, numRows) as the point of the root starting node of doom
    int numRows = inIT->height; int extremaTrans = inIT->speed - 1;
    int numCollisions = 0; int curWidth = 0;

    for(int curHeight = 0; curHeight <= numRows; curHeight++)
    {
        // iterating through every row given that we only go down 
        // one row at a time, extrema transformation
        if(gameBoard[curHeight][curWidth % inIT->width] == '#') numCollisions++;
        bestMap[curHeight][curWidth] = numCollisions;

        (initialPath->bestPath)[curHeight] = extremaTrans;
        (initialPath->curPath)[curHeight] = extremaTrans;

        curWidth += extremaTrans; 
        // printf("The path is %d at height %d\n", (initialPath->BestPath)[curHeight], curHeight);
    }

    // setting up the values for going from bottoms up with the roots, and from each root top 
    // down; we go up the coordinates in the path using the height and number of collisions as
    // methods of identifying position in conjunction with the map and board
    initialPath->numCollisionsBest = numCollisions;
    initialPath->numCollisionsCur = numCollisions;

    initialPath->curHeightBest = numRows;
    initialPath->curHeightCur = numRows;

    initialPath->numTransformBest = numRows;
    initialPath->numTransformCur = numRows;

    initialPath->curWidthBest = curWidth;
    initialPath->curWidthCur = curWidth;

    printf("The number of collisions down this path is %d, with a height of %d and %d number of transformations, and a width of %d\n",
    initialPath->numCollisionsCur, initialPath->curHeightCur, initialPath->curHeightBest, curWidth);

    return initialPath;
}



// for the recursive function, if the best path map has a negative value that is not
// abs greater than 350 (remember the hardcoded max number of lines) and is also abs more
// than the current number of collisoins, then we iterate backwards across the transformations 
// of the current path and modify the best path by your values
// this wouldn't be a problem for fun languages that set initialized but empty places in memory
// as false, but C has to be special weshel 



int main()
{
    struct gameValues* gV = initializeGameValues();
    char** board = createBoard("input.txt", gV);
    int** map = createMap(gV);
    struct pathWay* initialPath = findInitial(board, map, gV);

    printf("The number of collisions down this path is %d, with a height of %d and %d number of transformations, and a width of %d\n",
    initialPath->numCollisionsCur, initialPath->curHeightCur, initialPath->numTransformCur, initialPath->curWidthBest);
    return 0;
}



// important notes

// kaloti recommends writing a priority cue (binary heap), which isn't available in C but is in 
// C++, so consider rewriting in C++ after finishin in C; comparing speeds would be interesting,
// but finish this first;
    // mini update, create a ((2 * speed) - 1)ary tree which allows changing the coordinate of 
    // focus to a particular place in the current 2d array map for coordinate's least #col
    // this would reduce runtime by allowing us to quickly access the number of coordinates 
    // in a timely fashion at the particular coordinate. i really like this 
    // things are getting a bit crazy here, but wowee. 




