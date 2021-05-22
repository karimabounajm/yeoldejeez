//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include "dain.h"
//
//// want to create test cases to check speed and make sure that the code is functional
//// run these test cases with traditional recursive methods and the heuristics
//// identify an expected output in advance
//// thank you Kaloti my man, ecs 32b ss2 coming in clutch with the "setting ourselves apart" recommendation
//
//
//// so because we are using a 2d array of ints to describe the current best collisions and an array of 
//// transformations of the x value alone to describe the current best path, it is difficult to identify 
//// what coordinates are on the current best path for the heuristic to reject the path. because of this, 
//// we will set all values in the current best path's coordinates in the bestCol 2d array to -(best value), which can
//// easily be checked at every node. if a node is negative, the algorithm will know that it is on the best path. then 
//// we can very simply just compare the current #collisions in the current path with the absolute value of the #collisions
//// in the best path. replace accordingly
//    // a minor problem that pops up from this is nmaking the coordinates of the current path negative if it becomes a new 
//    // best path. we will have to iterate backwards from here, probably make a helper function to do so. make all points
//    // no longer in the best path positive, make all new points negative
//// so in practice, bestnum is 0 if it hasn't been reached yet, ie hasn't been explored down a path; positive if it is just a
//// normal point, and negative if it is a point in the set of best coordinates
//
//struct gameValues
//{
//    // from the txt file, used to allocate memrory for board 2d array
//    int height, width; 
//
//    // from the user, used in iterating over the recursive possibilities and importantly to 
//    // determine the leftmost horizontal movement based on the speed so that we can iterate
//    // over the possible transformation between itself and its positive self;
//    int minHoriTrans, speed;
//};
//
//struct pathWay
//{
//    // the current width and height can be used with the array of previous transformations and the speed from the 
//    // game value to backtrack into the previous points; when backtracking, we update the number of collisions at 
//    // node by checking if the node we were at before backtracking one instance was a collision, meaning we reduce
//    // the number at that point by one; by using the curWidth and curHeight in conjunction the array of transformations
//    // (itself indexed by the number of transfromations), we can go backwards
//
//    // going forwards is a simple matter of translating the current horizontal transformation with the speed from the 
//    // game values to evaluate exactly which coordinates will be transformed into
//
//    // used as coordinates
//    int curHeightBest, curWidthBest;
//    int curHeightCur, curWidthCur;
//
//    // used to give a value to the current node and serve as an index to the array of horizontal transfromations 
//    // respectively, both serving essential functions
//    int numCollisionsBest, numCollisionsCur;
//    int numTransformBest, numTransformCur; 
//
//    // adjusted coordinates for map of best values
//    int mapBest, mapCur;
//
//    // array of horizontal transformations, which is indexed by the numTransformations and is used to store the path 
//    // from the starting point of each root
//    int bestPath[350]; 
//    int curPath[350]; 
//    
//};
//
//
//
//int main()
//{
//    return 0;
//}       
