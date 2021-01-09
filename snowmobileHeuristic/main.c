#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dain.h"

// major update idea, realized that there are many nodes that literally cannot be reached despite being the bounds of the 
// extrema; only two additional possible nodes are added per row, so the memory allocated to the 2d array containing the least
// cost/best number of collisions at each point can be significantly reduced in size, with 2n + 1 pieces of memory allocated 
// per row; this is awesome, but will require some extra math work I will implement later. 


// important note, what if several paths are skipped down a tree because a best path has been found 
// in it's node, but it hasn't been properly explored
// guess I probably shouldn't worry about it, due to the extrema initial path and how it works; pretty 
// convenient how that ended up huh


// an initia; struct used to hold pieces of information on the board, like the width based on the txt
// file used, the number of possible transformations
struct gameValues
{
    // from the txt file, used to allocate memrory for board 2d array
    int height, width; 

    // from the user, used in iterating over the recursive possibilities and importantly to 
    // determine the leftmost horizontal movement based on the speed so that we can iterate
    // over the possible transformation between itself and its positive self;
    int speed;
};


struct pathWay
{
	// this struct is meant to hold all the necessary integer values, such as coordinates and values that go into the recursive 
	// callings and the paths; it accomidates for the best path, the current path, and the current position along the extrema line 
	// in the external for loop which initiates a new optimization sequence (taking that value as the node); also has board values

	// game values (to be absorbed from the game values struct)
    int numRows, speed, width; 

    // extrema path and values
    int numTransExtrema, numCollisionExtrema, widthExtrema, mapExtremaAdjusted;

    // best path; note, when replacing a portion of the best path, iterate backwards from the point where the best path ends (given
    // by widthBest) and the number of transformations to the point at which the new best and old best intersect, and then meld the
    // two paths together from that point given all the transformation just made
    int numTransBest, widthBest, heightBest, numCollisionBest, mapBestAdjusted;
    int bestPath[350];

    // current best, which itself is modified with every calling of the recursive function and is reverted back to a value in the 
    // extrema line when a tree is finished being explored
    int numTransCurrent, widthCurrent, heightCurrent, numCollisionCurrent, mapCurrentAdjusted;
    int currentPath[350];
};


// deallocating the structs
void deallocatePathWay(struct pathWay* pathInst)
{
    free(pathInst->bestPath);
    free(pathInst->currentPath);
    free(pathInst);
}


// simply requests the speed from the user and then initializes the struct to be passed into createBoard
struct gameValues* initializeGameValues()
{
    struct gameValues* inIT = malloc(sizeof(struct gameValues));

    inIT->width = 0; inIT->height = 0; // set to zero so that they can be modified in a helper function

	inIT->speed = 3;
    // printf("Speed: ");
    // scanf("%d", &(inIT->speed));

    return inIT;
}


// idea, modify this to be const, which would keep a copy from being created for each and every
// calling of a function that relates to it
char** createBoard(char* filename, struct gameValues* inIT)
{
    FILE *file = fopen(filename, "r");
    if (!file) 
	{
		printf("File not run properly\n");
		return NULL;
	}
	
	// finding the width of the a row, to save allocating memory; take buffer
	// string for this,
	char bufferString1[150];
    fgets(bufferString1, 150, file); 

    // recording the number of characters in each line of the file, taking into account
	// the newline. note, am not adjusting for indices
    int charRow = strlen(bufferString1) - 1;

    // finding the sizes of arrays to avoid having to calculate every time
    // setting max 350 rows and allocating memory for each row by the length just found
	// note the additional 1 character allocated for the new line \n 
    int malPointer = sizeof(char*) * 350; 
    int malRow = sizeof(char) * (charRow + 2);

    // defining the board array and its first row, allocating memory for each
    char** gameBoard = malloc(malPointer);
    gameBoard[inIT->height] = malloc(malRow);

	// setting up the ghost layer at height = 0; 
	for(int i = 0; i <= charRow; i++) gameBoard[inIT->height][i] = '.';
	(inIT->height) += 1;

    // reading the first line from the file
	gameBoard[inIT->height] = malloc(malRow);
	strcpy(gameBoard[inIT->height], bufferString1);
    
    // iterating through the file to create the rest of the game board
	int Height = inIT->height;
    while(++Height)
    {
        gameBoard[Height] = malloc(malRow);
        if(!fgets(gameBoard[Height], 150, file))
            break;
        // printf("Line %d is %s", Height, gameBoard[Height]); // still learning debugger oki?
    }

	// note, height and width must be adjusted because they are indexed
    inIT->height = Height; 
	inIT->width = charRow;

	// closing the file
    fclose(file);

    return gameBoard;
}


int** createMap(struct gameValues* inIT)
{
    // allocating memory for each pointer to an array of integers, or allocating
    // memory for each row
    int numRows = inIT->height;
    int** bestMap = malloc(sizeof(int*) * numRows);

    // note: because we are creating a map of possible values, we can avoid wasting 
    // memory by only allocating memory to coordinates that can be reached, and from 
    // there translating coordinates to the gameboard and the map of best values being created
    // to indices in arrays of the map

    // NOTE: positive values in the map indicate the least number of collisions among any path
    // so far explored down a particular coordinate, a negative value distinguishes this 
    // coordinate as being in the current best path, and a value of 351 indicates that the
    // current point is unexplored

    // starting with an initial width of 1, and increasing the possible width to the range
    // of possible horizontal values as we go along, allocating memory accordingly
    int speed = inIT->speed; int widthRow = 1;
    for(int i = 0; i <= numRows; i++)
    {
        // printf("The row number is %d, and it has a width of %d\n", i, widthRow);
        bestMap[i] = malloc((sizeof(int) + 1) * widthRow);
        for(int j = 0; j < widthRow; j++) bestMap[i][j] = 351;
        widthRow += 2 * (speed - 1);

        // values are being defaulted to 351 because the maximum number of lines is hardcoded
        // to be 350, meaning that there will at most be 350 collisions, so 351 cannot be part 
        // of any path, and thus is a negative value that can be distinguished from a coordinate
        // in the best path
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


struct pathWay* initializePaths(char** gameBoard, int** bestMap, struct gameValues* inIT)
{
    // initialize the struct, setting values to 0
    struct pathWay* initialPath = malloc(sizeof(struct pathWay));

    // remember, we take (0, numRows) as the point of the root starting node of doom
    int numRows = inIT->height; int extremaTrans = inIT->speed - 1;
    int numCollisions = 0; int curWidth = 0;

	// setting the ghost layer transformations; note, extrema trans is used to avoid accessing memory
	// incorrectly in the context of the for loop going backwards; 
    (initialPath->bestPath)[0] = extremaTrans;
    (initialPath->currentPath)[0] = extremaTrans;

    // iterating through every row given that we only go down one row at a time, extrema transformation
	// note that height will start at 1, due to the ghost layer
    for(int curHeight = 1; curHeight <= numRows; curHeight++)
    {
        // adjusting current coordinates to fit gameBoard array structure; remembering that horizontally the 
		// pattern repeats and that the inIT->width describes the number of discrete values to be looked at,
		// barring the new line \n
        if(gameBoard[curHeight][curWidth % (inIT->width)] == '#') numCollisions++;

        // adjusting current coordinate to the bestMap jagged array structure; note, must be negative as this is
		// temporarily the best path
        bestMap[curHeight][curWidth + curHeight * extremaTrans] = -numCollisions;
        printf("There is %d collisions at height %d, width %d; adjusted width is %d: char of %c\n", bestMap[curHeight][curWidth + curHeight * extremaTrans], 
        	curHeight, curWidth, curWidth + curHeight * extremaTrans, gameBoard[curHeight][curWidth % (inIT->width)]);

        // updating the horizontal transformation path, for iterating backwards
        (initialPath->bestPath)[curHeight] = extremaTrans;
        (initialPath->currentPath)[curHeight] = extremaTrans;

        // this is the unadjusted coordinate; note, because of the ghost layer used, this
        // syntax will add an extra value of the extrema transformation to the width, so 
        // subtract one value of it from the final width
        curWidth += extremaTrans; 
    }

    // adjusting the width to reflect the extra transformation that was applied to it
    curWidth -= extremaTrans;

    // setting the map values
    initialPath->numRows = numRows;
    initialPath->speed = inIT->speed;
    initialPath->width = inIT->width;

    // extrema path values
    initialPath->numTransExtrema = numRows;
    initialPath->numCollisionExtrema = numCollisions;
    initialPath->widthExtrema = curWidth;
    initialPath->mapExtremaAdjusted = curWidth + numRows * extremaTrans;

    // current path values, path array already written in for loop
	initialPath->numCollisionCurrent = numCollisions;
	initialPath->heightCurrent = numRows;
	initialPath->numTransCurrent = numRows;
	initialPath->widthCurrent = curWidth;
	initialPath->mapCurrentAdjusted = curWidth + numRows * extremaTrans;

	// best path values, path array already written in for loop
	initialPath->numCollisionBest = numCollisions;
    initialPath->heightBest = numRows;
    initialPath->numTransBest = numRows;
    initialPath->widthBest = curWidth;
    initialPath->mapBestAdjusted = curWidth + numRows * extremaTrans;

    // free the original game values struct
    free(inIT);

    return initialPath;
}












   




// takes a horizontal transformation, and from there checks every possible tranformation from it
int recursiveFunCtion(struct pathWay* pathsVal, int** map, char** board, int curTrans)
{
	// adjust the current values based on the transformation
	pathsVal->widthCurrent += curTrans;
    pathsVal->heightCurrent += pathsVal->speed - abs(curTrans);
	pathsVal->mapCurrentAdjusted += curTrans + (abs(pathsVal->speed) - 1) * (pathsVal->speed - abs(curTrans));

	// add the transformation into the path of transformations
	(pathsVal->numTransCurrent)++;
	(pathsVal->currentPath)[pathsVal->numTransCurrent] = curTrans;

	// check if the coordinate the current path transforms into is a collision, and increase the number accordingly
    if(board[pathsVal->heightCurrent][(pathsVal->mapCurrentAdjusted) % (pathsVal->width)] == '#') (pathsVal->numCollisionCurrent)++;

	// printf("The best number of collisions at width %d and height %d is %d\n", pathsVal->mapCurrentAdjusted, pathsVal->heightCurrent, 
		// map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted]);

	// evaluate the base cases of the recursive function
	int baseCaseIndex = evaluateBaseCases(pathsVal, map);

	switch(baseCaseIndex)
	{
		case 0: // this is if a path with zero collisions is found, which will result in the ending of the search;
			return 0;
		case 1: // this is if a path with the potential of being the best cannot be found down this tree
			break;
		case 2: {
			// for cases in which certain transformations would take the player beneath the boundary of the map, the 
			// VERTICAL movements must be limited, ie the horitzontal movements must be greater than a certain absolute 
			// value to keep the vertical movement less than the amount that would take the value over the edge; in 
			// practice, this means iterating away from the middle/straight down path

			// min horizontal trans is the minimum distance to the sides needed to keep the tree from breaking 
			int maxTrans = pathsVal->speed - abs(pathsVal->numRows - pathsVal->heightCurrent);

			// iterated backwards from - (speed - maxTrans) to - speed
			for(int i = -abs(maxTrans); i > -(pathsVal->speed); i--) 
				recursiveFunCtion(pathsVal, map, board, i);
			for(int i = abs(maxTrans); i < pathsVal->speed; i++) 
				recursiveFunCtion(pathsVal, map, board, i);
			break;
		}
		case 3: {
			// this is for cases when every possible tranformation given the speed is possible
			int maxTrans = 1 - abs(pathsVal->speed);

			// checks every transformation between -maxTrans to +maxTrans
			for(int i = -abs(maxTrans); i < abs(maxTrans); i++) 
				recursiveFunCtion(pathsVal, map, board, i);
			break;
		}
	}

	// now it is time to move up to the previous node, allowing for the recursive mechanism of the function to function
	// this checks if the coordinate transformed into was a collisions; if so, remove one collision from the count as we
	// move up the tree once more
	if(board[pathsVal->heightCurrent][(pathsVal->mapCurrentAdjusted) % (pathsVal->width)] == '#') (pathsVal->numCollisionCurrent)--;

	// this returns the coordinates to the previous node
	pathsVal->widthCurrent -= curTrans;
    pathsVal->heightCurrent -= pathsVal->speed - abs(curTrans);
	pathsVal->mapCurrentAdjusted -= curTrans + (abs(pathsVal->speed) - 1) * (pathsVal->speed - abs(curTrans));

	// this quickly returns the number of transformations to the proper value (back one transition)
	(pathsVal->numTransCurrent)--;

	return 1; // true, signal to continue recursively searching; return 0 is used when absolute best path is found
}



int evaluateBaseCases(struct pathWay* pathsVal, int** map)
{
	// if a path with zero collisions has been found, then a better one cannot be found, so the search should end
	if(pathsVal->numCollisionBest == 0) 
	{
		// create exit function over here, essentially prints out the current best path 
		return 0;
	}


	// check if the end has been reached
	if(pathsVal->heightCurrent == pathsVal->numRows) 
	{
		// if the new path down has fewer collisions than the best path, it forms a new best path that has no 
		// direct path intersection, meaning that the new best path has not been found because a point on the previous
		// best path has been found with fewer collisions, so they aren't fused together
		if(pathsVal->numCollisionCurrent < abs(pathsVal->numCollisionBest)) 
		{
			updateBestMap(pathsVal, map);
			updateBest(pathsVal, map);
		}
		
		// this indicates breaking out of the loop of possible transformations
		return 1;
	}


	// check if the current path has fewer collisions than the best path and they intersect (point is negative); if so, update
	// the map and the best path. no need to continue down this tree though
	if((map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted] < 0) && (pathsVal->numCollisionCurrent < abs(map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted])))
	{

		// check if the current node is on the extrema path; do this to avoid causing the function to interpret that the current path
		// is better than the previous one
		if(!(pathsVal->mapCurrentAdjusted == pathsVal->mapExtremaAdjusted)) 
		{
			// printf("The current number of collisions is %d at adjusted width %d and height %d, and the previous best is %d at %d and %d\n", pathsVal->numCollisionCurrent, pathsVal->mapCurrentAdjusted,
				// pathsVal->heightCurrent, pathsVal->numCollisionBest, pathsVal->mapBestAdjusted, pathsVal->heightBest);

			updateBest(pathsVal, map);
			updateBestMapIntersect(pathsVal, map);
			
			// this indicates breaking out of the loop of possible transformations
			return 1;
		}

		else
		{ 
			// this indicates that every possible transformation should be explored
			return 3;
		}
	}

	
	printf("The previous best number of collisions at this point is %d\n", map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted]);


	// check if the current path has more collisions than the best path down the current node; this would mean the node is to
	// be rejected, as it is impossible to get a better path given the tree down the node has already been searched
	if(pathsVal->numCollisionCurrent > abs((map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted]))) return 1;


	// check if there's a possibility an overshoot may occur
	if((pathsVal->heightCurrent) >= (pathsVal->numRows - pathsVal->speed)) 
	{
		// check if the node is unexplored, given that 351 is set for all nodes that haven't been explored yet
		if(map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted] == 351) 
		{
			// update this coordinate in the map to include its very first entry, and then check if 
			map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted] = pathsVal->numCollisionCurrent;
		}		

		// if the current node has fewer collisions than the previous best path that traversed this node
		else if(pathsVal->numCollisionCurrent < map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted]) 
			map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted] = pathsVal->numCollisionCurrent;


		return 2;
	}

	else
	{
		// check if the node is unexplored, given that 351 is set for all nodes that haven't been explored yet
		if(map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted] == 351) 
		{
			// update this coordinate in the map to include its very first entry, and then check if 
			map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted] = pathsVal->numCollisionCurrent;
		}		

		// if the current node has fewer collisions than the previous best path that traversed this node
		else if(pathsVal->numCollisionCurrent < map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted]) 
			map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted] = pathsVal->numCollisionCurrent;

		return 3;
	}

}



// note, still need to update the paths with the changed/decreased number of collisions on the map and the array itself
// is in need of testing. I should've spend more time trying to actually finish this over the break


// implement heuristic of accessing the array of tranformations from the node on the extrema currently being checked, 
// thus maximizing efficiency

// this is to update the map for a new best path that intersects with the old one
void updateBestMapIntersect(struct pathWay* pathsVal, int** map)
{
	// variables to hold coordinates to iterate backwards over 
	int bufferHeight = pathsVal->numTransExtrema;
	int bufferWidth = pathsVal->widthExtrema;
	int bufferAdjustedWidth = pathsVal->mapExtremaAdjusted;
	int indexNew = pathsVal->numTransExtrema;
	
	// find the difference between the new best value and the old one to update the old tree while going through it
	int deltaBest = pathsVal->numCollisionCurrent - abs(map[bufferHeight][bufferAdjustedWidth]);

	// these values should not be updated, as this portion of the current best path will be maintained
	while((pathsVal->currentPath[indexNew]) == (pathsVal->bestPath[indexNew]))
	{
		// update the coordinates of the best path, with the intention of transforming back into the current node
		bufferAdjustedWidth += pathsVal->currentPath[indexNew];
		bufferWidth += pathsVal->currentPath[indexNew];
		bufferHeight += (pathsVal->speed - abs(pathsVal->currentPath[pathsVal->numTransBest]));

		// update the number of transformations, which will be used as the starting index 
		indexNew++;
	}

	// creating buffer coordinates for replacing old coordinates with positive values
	int indexRepOld = indexNew;
	int repOldWidth = bufferAdjustedWidth; int repOldHeight = bufferHeight;

	while(indexRepOld <= pathsVal->numTransBest)
	{
		// make this coordinate in the map positive
		map[repOldHeight][repOldWidth] = abs(map[repOldHeight][repOldWidth]) - deltaBest;

		// update the coordinates of the best path, with the intention of transforming back into the current node
		repOldWidth += pathsVal->bestPath[indexRepOld];
		repOldHeight += (pathsVal->speed - abs(pathsVal->bestPath[indexRepOld]));

		// update the number of transformations, which will be used as the starting index 
		indexRepOld++;
	}

	// creating buffer coordinates for replacing old coordinates with positive values
	int indexCreateNew = indexNew;
	int repNewWidthAdjusted = bufferAdjustedWidth; int repNewHeight = bufferHeight;
	
	// this will both set in the new coordinates on the map with negative values, but will also update the array of 
	// transformations on the best path
	while(indexCreateNew <= pathsVal->numTransCurrent)
	{
		// make this coordinate in the map positive
		map[repNewHeight][repNewWidthAdjusted] = -(abs(map[repNewHeight][repNewWidthAdjusted]) - deltaBest);

		// update the coordinates in the array of values
		pathsVal->bestPath[indexRepOld] = pathsVal->currentPath[indexCreateNew];

		// update the coordinates of the best path, with the intention of transforming back into the current node
		repNewWidthAdjusted += pathsVal->currentPath[indexCreateNew];
		bufferWidth += pathsVal->currentPath[indexCreateNew];
		repNewHeight += (pathsVal->speed - abs(pathsVal->currentPath[indexCreateNew]));

		// update the number of transformations, which will be used as the starting index 
		indexCreateNew++;
	}

	// setting the new values for best path, updating to the current values
	pathsVal->numCollisionBest = pathsVal->numCollisionCurrent;
    pathsVal->numTransBest = pathsVal->numTransCurrent;
    pathsVal->widthBest = bufferWidth;
    pathsVal->mapBestAdjusted = repNewWidthAdjusted;	
}



// this is to update the map for a new best path that does not intersect with the old one
void updateBestMap(struct pathWay* pathsVal, int** map)
{
	// variables to hold coordinates to iterate backwards over 
	int bufferHeightOld = pathsVal->numTransExtrema;
	int bufferAdjustedWidthOld = pathsVal->mapExtremaAdjusted;
	int indexNewButOld = pathsVal->numTransExtrema;

	// make positive all the values along the old best path
	while(indexNewButOld <= pathsVal->numTransBest)
	{
		// update the coordinate on the map
		map[bufferHeightOld][bufferAdjustedWidthOld] = abs(map[bufferHeightOld][bufferAdjustedWidthOld]);

		// update the coordinates of the best path, with the intention of transforming back into the current node
		bufferAdjustedWidthOld += pathsVal->currentPath[indexNewButOld];
		bufferHeightOld += (pathsVal->speed - abs(pathsVal->currentPath[pathsVal->numTransBest]));

		// update the number of transformations, which will be used as the starting index 
		indexNewButOld++;
	}

	// variables to hold coordinates to iterate backwards over 
	int bufferHeightNew = pathsVal->numTransExtrema;
	int bufferAdjustedWidthNew = pathsVal->mapExtremaAdjusted;
	int indexNewButNew = pathsVal->numTransExtrema;

	// make negative all the values along the new best path, and update the array of transformations
	while(indexNewButNew <= pathsVal->numTransCurrent)
	{
		// update the coordinate on the map
		map[bufferHeightNew][bufferAdjustedWidthNew] = abs(map[bufferHeightNew][bufferAdjustedWidthNew]);

		// update the coordinates of the best path, with the intention of transforming back into the current node
		bufferAdjustedWidthNew += pathsVal->currentPath[indexNewButNew];
		bufferHeightNew += (pathsVal->speed - abs(pathsVal->currentPath[pathsVal->numTransBest]));

		// update the number of transformations, which will be used as the starting index 
		indexNewButNew++;
	}

	// variables to hold coordinates to iterate backwards over 
	int bufferHeight = pathsVal->heightBest;
	int bufferAdjustedWidth = pathsVal->mapBestAdjusted;
	int indexNew = pathsVal->numTransBest;

	while(indexNew > 0)
	{
		// update the coordinates of the best path, with the intention of transforming back into the current node
		bufferAdjustedWidth -= pathsVal->bestPath[pathsVal->numTransBest];
		bufferHeight -= (pathsVal->speed - abs(pathsVal->bestPath[pathsVal->numTransBest]));

		// make this coordinate in the map positive
		// printf("Number of collisions at adjust width %d and height %d is %d\n", bufferAdjustedWidth, bufferHeight, map[bufferHeight][bufferAdjustedWidth]);
		map[bufferHeight][bufferAdjustedWidth] = abs(map[bufferHeight][bufferAdjustedWidth]);

		// update the number of transformations, which will be used as the starting index 
		indexNew--;
	}
}


struct pathWay* findBestPath(char* filename)
{
	// calling function to initialize values for the board, including height, width, speed, and min movement
	struct gameValues* gV = initializeGameValues();

	// calling function to create the array for the board character, with tree being represented by '#'
    char** board = createBoard(filename, gV);

    // calling function to create the array of least number of collisions by coordinate; 
    int** map = createMap(gV);

    // calling function to initialize the best and current paths, which both start as the same extrema path
    // as previously discussed;
    struct pathWay* pathsVal = initializePaths(board, map, gV);

    // quick debugging value check 
    printf("The number of collisions down the extrema path is %d, with a height of %d and a width of %d. It has experienced %d transformations, and has an adjusted width of %d\n", 
    	pathsVal->numCollisionCurrent, pathsVal->heightCurrent, pathsVal->widthBest, pathsVal->numTransCurrent, pathsVal->mapCurrentAdjusted);

    // a for loop iterating backwards through the coordinates of the extrema path; from here, we evaluate each node on this path
    // and call within the loop the recursive function that treats that node as the root, and checks all the possible paths given
    // the base cases and heuristics developed in the form of the extrema path and the bestMap working in conjunction; 

    for(; (pathsVal->numTransExtrema) >= 0; (pathsVal->numTransExtrema)--)
    {	
		// adjust collisions value of current path as we enter a new pathway with this point as its root node
    	if(board[pathsVal->numTransExtrema][(pathsVal->widthExtrema) % (pathsVal->width)] == '#') 
    	{
    		(pathsVal->numCollisionExtrema)--;
    		pathsVal->numCollisionCurrent = pathsVal->numCollisionExtrema;
			(pathsVal->numTransExtrema)--;
    	}
    	else
		{
			(pathsVal->numTransExtrema)--;
			pathsVal->numCollisionCurrent = pathsVal->numCollisionExtrema;
		}

		// adjusting the extrema values 
	    pathsVal->widthExtrema -= (abs(pathsVal->speed) - 1);
	    pathsVal->mapExtremaAdjusted -= 2 * (abs(pathsVal->speed) - 1);

    	// adjust coordinate values of current path 
    	pathsVal->heightCurrent -= 1;
		pathsVal->widthCurrent -= abs(pathsVal->speed) - 1;
		pathsVal->mapCurrentAdjusted -= 2 * (abs(pathsVal->speed) - 1);
		(pathsVal->numTransCurrent)--;

		// evaluate the base cases of the recursive function
		int baseCaseIndex = evaluateBaseCases(pathsVal, map);

		switch(baseCaseIndex)
		{
			case 0: // this is if a path with zero collisions is found, which will result in the ending of the search;
				return pathsVal;
			case 1: // this is if a path with the potential of being the best cannot be found down this tree
				break;
			case 2: {
				// for cases in which certain transformations would take the player beneath the boundary of the map, the 
				// VERTICAL movements must be limited, ie the horitzontal movements must be greater than a certain absolute 
				// value to keep the vertical movement less than the amount that would take the value over the edge; in 
				// practice, this means iterating away from the middle/straight down path

				// min horizontal trans is the minimum distance to the sides needed to keep the tree from breaking 
				int maxTrans = pathsVal->speed - abs(pathsVal->numRows - pathsVal->heightCurrent);

				// iterated backwards from - (speed - maxTrans) to - speed
				for(int i = -abs(maxTrans); i > -(pathsVal->speed); i--) 
					recursiveFunCtion(pathsVal, map, board, i);
				for(int i = abs(maxTrans); i < pathsVal->speed; i++) 
					recursiveFunCtion(pathsVal, map, board, i);
				break;
			}
			case 3: {
				// this is for cases when every possible tranformation given the speed is possible
				int maxTrans = 1 - abs(pathsVal->speed);

				// checks every transformation between -maxTrans to +maxTrans
				for(int i = -abs(maxTrans); i < abs(maxTrans); i++) 
					recursiveFunCtion(pathsVal, map, board, i);
				break;
			}
		}

	}
	deallocateBoard(board, pathsVal->numRows);
	deallocateMap(map, pathsVal->numRows);

	return pathsVal;
}



int main()
{
    struct pathWay* bestPath = findBestPath("input.txt");
    printf("Reached the end\n");
	deallocatePathWay(bestPath);
	return 0;
}
