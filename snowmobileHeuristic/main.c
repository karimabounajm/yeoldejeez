#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dain.h"



// NOTE: use memcopy when updating best path, as they are unlikely to share
// the same number of transformations



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

    printf("Speed: ");
    scanf("%d", &(inIT->speed));

    return inIT;
}



char** createBoard(char* filename, struct gameValues* inIT)
{
    FILE *file = fopen(filename, "r");
    if (!file) 
	{
		printf("File not run properly\n");
		return NULL;
	}

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
    inIT->width = strlen(gameBoard[0]);
    
    int Height = 0;
    while(++Height)
    {
        gameBoard[Height] = malloc(malRow);
        if(!fgets(gameBoard[Height], 150, file))
            break;
        // printf("Line %d is %s", Height, gameBoard[Height]); // still learning debugger oki?
    }

    inIT->height = Height; // note, Height has extra value from while so -1
    // printf("The length of the file is %d\n", inIT->height); // evaluate with file used

    fseek(file, 0, SEEK_SET);
    fclose(file);

    return gameBoard;
}



// pretty self explanatory, I reckon
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
        for(int j = 0; j <= widthRow; j++) bestMap[i][j] = 351;
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
    int numCollisions = 0; int curWidth = 1;

    // iterating through every row given that we only go down 
    // one row at a time, extrema transformation
    for(int curHeight = 0; curHeight <= numRows; curHeight++)
    {
        // adjusting current coordinates to fit gameBoard array structure;
        if(gameBoard[curHeight][curWidth % inIT->width] == '#') numCollisions++;

        // adjusting current coordinate to the bestMap jagged array structure
        bestMap[curHeight][curWidth + curHeight * extremaTrans] = numCollisions;
        printf("There is %d collisions at coordinates %d, %d; adjusted coord is %d\n", bestMap[curHeight][curWidth + curHeight * extremaTrans], 
        	curHeight, curWidth, curWidth + curHeight * extremaTrans);

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

    // current path values
	initialPath->numCollisionCurrent = numCollisions;
	initialPath->heightCurrent = numRows;
	initialPath->numTransCurrent = numRows;
	initialPath->widthCurrent = curWidth;
	initialPath->mapCurrentAdjusted = curWidth + numRows * extremaTrans;

	// best path values
	initialPath->numCollisionBest = numCollisions;
    initialPath->heightBest = numRows;
    initialPath->numTransBest = numRows;
    initialPath->widthBest = curWidth;
    initialPath->mapBestAdjusted = curWidth + numRows * extremaTrans;

    // free the original game values struct
    free(inIT);

    return initialPath;
}



int recursiveFunCtion(struct pathWay* pathsVal, int** map, char** board, int curTrans)
{
	// adjust the current values based on the transformation
	pathsVal->widthCurrent += curTrans;
    pathsVal->heightCurrent += pathsVal->speed - abs(curTrans);
	pathsVal->mapCurrentAdjusted += curTrans;

	// add the transformation into the path of transformations
	(pathsVal->numTransCurrent)++;
	(pathsVal->currentPath)[pathsVal->numTransCurrent] = curTrans;

	// check if the coordinate the current path transforms into is a collision, and increase the number accordingly
    if(board[pathsVal->heightCurrent][(pathsVal->mapCurrentAdjusted) % (pathsVal->width)] == '#') (pathsVal->numCollisionCurrent)++;

	// evaluate the base cases of the recursive function
	int baseCaseIndex = evaluateBaseCases(pathsVal, map);

	switch(baseCaseIndex)
	{
		case 0:
			return 0;
		case 1:
			break;
		case 2: {
			int maxTrans = 1 - abs(pathsVal->numRows - pathsVal->heightCurrent);
			for(int i = maxTrans; i <= abs(maxTrans); i++) 
				recursiveFunCtion(pathsVal, map, board, i);
			break;
		}
		case 3: {
			int maxTrans = 1 - abs(pathsVal->speed);
			for(int i = maxTrans; i <= abs(maxTrans); i++) 
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
	pathsVal->mapCurrentAdjusted -= curTrans;

	// this quickly returns the number of transformations to the proper value (back one transition)
	(pathsVal->numTransCurrent)--;

	return 1; // true, signal to continue recursively searching; return 0 is used when absolute best path is found
}


// this is to update the map for a new best path that intersects with the old one
void updateBestMapIntersect(struct pathWay* pathsVal, int** map)
{
	// variables to hold coordinates to iterate backwards over 
	int bufferWidth = pathsVal->widthBest; int bufferHeight = pathsVal->heightBest;
	int bufferAdjustedWidth = pathsVal->mapBestAdjusted;
	int bufferIndexBest = pathsVal->numTransBest;

	// these values should not be updated, as this portion of the current best path will be maintained
	while(bufferWidth != pathsVal->widthCurrent && bufferHeight != pathsVal->heightCurrent)
	{
		// update the coordinates of the best path, with the intention of transforming back into the current node
		bufferAdjustedWidth -= pathsVal->bestPath[pathsVal->numTransBest];
		bufferWidth -= pathsVal->bestPath[pathsVal->numTransBest];
		bufferHeight -= (pathsVal->speed - abs(pathsVal->bestPath[pathsVal->numTransBest]));


		// update the number of transformations, which will be used as the starting index 
		bufferIndexBest--;
	}

	while(bufferIndexBest >= 0)
	{
		// update the coordinates of the best path, with the intention of transforming back into the current node
		bufferAdjustedWidth -= pathsVal->bestPath[pathsVal->numTransBest];
		bufferWidth -= pathsVal->bestPath[pathsVal->numTransBest];
		bufferHeight -= (pathsVal->speed - abs(pathsVal->bestPath[pathsVal->numTransBest]));

		// make this coordinate in the map positive
		map[bufferHeight][bufferAdjustedWidth] = abs(map[bufferHeight][bufferAdjustedWidth]);

		// update the number of transformations, which will be used as the starting index 
		bufferIndexBest--;
	}
}



// this is to update the map for a new best path that does not intersect with the old one
void updateBestMapNoIntersect(struct pathWay* pathsVal, int** map)
{
	// variables to hold coordinates to iterate backwards over 
	int bufferWidth = pathsVal->widthBest; int bufferHeight = pathsVal->heightBest;
	int bufferAdjustedWidth = pathsVal->mapBestAdjusted;
	int bufferIndexBest = pathsVal->numTransBest;

	while(bufferIndexBest >= 0)
	{
		// update the coordinates of the best path, with the intention of transforming back into the current node
		bufferAdjustedWidth -= pathsVal->bestPath[pathsVal->numTransBest];
		bufferWidth -= pathsVal->bestPath[pathsVal->numTransBest];
		bufferHeight -= (pathsVal->speed - abs(pathsVal->bestPath[pathsVal->numTransBest]));

		// make this coordinate in the map positive
		map[bufferHeight][bufferAdjustedWidth] = abs(map[bufferHeight][bufferAdjustedWidth]);

		// update the number of transformations, which will be used as the starting index 
		bufferIndexBest--;
	}
}


void updateBest(struct pathWay* pathsVal, int** map)
{
	// updating the best value
	pathsVal->numCollisionBest -= abs(map[pathsVal->heightCurrent][(pathsVal->widthCurrent)] - pathsVal->numCollisionCurrent);

	// temporary value that holds the index at which the new path ends and the old best path dominates the path
	int bufferIndexBest = pathsVal->numTransBest;
	
	// transform backwards through the best path until the current point (which is a new best path) is reached
	while(pathsVal->widthBest != pathsVal->widthCurrent && pathsVal->heightBest != pathsVal->heightCurrent)
	{
		// update the coordinates of the best path, with the intention of transforming back into the current node
		pathsVal->widthBest -= pathsVal->bestPath[pathsVal->numTransBest];
		pathsVal->heightBest -= (pathsVal->speed - abs(pathsVal->bestPath[pathsVal->numTransBest]));

		// update the number of transformations, which will be used as the starting index 
		bufferIndexBest--;
	}

	// temporary value that holds the index at which the current path ends, which we use to continue writing the 
	// path in 
	int bufferIndexCurrent = pathsVal->numTransCurrent + 1;

	for(bufferIndexBest++; bufferIndexBest <= pathsVal->numTransBest; bufferIndexBest++)
	{
		(pathsVal->currentPath)[bufferIndexCurrent] = (pathsVal->bestPath)[bufferIndexBest];
		bufferIndexCurrent++;
	}

	for(int i = 0; i <= bufferIndexCurrent; i++)
	{
		(pathsVal->bestPath)[i] = (pathsVal->currentPath)[i];
	}

	pathsVal->numCollisionBest = bufferIndexCurrent;
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
	if(pathsVal->heightCurrent == pathsVal->numRows) return 1;

	// if the current node has more collisions than a previously explored path in this current node, the node can be
	// rejected; note, must accomidate for the negative nodes, which denote a best path; note 2, do not update best
	// path if path with equal number of collisions is found, would be a waste
	if(pathsVal->numCollisionCurrent > (map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted]))
	{
		// this is if the current path has fewer collisions than the best path;
		if((map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted] < 0) && (pathsVal->numCollisionCurrent < abs(map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted])))
		{
			updateBest(pathsVal, map);
			return 1;
		}
		
		// this is if the current path has more collisions than the fewest number of collisions in the map
		else
			return -1;
	}

	// if a transformation could take the player below the expected line, the transformations should be adjusted to
	// only allow those that would keep the player within bounds
	if((pathsVal->heightCurrent) > (pathsVal->numRows - pathsVal->speed)) return 2;
	
	// check if the node is unexplored, given that 351 is set for all nodes that haven't been explored yet
	if(map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted] == 351) 
		{
			// update this coordinate in the map to include its very first entry
			map[pathsVal->heightCurrent][pathsVal->mapCurrentAdjusted] = pathsVal->numCollisionCurrent;
			return 4;
		}

	else return 3;
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

    for(; (pathsVal->numTransExtrema) >= 0; --(pathsVal->numTransExtrema))
    {	
    	// adjust collisions value of current path as we enter a new pathway with this point as its root node
    	if(board[pathsVal->numTransExtrema][(pathsVal->widthExtrema) % (pathsVal->width)] == '#') 
    	{
    		(pathsVal->numCollisionExtrema)--;
    		pathsVal->numCollisionCurrent = pathsVal->numCollisionExtrema;
    	}
    	else pathsVal->numCollisionCurrent = pathsVal->numCollisionExtrema;

    	// adjust coordinate values of current path 
    	pathsVal->widthCurrent = pathsVal->widthExtrema;
    	pathsVal->heightCurrent = pathsVal->numTransExtrema;
		pathsVal->mapCurrentAdjusted = pathsVal->widthCurrent + pathsVal->heightCurrent * (pathsVal->speed - 1);

		// calling the actual recursive function; note the conditional, which fires if a conditional with 0 collisions is found
		if(recursiveFunCtion(pathsVal, map, board, 0) == 0) return pathsVal;

    	// adjusting the extrema values (after calling the recursive function at current point, tranform to next)
	    pathsVal->widthExtrema -= pathsVal->speed;
	    pathsVal->mapExtremaAdjusted -= 2 * pathsVal->speed;
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
