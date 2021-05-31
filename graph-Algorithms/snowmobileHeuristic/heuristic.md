# problem definition
so essentially, we are searching for the path down a mountain that has the least number of collisions. the mountain is defined by a txt file, with trees being represented by character '#'. given that after every instance of motion (quick note, the .txt file will almost not be wide enough to cover entire environment needed, and so because of "arboreal genetics and biome stability, the same pattern repeats to the right many times"; this means the map repeats, though the coordinates are discrete). now the sleigh dude has a chance to adjust the motion, and possible movements are by ratios of whole numbers a / b such that a + b == speed b is ≥1. note: positive a is going right, negative is left, positive b is going down. after finding the path with the least number of collisions (or I guess the first with zero collisions), print out the map and show the big man's path down da mountain, with character α representing the saint nickie himself.

what I'm trying to do is minimize the time needed to finding this path, or at the very least noticeably reduce it. I plan to do so by implementing some heuristics, which shall be explained post haste (thank you ecs 20). to reiterate an important clarifying update, each point is discrete. by this I mean that the number of collisions at each point is independent of the definition of the array you use to display the final answer. though the display may show the character move so far right he reappears in the left, the position he is in is still independent of the one he is displayed in.

# variables used
important variables used in this function and which I guess I'll be referencing a lot while writing it are:
    a 2d array containing all the chars of the gameboard
    a 2d array containing all the integer values of the least number of collisions at each point (updated constantly)
    an array of coordinates containing the coordinates (and thus the transformations) of the best path
    an array of coordinates containing the coordinates (and thus the transformations) of the current path
    an array of coordinates containing the coordinates (and thus the transformations) of the extrema path

# general heuristic strategy
to avoid the time drain of recursively going down every branch for every transformation, I plan to check at every node whether or not continuing down the tree of its possible transformation could possibly yield a path with fewer collisions than the currently maintained best;

this can be achieved by creating a 2d array of the same dimensions as the gameboard array which contains the current least number of collisions of any path once it has reached a point. what this means is, this blank array is populated over time with values for the least number of collisions as trees are gone down. at every node, if the current number of collisions is greater than the least number of collisions previous achieved as we go down this node, we can discount it (as in previous iterations, the full tree will have been gone down). 

the logic used above can be further extended to finding new best paths. because an array will be used to record the coordinates of the best path, if a node is reached which is both inside the current best path AND has fewer collisions than the current best number upon reaching the point, a new best path is found and the array of the best path's coordinates can be updated by the current path leading up to the current node (leaving the rest of the array the same, creating a new better path to the old best path) quick note to self, we don't need to check the other paths down this node because the past search down the tree during the finding of the best array will have done so, so we know that the best path is as its name implies the best path.

now for a quick logic explanation: because we are given rodulfs speed by the user, and because of the definition of the possible slopes that the character can go vroom vroom down the mountain, there are a finite number of transformations. this means that we can iterate over the transformation recursively, and thus go down every possible path from the bottom up. this brute forcing is not exactly preferable, and is my original inspiration for trying to solve this problem a wee bit differently (this is based on advent of code day 3 though, and god knows I butchered it in my interpretation). 

so instead of brute forcing it in such a way, I am going to try iterating through an extrema linear branch of the tree. what these means is, I'm taking a transformation that is the furthest right (or left, both work) and having the player engage in this movement until they reach the bottom of the map. the whole time, I will be recording the coordinates and counting the number of collisions (these will be the first best values, as they will temporarily be the only ones). 

from this point, because I went down this linear set of transformations in a for loop, the last coordinates will be last (instead of being first had it been recursive). what I plan to do is start at the last node, and from there go up one, and treat it as a parent node. from there, I will start a recursive function which will attempt to go down every path possible from that particular node. after doing so (all the while logging the coordinates in the 2d vector and checking for a new best path), we will move up another node in that linear path base case, and repeat the process of taking it as a parent node and going down every possible branch in its tree. 

from here, it is easy. every time a point is reached, it will attempt to log a new best value in the 2d array, and thus make the process more competitive. as more and more paths are placed into the 2d as coordinates are reached into multiple times, the time that would otherwise be taken to go fully down a path will be massively reduced as, yep you guessed it boyoossss, we can reject paths outright by the 2d array. 

this will continue to happen until we try every single path, most of which will be rejected early on; as a quick and important update, the array used to store the best values by coordinate can be written as a non-rectangular 2d array, with memory only being allocated based on the number of values needed and nothing more, and the map coordinates used and logged being translated over to the array by a simple mathematical operator I just realized we could use I'm drinking a good ole cuppa joe. 

So we start in the middle of the map, and we need to be able to distinguish between going right or left. that means we can either start at an origin of (0, numRows) or at a point such that we will never reach a negative value, or (numRows * (speed - 1), numRows); either way, we can translate these units over to proper units that have a horizontal component at zero for the furthest left component of the row. if we start at the origin, adding to the horizontal component (current height * (speed - 1)), or half of the range of the row, would set the lowest component of the row to zero. if we start at the before described midpoint, simply taking the remainder of (horizontal component % starting coordinate) + (height * (speed - 1)) would translate the component over to the jagged 2d array.

allocating memory to the array should itself be a simple task. you allocate for each row (height of row) * (speed - 1) * 2, which covers every possible horizontal value. why should be clear by now, but just to be clear both to myself when I probably rewrite this into something more eloquent and y'all readers, given that the height of the row is how far away the current row is vertically from the starting point, and that there is a minimum distance of 1 down vertically (away from starting vertical position) every transformation, the bound of how far away horizontally from the starting point the player may go in a path given the current line is that numRows * (speed - 1); we multiply this by two because the player can go in 2 directions. I might draw this out, but this is becoming slightly overkill

# base cases I'm thinking about
funny thing is, I've barely started to actually code this. my whiteboard is all marked up though, and I think tonight is going to be a fun night. so the base cases to consider pretty much have to be:

    if the number of collisions at this node is greater than the previous best number of collisons 
    
        (try another trans)
    
    if the number of collisions at this node is equal to the prev best 
    
        (you may pass)
    
    if the number of collisions at this node is less than the prev best 
        
        (you may pass + update best in 2d array):   
    
    if the number is less AND is a coordinate in the previous best path:
        
        (new best path taking the current path and the rest of the old path; go up one node in the recursive function/)
    
    if height < speed (to adjust for overshooting), follow the general trend of:
    
        if height < speed - 1, then do all transformations except straight down  
    
        if height < speed - 2, then do all transformations except straight down and speed - 1 down 1 right or left  
        
        if height < speed - 3, then do all transformations except straight down to speed - 2 down 1 right or left

    if the number of collisions in the best path is 0