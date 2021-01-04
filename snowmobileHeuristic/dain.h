#ifndef santa
#define santa



struct gameValues;

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


struct pathWay;


void deallocatePathWay(struct pathWay* pathInst);


struct gameValues* initializeGameValues(void);


char** createBoard(char* filename, struct gameValues* inIT);


int** createMap(struct gameValues* inIT);


void deallocateBoard(char** gameBoard, int numRows);


void deallocateMap(int** bestMap, int numRows);


struct pathWay* initializePaths(char** gameBoard, int** bestMap, struct gameValues* inIT);


int recursiveFunCtion(struct pathWay* pathsVal, int** map, char** board, int curTrans);


void updateBestMapIntersect(struct pathWay* pathsVal, int** map);


void updateBestMap(struct pathWay* pathsVal, int** map);


int evaluateBaseCases(struct pathWay* pathsVal, int** map);


struct pathWay* findBestPath(char* filename);


#endif


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

    // the current width and height can be used with the array of previous transformations and the speed from the 
    // game value to backtrack into the previous points; when backtracking, we update the number of collisions at 
    // node by checking if the node we were at before backtracking one instance was a collision, meaning we reduce
    // the number at that point by one; by using the curWidth and curHeight in conjunction the array of transformations
    // (itself indexed by the number of transfromations), we can go backwards

    // going forwards is a simple matter of translating the current horizontal transformation with the speed from the 
    // game values to evaluate exactly which coordinates will be transformed into

    // used as coordinates

    // used to give a value to the current node and serve as an index to the array of horizontal transfromations 
    // respectively, both serving essential functions

    // adjusted coordinates for map of best values

    // array of horizontal transformations, which is indexed by the numTransformations and is used to store the path 
    // from the starting point of each root    

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
// int translateCoordMap(int heightCur, int widthCur, int speed, int numRows)
// {
//     // this transformation is if the coord of the root node is (0, numRows)
//     int mapIndex = widthCur + (numRows - heightCur) * (speed - 1);
//     return mapIndex;

//     // for coord of root node is (numRows * (speed - 1), numRows)
//     // note, easier to include in struct the starting value 
//     //      int mapIndex = (widthCur -  nodeWidth) + (numRows - heightCur) * (speed - 1);
// }

// int translateCoordBoard(int boardWidth, int widthCur)
// {
//     // translate to the board given repeating biome
//     // remember, root node has starting width of 0
//     if(widthCur >= boardWidth) return (widthCur % boardWidth);
//     else return ((widthCur % boardWidth) + boardWidth);
// }
