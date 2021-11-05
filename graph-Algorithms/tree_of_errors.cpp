#include <sstream>
#include <iostream>
#include <stack>

const int num_alph = 26;


// defining class to hold whether or not a given vertex 
// has been visited and its edges; member variables are
// added to check for the errors, roles explained 
class Node {
    public:
        // used to check for error 2, as edges are checked whenever
        // an edge is created to see if duplicate exists; also holds edges;
        int* edges;
        // used to check for error 3, multiple children;
        int num_edges;
        // used to check for error 4 and 5; 4 if multiple vertices unvisited
        // then multiple roots, 5 if same vertex is visited twice in construction;
        bool visited;
        // identity of the actual Node, needed to check for duplicates; if -1, means
		// that the node does not exist in the graph/binary tree;
        int id; 
        // default constructor function;
        Node(): num_edges(0), visited(false), id(-1) {
            edges = new int[num_alph];
        }
        // input constructor function;
        Node(int _id): num_edges(0), visited(false), id(_id) {
            edges = new int[num_alph];
        }
};

// a class used to construct binary tree, and return errors if any
class BinaryTree {
    public:
        // creating an array of all the possible nodes;
        Node* nodes;
		// resulting string to be output, either binary tree in S-expression
		// or the respective error;
  		std::string result;
		// value holding the "root" of the tree, for starting BFS;
        int root;
		// default constructor
        BinaryTree(): root(-1) {
            Node* nodes = new Node[num_alph];
        }
        BinaryTree(std::string input): root(-1) {
            nodes = new Node[num_alph];
          	// Constructs Graph and also Checks for E1, E2, E3 Errors
            int error = _construct(input);
			// Check if errors E1, E2, or E3
			if(error > 0) this->result = "E" + std::to_string(error);
            else {
				// Check for Errors E4, E5; otherwise Print Tree
                error = checkGraph();
                if(error > 0) this->result = "E" + std::to_string(error);
                else this->result = "("+ representTree(nodes[root]) + ")";
            }
        }
        // creating helper function for creating the graph, note that in creating the graph
  		// E1, E2, and E3 can be checked for;
        int _construct(std::string input) {
          	// creating an array of errors, returning one with the highest
          	// precedence at the end of the call
          	int error = -1;
            // using std::string stream to extract the values
            std::istringstream iss(input);
            do {
                // extracting the edges using expected structure, checking for E1
                std::string edge;
                iss >> edge;
				if(edge == "" || edge== "\n") break;
                if(!(edge.length() == 5 && edge[0] == '(' && isalpha(edge[1]) && edge[2] == ',' \
                    && isalpha(edge[3]) && edge[4] == ')')) {
                    error = 1;
                    return error;
                }    
                // if an E2 is caught, there is no need to check for anything except for E1
              	if(error == 2) continue;
                // extracting the vertices using verified expected structure; 
                char char_1 = edge[1]; char char_2 = edge[3];
              	int adjusted_1 = char_1-'A'; int adjusted_2 = char_2-'A';
				// setting the vertices as real on the graph
				nodes[adjusted_1].id = adjusted_1; nodes[adjusted_2].id = adjusted_2;
                // checking if the edge added is a duplicate, checking for E2
                for(int i=0; i<nodes[adjusted_1].num_edges; ++i) {
                    if(nodes[adjusted_1].edges[i] == adjusted_2) {
                        error = 2;
                        break;
                    }
                }
            	// checking if edge added will result in more than 2 children, checking for E3
              	if(nodes[adjusted_1].num_edges >= 2) {
                  	error = 3;
                }
              	// NOTE, the edges need to be added even when error 3 occurs, because if E3 occurs 
                // and edge is not added, but a duplicate would exist in later (num > 3), E2 would be missed
                Node& parent = nodes[adjusted_1];
              	parent.edges[parent.num_edges] = adjusted_2;
                parent.num_edges++;
				// showing that the child vertex has been visited, is not a root
				nodes[adjusted_2].visited = 1;
			} while(iss);
          	return error;
        }
  		int checkGraph(void) {
          	// finding roots, checking for multiple roots, checking for E4
          	int error = -1; bool exists = false; 
          	for(int i=0; i<num_alph; ++i) {
				// checking if the node exists in the graph
				if(nodes[i].id < 0) continue;
				// checking if the node is a root, and another root already exists
				// returning as E1-3 are no longer possible
				if(exists && !nodes[i].visited) {
					error = 4;
					return error;
				}
				if(nodes[i].visited == 0) {
					exists = true;
					this->root = i;
				}
            } 
          	// creating array of values to check if visited, finding cycles
          	int visited[num_alph] = {0};
          	// performing DFS using root found to find cycles, checking for E5
          	std::stack<int> stack_node;
          	stack_node.push(this->root);
          	while(!stack_node.empty()) {
              	int node = stack_node.top();
                stack_node.pop();
              	// check for cycle error, returning as E4 is no longer possible
              	if(visited[node] == 1) {
                  	error = 5;
                  	return error;
                }
              	// mark node as visited
              	visited[node] = 1;
                // adding the remaining edges
              	for(int i=0; i<nodes[node].num_edges; ++i) {
                  	stack_node.push(nodes[node].edges[i]);
                }
            }
            return error;
        }
        std::string representTree(Node node) {
			// checking if the node has no children
			if(node.num_edges == 0) {
				return std::string(1, char(node.id+'A'));
			}
			// checking if the node has only one child
			if(node.num_edges == 1) {
				return std::string(1, char(node.id+'A')) + "(" + representTree(nodes[node.edges[0]]) + ")";
			}
			// if the node has two children (verified by the problem), order alphabetically
			if(node.edges[1] < node.edges[0]) {
				return std::string(1, char(node.id+'A')) + "(" + representTree(nodes[node.edges[1]]) + ")" \
					 + "(" + representTree(nodes[node.edges[0]]) + ")";
			}
			else {
				return std::string(1, char(node.id+'A')) + "(" + representTree(nodes[node.edges[0]]) + ")" \
					 + "(" + representTree(nodes[node.edges[1]]) + ")";
			}
		}
};

int main() {
    std::string input;
    getline(std::cin, input);
	// calling the class constructor for the graph, and solving the 
	// resulting graph for errors in O(n) time
    BinaryTree graph = BinaryTree(input);
    std::cout << graph.result << std::endl;
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    return 0;
}