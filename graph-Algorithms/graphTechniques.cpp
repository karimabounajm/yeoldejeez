




/*
Representing Information:
	Determine sparse or dense graph, for deciding between an adjacency matrix and list
	• sparse is adjancency list, if used explain ADT used and how data is sorted
	  V + E for adjancency list
	• dense is adjacency matrix, array; if it is not array then must describe
	  V^2 for building adjacency matrix
*/


/* Traversing Definitions
  	A vertex is essentially a node or a quanta of data, and can be connected to another vertex in 
hat is called an edge.
  	An edge can be directed, meaning that the connection between nodes is bidirectional, or it can 
be undirected, which means that edges are reflexive in their connective properties. 
	A loop is a vertex that is connected to itself
	Paths are sequences of vertices connected by edges, with a length equal to the number of edge
in the given sequence. 
	Simple Paths are paths in which no vertex is repeated
	Cycles are paths of a length of at least 1 such that the first and last vertex are the same. 
	Simple Cycles are cycles in which no vertex is repeated, and no edges repeated if the graph 
is undirected
	Directed Acylic Graphs (DAGs) are graphs with no cycles and are directed. they have roots,
or vertices with no incoming edges
	Connected Graphs are undirected graphs in which a path exists between every vertex
	Strongly Connected Graphs are directed graphs in which a path exists between every vertex
	Weakly Connected Graphs are directed graphs that don't have connections between every vertex,
however if the edges were undirected would be strongly connected
	Complete Graphs are graphs in which every vertex shares an edge with all the other vertices 
*/


/* Representing Graphs
	Adjancecy Matrices are essentially 2d arrays that have indices that represent vertices. the 
element at the indices in the 2d array, the elements being identifiers of the vertices, can be 
the weight between the edges, with the absence of an element indicating the edge DNE. because 
it has a spatial complexity Theta(V^2), using matrices to store adjacencies is most feasible 
when the graph is dense. 
	Adjacency Lists involve keeping a distinct list of adjacent vertices for each vertex, which 
has a spatial complexity of Theta(Edges + Vertices). Weights can be stored in each element of 
the list if the graph is weighted. the adjacency list itself can be stored in an array of lists,
where vertices are mapped to their adjacency lists somehow. it is also possible to store the 
adjacency list in the elements themselves
	Note: in my Dijkstra's implementation, I used an array to store every element in the graph,
and in each element had a smaller array/list of all the other vertices and their cost, which in
context is the intervals at which one person visits another. the elements also contained the 
previous cost of the element itself, a bool for if the element has been "visited", and the 
number of other vertices a given vertex visits, to iterate over the array. this is essentially
a jagged array, neither a square matrix or an adjacency list
*/


/* Sorts
	Topological Sorts are performed on directed acyclic graphs (DAGs) with the intention of 
storing or listing all of the vertices in the graph in order, where root vertices are removed
from the graph, forming more, until every vertex in the graph is removed. essentially, the 
algorithm removes every vertex in a given graph with an indegree of zero, with each removal 
resulting in more vertices having indegrees of zero. 
	Efficient Topological sorts, with complexity O(Edges + Vertices), involve an initial search 
to find all root vertices (no inward edges), and placing all of them in a queue. as each vertex 
is dequeued and removed from the graph, the vertices it is connected to by its outward edges
have their indegree decremented, which is then checked. each vertex that subsequently becomes 
a root node is then itself added to the queue to be decremented. removing the vertices in the 
queue at any given time through the algorithm must result in the creation of at least one other 
root node or the a finished topological sort.
*/


/* Shortest Path Analysis
	Unweighted Shortest Path Analysis involves minimizing unweighted path length, which is the 
number of edges in a path (Vertices - 1), uses Breadth First and being complexity O(E + V)
	Weighted Shortest Path Analysis involves minmizing the weighted path length, which is more 
complex in that edges have a cost and the cost of a path isn't the number of edges, but rather
the sum of the weight of each edge
	Negative Cost Cycle is the potential loop in which a path is repeated infinitely because it
includes a negative edge, and the purpose of a minimum algorithm is to minimize the cost of the 
path between two vertices, and going through a cycle with a negative edge reduces the cost
	Applications of shortest path analysis include minimizing cost between two vertices, which
can be implemented by using the first finding of the destination vertex as the end condition
*/


/* Breadth First Search
	Breadth First Search involves to evaluation of vertices immediately connected to starting 
vertex before going on to process other, farther away, vertices. essentially, it involves 
exploring all the vertices a starting vertex is connecting to, giving info regarding the vertex
that discovered them, the cost of discovering them, and confirming that they've been explored. 
after each discovery, the vertices themselves are queued up to be explored. in dijkstra's, this
queue is a min heap, where the vertex with the least cost is what is removed from the heap and 
explored. vertices that are already explored do not subsequently need to be re-evaluated.
	Breadth First Search is often used to enqueue edges in a single step within a larger algorithm,
with Dijkstra using depth first whenever a vertex is dequeued in the sense that every undiscovered
vertex that the one unqueued has an edge of less cost to is added to the priority queue (minheap).
*/


/* Dijkstra's
	Dijkstra's Priority Queue algorithm is a weighted breadth first search with a runtime 
O(E*log(V) + V*log(V)), and used to find the path of least cost between 2 vertices. it 
essentially involves placing all of the outward edges a given vertex has into a minheap, with 
each edge being inserted iff the vertex has not been "explored" and the cumulative cost of the 
edge is less than the least recorded cost of a path to the given vertex at the time of the 
processing. being removed from the min heap entails being explored, and in the meantime a given 
vertex can have its minimum lowered. when it is removed from the minheap, it will have the 
least cost of all the unexplored vertices, and thus will have had achieved its best cost. this 
process continues until there are no edges left to be removed, meaning there are no vertices 
left to explore. 
	Dijkstra's List algorithm is another potential implementation, with the costs at any given 
phase being O(V) because the minimum cost must be checked for every edge, meaning that O(V^2) 
will be spent finding the minimums in the algorithm. updating the minimum cost edge is O(1), 
and with a maximum of E updates the total runtime is O(E + V^2). 
	The priority queue implementation is best suited for sparse graphs, as it would not require
too many insertions of edges into the priority queue/min heap. dense graphs are better suited 
for the list implementation, as iterating over all the potential edges is faster than using a 
binary heap, and given that the graph is dense there will not be too many iterations over 
empty elements, minimizing time lost. using a dense graph with the priority queue implementation 
would not be a good idea, something that is clear from its complexity O(E*logV + V*logV), as it
would add a multiplier to a large value if edges E was large. 
	Dijkstra requires starting points, and has accumulative weights. what it returns is dependent
on what point it starts at. 
*/


/* Minimum Spanning Trees
	Minimum Spanning Trees are undirected graphs that connect all the vertices of a given graph 
with the lowest total cost. it is a tree because the structure is acyclic, it is spanning 
because it includes every vertex in the original graph, and it is minimum just because. the 
minimum spanning tree of a graph has V - 1 edges, with V being the number of vertices in the 
original graph. the insertion of a new edge creates a cycle, one which must be broken by 
removing an edge from within the cycle. that edge is the one with the greatest weight. 
	Prim's Algorithm creates a minimum spanning tree by growing it in succcessive stages. each 
stage involves picking a vertex as a root of a tree and adding an edge, and thus a vertex, to 
it. the algorithm works by finding the edge of least cost at every stage between a vertex that
is in the tree and a vertex that hasn't yet been added to it. essentially, that makes the 
algorithm like Dijkstra's with each phase involving finding the minimum cost. The algorithm is 
O(V^2) without priority queues, which is optimal for dense graphs, and O(E * logV) when binary
heaps are used to sort the edges for sparse graphs. Don't use Prim's for sparse graphs though,
too much overhead in comparison to Kruskal's for the task given condition of adding vertex
	Kruskal's involves sorting the edges by increasing cost using buildheap, building it in 
linear time, and essentially removing minimum edges one by one, connecting vertices together
if connecting them does not form a cycle. This means that multiple sub trees can be formed, 
and continue to form until all are connected as edges are removed from the min heap. This runs 
in O(E * logV), and is good for sparse graphs. The number of vertices is always essentially  
greater than the number of edges, however if there are a minimal number of edges then 
E*logV < V^2. 
	Prim's and Kruskal's are distinguished by the qualifications for edges being added to the 
tree that is being formed. In Prim's the edge that is added has to connect a vertex already in
the tree while Kruskal's allows the connection of any vertices so long as they do not form a 
cycle, something which can be easily determined by checking the root of the vertices given that
unions are being used. 
	Summary:
	• Prim's: O(V^2), best for dense graphs
	• Kruskal's: O(E * logV)
*/


/* Critical Path Analysis
	Topological Sort is needed, which essentially involves removing all the vertices that are 
roots, ie have no incoming edges, and enqueue them. repeat this process with the new roots 
that are created by the process, until there are no vertices left. this only works on directed
acyclic graphs
	Critical Path Analysis involves going through the list sorted by the topological sort and 
forming a graph directed acyclic graph in which every vertex has a completion time that is 
equal to the maximum completion time of the edges leading into it. The first completion times 
are formed from the edges from the roots into the tasks they are prerequisites for. This 
continues until all the tasks are completed, and each task has its own completion time 
determined by the maximum of its children's completion time. Note that a ghost edge is used to
connect all the final or leaf tasks together, and itself has a completion time of the maximum 
of its children. its completion time is the minimum completion time for the given project. 
	From the final vertex and its completion time, or the minimum completion time for the 
project, slack is determined by finding the difference between the latest start time and the 
earliest start time at each vertex. The latest start time is the difference between the 
minimum earliest start time of every vertex a given vertex is connected to and the expected time
required for the completion of the vertex that has outward edges to the vertices in question. 
The minimum start time is already found, and calculating the maximum is calculated as just 
decribed
	Slack essentially means the number of units of time a given task can be delayed without 
causing the project's minimum completion time to decrease. Critical Path is the path in this 
tree in which every edge has a slack of 0. Multiple critical paths can exist, and they signify
a sequence of tasks in which no delays are permitted.
*/


/* Articulation Points
	Articulation Points are essentially vertices in a graph that, if removed, would result in 
the graph becoming disjoint, ie it splitting such that not all vertices can reach one another.
They are found by using a depth first search to number all the vertices in the graph, and while
doing so also recording the least number any given vertex can reach through its first backedge
	If a given vertex has a child with a low greater than or equal to its number, as determined 
by the initial depth first search, then that vertex is an articulation point. This is how the 
actual process of finding all the articulation points works:
	Depth First Search is performed on a given graph, numbering the vertices as they are passed,
with the number assigned to each vertex being Num(v). Afterward, the minimum of every vertex is
computed, with the minimum being the least value of Num(v) that can be reached by a given 
vertex by traversing through its edges and a single backedge. This least value is reached by 
taking the first vertex that has a backedge that the vertex can question can access. Low(v) can 
be computed in O(V + E) time, as it essentially involves a postorder traversal.
	NOTE: C is an articulation point IF AND ONLY IF given a vertex G that is a child of C, the 
Low(G) ≥ Num(C). So the number of the vertex must be less than or equal to the lowest traversal
ID that one of its children is connected to.
	Used in the identification of bottlenecks and the desigining of redundancies 
*/


/* Network Flow
	Big O(Max flow * big-O of dijkstra). This one is pretty simple, just dijkstra away until 
there are no paths from sources to sinks
*/
