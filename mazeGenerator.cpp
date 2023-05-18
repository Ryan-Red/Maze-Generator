#include <iostream>
#include <vector>
 

struct Node{
    int x;
    int y;
    std::vector<std::vector<int>> adj;
};

template < class T >
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) 
{
    os << "[";
    for (typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii)
    {
        os << " " << *ii;
    }
    os << " ]";
    return os;
}

// A utility function to add an edge in an
// undirected graph.
void addEdge(std::vector<std::vector<Node>> &nodesList, std::vector<int> first, std::vector<int> second)
{
    std::cout << "Added" << std::endl;
    nodesList[first[0]][first[1]].adj.emplace_back(second);
    nodesList[second[0]][second[1]].adj.emplace_back(first);

    std::cout << nodesList[first[0]][first[1]].adj << std::endl;

}
 
// A utility function to print the adjacency list
// representation of graph
void printGraph(std::vector<std::vector<Node>> nodesList, int n, int m)
{
    int i, j = 0;

    for(i = 0; i < n; i ++){
        for(j = 0; j < m; j++){
            std::vector<std::vector<int>> v = nodesList[i][j].adj;
            std::cout << "\n Adjacency list of vertex (" << i << ", " << j << ")\n head ";

            for (auto x : v)
                std::cout << " -> (" << x[0] << ", " << x[1] << ")";
            std::cout << std::endl;


        }
    }

}
 
// Driver code
int main()
{

    int n = 3;
    int m = 3;

    std::vector<std::vector<Node>> nodesList(n,std::vector<Node>(m));

    int i, j = 0;

    for(i = 0; i < n; i ++){
        for(j = 0; j < m; j++){
            nodesList[i][j].x = j;
            nodesList[i][j].y = i;
        }
    }

    addEdge(nodesList, {0,0}, {1,0});
    addEdge(nodesList, {0,0}, {2,0});
    addEdge(nodesList, {0,1}, {1,0});


    // int V = 5;
    // std::vector<int> adj[V];
    // addEdge(adj, 0, 1);
    // addEdge(adj, 0, 4);
    // addEdge(adj, 1, 2);
    // addEdge(adj, 1, 3);
    // addEdge(adj, 1, 4);
    // addEdge(adj, 2, 3);
    // addEdge(adj, 3, 4);
    printGraph(nodesList, n, m);
    return 0;
}