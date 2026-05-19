#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <list>
#include <stdexcept> //for throw
#pragma once
#include <fstream> // for graphviz
#include <string>

template<typename T>
class unidirected_edge {
private:
    T start_node;
    T end_node;

public:
    //constructor with two nodes as input
    unidirected_edge(const T& node1, const T& node2) //forcing from < to
        {if (node1<node2)
            {start_node= node1;
            end_node= node2;}
        else {start_node= node2;
              end_node= node1;}
            }

    T from() const { return start_node;}
    T to() const { return end_node;}

    bool operator<(const unidirected_edge& other) const { // it returns True if my edge<other
        if (this->start_node != other.start_node) { //this-> referenzia al mio oggetto e non other
            return this->start_node < other.start_node;
        }
        else {
            return this->end_node < other.end_node;
        }
    }

    bool operator==(const unidirected_edge& other) const {
        return this->start_node==other.start_node && this->end_node==other.end_node;
    }

};
template<typename T>
std::ostream& operator<<(std::ostream& os, const unidirected_edge<T>& edge) {
        os << "(" << edge.from() << "," << edge.to() << ")";
        return os;
    }

template <typename T, typename W= double> // I add the weights to the edges, they are double if not specified
class unidirected_graph {
private:
    std::set<T> nodes; //I want them ordered
    std::unordered_map<T, std::set<T>> adj_map; //for neighbours, I associate at every node its neighbours
    // I use unordered_map because it is faster when searching in it than map, and I don't care about the order. If T is not int or string there might be some problems with hash functions though, but usually nodes are int or string
    std::set<unidirected_edge<T>> edges; // the set automatically ordinates himself when I add an edge using operator<
    // but you can't access the ith element. You can move around in order though
    std::map<unidirected_edge<T>,W> weights; // I associate the weights to the edges for djikstra
public:
    unidirected_graph()=default;
    unidirected_graph(const unidirected_graph<T>& other) { //costruttore di copia
        nodes= other.nodes;
        adj_map= other.adj_map;
        edges= other.edges;
        weights= other.weights;
    }

    const std::set<T>& neighbours(const T& node) const { // the first const avoids making a copy of the output
        //the second const is standard for the input
        // the third one tells me I'm not modifying objects of my class
        auto it = adj_map.find(node); // auto automatically recognizes the type of it

        if (it!= adj_map.end()) {// if the iterator didn't reach the end it means it found the node in the map
            return it->second; //'it' is an object of the map and is made of the key (first) and the value (second)
        }
        throw std::invalid_argument("The node doesn't exist in the graph"); //throw returns an error

    }

    void add_edge(const T& node1, const T& node2, W weight=1) {// weight is 1 by default
        // I don't use const & for weight because reference occupies 8 bytes, int or float 4 and double 8 so usinge reference doesn't improve the performance
        unidirected_edge<T> new_edge(node1,node2);
        auto result = edges.insert(new_edge); //insert returns an iterator (first) of the position of insertion (or the position of the copy of the edge already present)
        // and (second) true if the insertion was successfull, false otherwise
        if (result.second==true) { //if everything goes well (the edge wasn't already there) I add the node to the sets and maps
            nodes.insert(node1);
            nodes.insert(node2);
            adj_map[node1].insert(node2);
            adj_map[node2].insert(node1);
            weights[new_edge]= weight;
        }
    }

    W get_weight(const T& node1, const T& node2) const {
        unidirected_edge<T> edge(node1,node2);
        auto it= weights.find(edge);
        if (it!=weights.end()) {
            return it->second;
        }
        throw std::invalid_argument("The edge doesn't exist");
    }

    const std::set<unidirected_edge<T>>& all_edges() const {return edges;}
    const std::set<T>& all_nodes() const {return nodes;}

    int edge_number(const unidirected_edge<T>& edge) const {

        auto result = edges.find(edge); //I first see if the edge exists (fast)
        if (result!=edges.end()) { //if it does exist I look for the position
            int counter=0;
            for (auto it = edges.begin(); it!=result; ++it) { //when it==result I stop
                //that is faster than checking if the edges match because I compare memory addresses rather than objects
                counter++;
            }
            return counter;
        }

        throw std::invalid_argument("The edge doesn't exist in the graph");

    }

    const unidirected_edge<T>& edge_at(size_t number) const { // use size_t because .size() returns a size_t
        if (number >= edges.size()) {// out of range
            throw std::out_of_range("Indice dell'arco oltre il limite");
        }

        auto counter = edges.begin(); //I put myself at the start of edges
        for (size_t it = 0; it < number; it++) { //and I go on until the index is the same as the input
            ++counter;
        }
        return *counter;
    } 

    unidirected_graph<T,W> operator-(const unidirected_graph<T,W>& other) const {
        unidirected_graph<T,W> difference_graph; //this will be my result

        for (const auto& first_edge : edges) { //iterating on first graph's edges
            if (other.edges.find(first_edge) == other.edges.end()) {//if it's not present in g2
                difference_graph.add_edge(first_edge.from(), first_edge.to());// I add it to the result
            }
        }
        return difference_graph;
    }
    
    // Metodo per esportare il grafo in formato Graphviz (DOT)
    void print_dot(const std::string& filename) const {
        std::ofstream out(filename);
        
        if (!out.is_open()) {
            throw std::runtime_error("Impossibile aprire il file per la scrittura.");
        }

        out << "graph G {\n";
        out << "  node [shape=circle, style=filled, fillcolor=lightblue];\n";
        out << "  edge [color=darkgray, fontcolor=red];\n";

        // Stampiamo gli archi e i pesi
        for (const auto& [edge, weight] : weights) {
            out << "  " << edge.from() << " -- " << edge.to()
                << " [label=\"" << weight << "\"];\n";
        }

        //Stampiamo i nodi isolati 
        for (const T& node : nodes) {
            if (adj_map.find(node) == adj_map.end() || adj_map.at(node).empty()) {
                out << "  " << node << ";\n";
            }
        }

        // Chiudiamo la parentesi del grafo
        out << "}\n";
        
        out.close();
    }
};

template<typename T,typename W>
std::ostream& operator<<(std::ostream& os, const unidirected_graph<T,W>& graph)
{
    auto the_edges = graph.all_edges();
    size_t n = the_edges.size();
    size_t i = 0; 
    
    os << "{";
    for(const auto& edge : the_edges)
    {
        os << edge;
        if(i != n - 1) os << ", "; //so I don't print "," after the last edge
        i++;
    }
    os << "}\n";
    return os;
}

