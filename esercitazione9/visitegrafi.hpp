#pragma once
#include "graph.hpp"
#include "containers.hpp"
#include <set> // for the visited nodes
#include <stdexcept> // for throw
#include <limits> //for infinite
#include <unordered_map>

template<typename T, typename C> //C is for the container to use
unidirected_graph<T> graph_visit(const unidirected_graph<T>& g, const T& source, C& cont) { //I don't put const because when I call put,get,empty I'm modifying the container
    unidirected_graph<T> solution_tree;
    std::set<T> reached; // when I visit a node I add it here

    auto nodes= g.all_nodes();
    if (nodes.find(source) == nodes.end()) { //check if the source node is in the graph
        throw std::invalid_argument("The source node is not present in the graph");
    }

    cont.put(source);
    reached.insert(source);
    // we don't consider cases with graphs without edges
    while (!cont.empty()) {
        T u=cont.get();
        for (const auto& w : g.neighbours(u)) { //we consider only nondirected graphs, unlike the pseudocode
            if (reached.find(w)==reached.end()) { //check if not already visited
                cont.put(w);
                reached.insert(w); //It's better to do it here rather than outside the for as in the pseudocode
                //so I don't have to add the if (reached[u]), continue present in the DFS pseudocode
                solution_tree.add_edge(u,w); // when adding the edge I also add the nodes
            }
        }
    }
    return solution_tree;
};

template<typename T>
void recursive_dfs_fake(const unidirected_graph<T>& g, const T& source, std::set<T>& reached, unidirected_graph<T>& solution_tree) {
    for (const auto& w : g.neighbours(source)) { //we consider only nondirected graphs, unlike the pseudocode
        if (reached.find(w)==reached.end()) { //check if not already visited
            reached.insert(w); //It's better to do it here rather than outside the for as in the pseudocode
            //so I don't have to add the if (reached[u]), continue present in the DFS pseudocode
            solution_tree.add_edge(source,w); // when adding the edge I also add the nodes
            recursive_dfs_fake(g,w,reached,solution_tree); // if w has only source as neighbour the function does nothing when called inside the for
        }  // with the recursion we explore by branches
    }
};

template<typename T>
unidirected_graph<T> recursive_dfs(const unidirected_graph<T>& g, const T& source) {
    unidirected_graph<T> solution_tree;
    auto nodes= g.all_nodes();
    if (nodes.find(source) == nodes.end()) { //check if the source node is in the graph
        throw std::invalid_argument("The source node is not present in the graph");
    }
    std::set<T> reached;
    reached.insert(source);

    recursive_dfs_fake(g,source,reached, solution_tree);

    return solution_tree;
    
};


template <typename T,typename W=double>
unidirected_graph<T,W> dijkstra(const unidirected_graph<T,W>& g, const T& source) {
    unidirected_graph<T,W> solution_tree;
    auto nodes= g.all_nodes();
    if (nodes.find(source) == nodes.end()) { //check if the source node is in the graph
        throw std::invalid_argument("The source node is not present in the graph");
    }
    std::unordered_map<T,W> dist; //I use unordered_map because faster when searching in it. The problem would be if T is not int but usually it is. In that case we use map
    std::unordered_map<T,T> pred;

    //I need to initialize them, so I need the infinite
    W infinite;
    if (std::numeric_limits<W>::has_infinity) {
        infinite= std::numeric_limits<W>::infinity(); //if infinite is supported use that
    }
    else { infinite = std::numeric_limits<W>::max();} //otherwise take the max

    for ( const auto& node: g.all_nodes()) {
        dist[node]=infinite;
    }
    dist[source]=0;

    PQ<T,W> pq;
    pq.put(source,0); //useless to put all nodes already, we'll add them only if reachable

    while (!pq.empty()) {
        PQelement<T,W> current= pq.get(); // current is a PQelement so I can use the names of my struct
        W current_dist = current.dist;
        T current_node = current.node;

        if (current_dist > dist[current_node]) { // if the current distance is higher than the one already found for that node we start again
            continue;
        }

        for (const T& w: g.neighbours(current_node)) { //iterating on the neighbours of current node
            W edge_weight = g.get_weight(current_node,w);
            W new_dist = current_dist + edge_weight; // I update the distnace with the distance from source+ distance for new edge
            if (dist[w]> new_dist) {
                dist[w]= new_dist;
                pred[w]= current_node;
                pq.put(w,new_dist); //update the prioriy queue
            }
        }
    }

    //now i need to reconstruct the solution_tree using pred
    // I didn't put the source in pred so I don't need to worry about it, it can't be a child
    for (const auto& [child, parent] : pred ) { //[,] gives me the pair child parent, must put auto, T gives me error
        W edge_weight = g.get_weight(parent,child);
        solution_tree.add_edge(parent,child,edge_weight);
    }
    return solution_tree;

}


 


