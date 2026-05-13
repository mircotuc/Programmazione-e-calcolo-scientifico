#include "graph.hpp"
#include <iostream>
#include <string>

using namespace std;

//Test inser
bool test_inser() {
    unidirected_graph<int> g;
    g.add_edge(1,2);

    if (g.all_edges().size() == 1 && 
        g.all_nodes().size() == 2 && 
        g.all_nodes().contains(1) && 
        g.all_nodes().contains(2)) //check nodes 1 and 2 are present
    {
        cout << "Test inser: PASSED" << endl;
        return true;
    } 
    else {
        cout << "Test inser: FAILED!" << endl;
        return false;
    }
}

// Test bijective mapping
bool test_bijective_mapping() {
    unidirected_graph<string> g;
    g.add_edge("Altamura", "Cosenza");
    unidirected_edge<string> e1("Altamura", "Cosenza");
    size_t id = g.edge_number(e1);
    unidirected_edge<string> e2 = g.edge_at(id);

    if (id == 0 && e1==e2) {
        cout << "Test Mapping: PASSED" << endl;
        return true;
    } 
    else {
        cout << "Test Mapping: FAILED!" << endl;
        return false;
    }
}

//Test neighbours
bool test_neighbours() {
    unidirected_graph<int> g;
    g.add_edge(1, 2);
    g.add_edge(1, 3);

    auto v = g.neighbours(1);

    if (v.size() == 2 && v.contains(2) && v.contains(3)) {
        cout << "Test Neighbours: PASSED" << endl;
        return true;
    } 
    else {
        cout << "Test Neighbours: FAILED!" << endl;
        return false;
    }
}

//Test difference
bool test_difference() {
    unidirected_graph<int> g1, g2;
    g1.add_edge(1, 2);
    g1.add_edge(2, 3);
    
    g2.add_edge(2, 3); // Arco comune

    unidirected_graph<int> difference = g1 - g2;
    const auto& diff_edges = difference.all_edges();

    if (difference.all_edges().size() == 1 && 
        difference.edge_number({1, 2}) == 0 && 
        (diff_edges.find({2,3}) == diff_edges.end())
        ) 
    {
        cout << "Test difference: PASSED" << endl;
        return true;
    } 
    else {
        cout << "Test difference: FAILED!" << endl;
        return false;
    }
}

int main() {
    cout << "=== STARTING UNIT TESTS ===\n";
    bool success = true;
    if(!test_inser()||!test_bijective_mapping()||!test_neighbours()||!test_difference()) success = false;
    
    if(success){
        cout << "ALL TEST PASSED" <<endl;
        return EXIT_SUCCESS;
    }
    else
    {
        cout<< "FAILURE"<<endl;
        return EXIT_FAILURE;
    }
}