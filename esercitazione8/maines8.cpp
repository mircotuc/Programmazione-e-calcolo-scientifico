#include "graph.hpp"
#include <iostream>
#include <string>

using namespace std;
int main(){ //just to visualize things
    cout << "Inser test" << endl;
    unidirected_graph<int> g1;
    g1.add_edge(1, 3);
    g1.add_edge(3, 1);  // this shouldn't be added
    g1.add_edge(2, 60);
    g1.add_edge(3, 2);

    cout << "Graph G1: " << g1 << endl; //using my overload of <<
    cout << "Nodes: ";
    for(auto n : g1.all_nodes()) {cout << n << " ";}
    cout << endl;
    cout << "Neighbours test" << endl;


    int nodo_test = 3;
    cout << "Neighbours of node " << nodo_test << ": ";
    auto neighbours = g1.neighbours(nodo_test);
    for(auto v : neighbours) cout << v << " ";
    cout << "\n" << endl;


    cout << "Bijective test (with inverted edge)" << endl; // since the graph is undirected 
    unidirected_edge<int> e1(60, 2); 
    size_t id = g1.edge_number(e1);
    
    cout << "Index of (60,2): " << id << endl;
    cout << "And the edge at index " << id << " is " << g1.edge_at(id) << endl;

    cout << "Test difference" << endl;
    unidirected_graph<int> g2;
    g2.add_edge(1, 3); 
    
    cout << "G1: " << g1;
    cout << "G2: " << g2;
    
    unidirected_graph<int> g_diff = g1 - g2;
    cout << "G1-G2: " << g_diff << endl;

    cout << "String test" << endl;
    unidirected_graph<string> g_str;
    g_str.add_edge("Altamura", "Cosenza");
    g_str.add_edge("Cosenza", "Monterotondo");
    cout << "Grafo di città: " << g_str << endl;

    return 0;
}