




/*
Representing Information:
	Determine sparse or dense graph, for deciding between an adjacency matrix and list
	• sparse is adjancency list, if used explain ADT used and how data is sorted
	• dense is adjacency matrix, array; if it is not array then must describe


dijkstra table inside the vertices
justifications only one sentence


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
	Complete Graphs are graphs in which every vertex shares an edge with all the other vertices */


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
number of other vertices a given vertex visits, to iterate over the array */


/* Sorts
	Topological Sorts are performed on directed acyclic graphs (DAGs) with the intention of 
storing or listing all of the vertices in the graph in order, where root vertices are removed
from the graph, forming more, until every vertex in the graph is removed. essentially, the 
algorithm removes every vertex in a given graph with an indegree of zero, with each removal 
resulting in more vertices having indegrees of zero. 
	Efficient Topological sorts, with complexity O(Edges + Vertices), involve an initial search 
through the graph to find all the root vertices, and placing all of them in a queue. as each 
vertex is dequeued and removed from the graph, the vertices it is connected to by its outward 
edges have their indegree decremented, which is then checked. each vertex that subsequently 
becomes a root node is then itself added to the queue to be decremented. all the vertices in 
the queue at any given time through the algorithm must result in the creation of at least one
other root node


/* Shortest Path Analysis
	Unweighted Shortest Path Analysis involves minimizing unweighted path length, which is the 
number of edges in a path (Vertices - 1), uses Breadth First and being complexity O(E + V)
	Weighted Shortest Path Analysis involves minmizing the weighted path length, which is more 
complex in that edges have a cost and the cost of a path isn't the number of edges, but rather
the sum of the weight of each edge
	Negative Cost Cycle is the potential loop in which a path is repeated infinitely because it
includes a negative edge, and the purpose of a minimum algorithm is to minimize the cost of the 
path between two vertices, and going through a cycle with a negative edge reduces the cost
	Applications of shortest path analysis include minimizing cost between two vertices, meaning 
that */


/* Search Types
	Breadth First Search involves to evaluation of vertices immediately connected to starting 
vertex before going on to process other, farther away, vertices. essentially, it involves 
exploring all the vertices a starting vertex is connecting to, giving info regarding the vertex
that discovered them, the cost of discovering them, and confirming that they've been explored. 
after each discovery, the vertices themselves are queued up to be explored. in dijkstra's, this
queue is a min heap, where the vertex with the least cost is what is removed from the heap and 
explored. vertices that are already explored do not subsequently need to be re-evaluated */


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
on what point it starts at. */


/* Minimum Spanning Trees
	Minimum Spanning Trees are undirected graphs that connect all the vertices of a given graph 
with the lowest total cost. it is a tree because the structure is acyclic, it is spanning 
because it includes every vertex in the original graph, and it is minimum just because. the 
minimum spanning tree of a graph has V - 1 edges, with V being the number of vertices in the 
original graph. the insertion of a new edge creates a cycle, one which must be broken by 
removing an edge from within the cycle. that edge is the one with the greatest weight. 
	Kruskal's is good for sparse graphs, as it has a complexity of O(E*logE)
	Prim's Algorithm creates a minimum spanning tree by growing it in succcessive stages. each 
stage involves picking a vertex as a root of a tree and adding an edge, and thus a vertex, to 
it. the algorithm works by finding the edge of least cost at every stage between a vertex that
is in the tree and a vertex that hasn't yet been added to it. essentially, that makes the 
algorithm like Dijkstra's width each phrase involving finding the minimum cost between O(E*logV)
Prims is best for dense graphs
	No distinguished vertex as the starting point
*/


/* Critical Path Analysis
	Topological Sort is needed, which essentially involves removing all the vertices that are 
roots, ie have no incoming edges, and enqueue them. repeat this process with the new roots 
that are created by the process, until there are no vertices left. this only works on directed
acyclic graphs
	Critical Path Analysis involves going through the list sorted by the topological sort and 
determining the slack of each of the vertices/tasks, which essentially means how much they can
be delayed. the slack of any given task is equal to the difference between its completion time
and the greatest/longest completion time of the tasks in the given layer. furthermore, the 
sort itself yields important insight as to the minimum time required to complete a given project
or directed acyclic graph 
	The critical path itself is the path in which every edge has a slack of 0 */


/* Articulation Points
	Articulation Points are essentially vertices in a graph that, if removed, would result in 
the graph becoming disjoint, ie it splitting such that not all vertices can reach one another.
They are found by using a depth first search to number all the vertices in the graph, and while
doing so also recoding the least number any given vertex can reach through its first backedge;
	If a given vertex has a child with a low greater than or equal to the least of the number 
of the current vertex, than the vertex is an articulation apoint. this is because the ;
	Depth First Search is performed on a given graph, numbering the vertices as they are passed,
with the number assigned to each vertex being Num(v). Afterward, the minimum of every vertex is
computeted, with the minimum being the least value of Num(v) that can be reached by a given 
vertex by traversing through its edges and a single backedge. this least value is reached by 
taking the first vertex that has a backedge that the vertex can question can access. Low(v) can 
be computed in O(V + E) time, as it essentially involves a postorder traversal;
	NOTE: C is an articulation point IF AND ONLY IF given a vertex G that is a child of C, the 
Low(G) ≥ Num(C). So the number of the vertex must be less than or equal to the lowest traversal
ID that one of its children is connected to 
	Used in the identification of bottlenecks and the desigining of redundancies */

/* Network Flow
Max flow * big-O of dijkstra

