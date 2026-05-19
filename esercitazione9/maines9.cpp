#include <iostream>
#include "containers.hpp"
#include "graph.hpp"
#include "visitegrafi.hpp"

using namespace std;

int main() {
    unidirected_graph<int> G;

    // test
    G.add_edge(1,2);G.add_edge(1,3);G.add_edge(1,4);G.add_edge(1,6);G.add_edge(2,4);
    G.add_edge(2,5);G.add_edge(2,7);G.add_edge(4,6);G.add_edge(4,7);G.add_edge(3,6);
    G.add_edge(5,7);G.add_edge(6,7);G.add_edge(6,8);G.add_edge(8,9);G.add_edge(7,9);
    
    cout << "BFS,DFS,Recursive DFS on graph:\n" << G <<endl;
    // DFS
    lifo<int> s;
    unidirected_graph<int> dfsG = graph_visit(G, 1, s);
    cout << "dfs"<< endl;
    cout << dfsG << endl;
    dfsG.print_dot("dfs.dot");

    // BFS
    fifo<int> q;
    unidirected_graph<int> bfsG = graph_visit(G, 1, q);
    cout << "bfs" << endl;
    cout << bfsG << endl;
    bfsG.print_dot("bfs.dot");
    

    // Recursive DFS
    unidirected_graph<int> dfsrG = recursive_dfs(G, 1);
    cout << "recursive dfs" << endl;
    cout << dfsrG << endl;
    dfsrG.print_dot("recursivedfs.dot");
    
    G.print_dot("original_graphG.dot");

    // test dijkstra
    unidirected_graph<int> G1;

    G1.add_edge(1, 2, 5);G1.add_edge(1, 3, 1);G1.add_edge(2, 3, 2);G1.add_edge(2, 4, 1);G1.add_edge(3, 4, 7);
 
    cout << "\nDijkstra on graph:" << endl;
    cout << G1<<endl;

    int source = 1;
    cout << "Starting from node: " << source << endl;
    
    unidirected_graph<int, double> solution_tree = dijkstra(G1, source);

    cout << "\nSolution tree:" << endl;
    cout << solution_tree <<endl;

    G1.print_dot("original_graphG1.dot");
    solution_tree.print_dot("solution_tree.dot");

    //test dijkstra with strings
    unidirected_graph<string> G2;

    G2.add_edge("Torino", "Milano",2);
    G2.add_edge("Torino", "Genova",7);
    G2.add_edge("Milano", "Genova",3);
    G2.add_edge("Milano", "Bologna",6);
    G2.add_edge("Genova", "Bologna",1);
    G2.add_edge("Milano", "Venezia",11);
    G2.add_edge("Bologna", "Venezia",2);
    G2.add_edge("Venezia", "Roma",10);
    G2.add_edge("Bologna", "Roma",4);

    //add isolated nodes
    G2.add_edge("Cagliari", "Sassari", 5);

    
    cout << "\nDijkstra on string graph:\n"<<G2<< endl; 
    string sorg = "Torino";
    cout << "starting from: " << sorg <<"\n"<< endl;
    
    unidirected_graph<string, double> solution_tree_string = dijkstra(G2, sorg);
    cout << "\nSolution tree:" << endl;
    cout << solution_tree_string <<endl;

    G2.print_dot("original_graphG2.dot");

    solution_tree_string.print_dot("solution_tree_string.dot");

    return 0;
}

