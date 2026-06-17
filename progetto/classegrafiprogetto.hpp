#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <list>
#pragma once
#include <fstream>
#include <string>

template<typename T>
class unidirected_edge {
private:
    T start_node;
    T end_node;
    
public:
    //costruttore con due nodi di input
    unidirected_edge(const T& node1, const T& node2) //ordiniamo automaticamente
        {if (node1<node2)
            {start_node= node1;
            end_node= node2;}
        else {start_node= node2;
              end_node= node1;}
            }

    T from() const { 
        return start_node;
    } //il const avvisa che le funzioni non modificheranno
    //gli elementi della mappa
    T to() const { 
        return end_node;
    }

    bool operator<(const unidirected_edge& other) const { //ritorna true le il mio arco inizia con un nodo
        // < dell'altro. se i due nodi iniziali sono uguali, confronta quelli finali
        if (start_node != other.start_node) { 
            return start_node < other.start_node; 
        }
        else {
            return end_node < other.end_node;
        }
    }

    bool operator==(const unidirected_edge& other) const {
        return start_node==other.start_node && end_node==other.end_node;
    }

};


template<typename T>
std::ostream& operator<<(std::ostream& os, const unidirected_edge<T>& edge) {
        os << "(" << edge.from() << "," << edge.to() << ")";
        return os;
    }



template <typename T> 
class unidirected_graph {
private:
    std::set<T> nodes; //in questo modo i nodi saranno ordinati 
    std::unordered_map<T, std::set<T>> adj_map; //per i vicini, associo a ogni nodo (chiave) il set dei suoi vicini
    //attenzione: se i nodi non sono né int né string ci potrebbero essere problemi nelle 
    // hash functions. nel caso del nostro progetto, il grafo contiene degli int. 
    // la velocità di ricerca in una unordered map è, in genere, maggiore rispetto ad una ordered map, ma
    // può arrivare fino ad O(n)  
    
    
    std::set<unidirected_edge<T>> edges; //il set si riordina automaticamente quando aggiungo un arco. per farlo, usa l'operatore
    // < definito prima. A differenza dei vettori, tuttavia, non possiamo accedere all'i-esimo elemento.
    
public:


    unidirected_graph() {}; //costruttore di default


    unidirected_graph(const unidirected_graph<T>& other) { //costruttore di copia
        nodes= other.nodes;
        adj_map= other.adj_map;
        edges= other.edges;
    }


    std::set<T> neighbours(const T& node) const {
        //il primo const mi dice che il nodo non verrà modificato (standard per l'input), il secondo mi dice che nessun elemento 
        //della classe verrà modificato
    
        auto it = adj_map.find(node); // auto riconosce automaticamente il tipo di it
        //find restituisce un iteratore che punta alla coppia (chiave,valore) della mappa. 
        //estraggo la chiave con ->first e 
        // l'elemento con -> second.
        // se find non trova node nella mappa, restituisce un puntatore "oltre la fine"
        if (it!= adj_map.end()) {// se l'iteratore non è arrivato alla fine della mappa, 
            //vuol dire che ha trovato il nodo nella mappa 
            return it->second; // 'it->second' è l'elemento della mapppa 
            
        }
        std::cerr << "ERRORE: nodo non trovato nella lista di adiacenza" << std::endl;
        return std::set<T>(); //se il nodo non è stato trovato, la funzione restituisce un set vuoto

    }


    bool add_edge(const T& node1, const T& node2) {
        unidirected_edge<T> new_edge(node1,node2);
        auto result = edges.insert(new_edge); //se l'arco non era nel grafo, insert inserisce l'arco e 
        //restituisce un iteratore (first) con la posizione dell'arco inserito e true (second);
        //se l'arco era già nel grafo, insert restituisce un iteratore  con la sua posizione (first) e false (second)
        if (result.second==true) { //se va tutto bene (ossia l'arco non era già presente), aggiungo il nodo ai set e alle mappe
            nodes.insert(node1);
            nodes.insert(node2);
            adj_map[node1].insert(node2);
            adj_map[node2].insert(node1);
            return true;
        }
        else {
            return false;
        }
    }

    const std::set<unidirected_edge<T>>& all_edges() const {return edges;}
    const std::set<T>& all_nodes() const {return nodes;}
    //l'output di queste due funzioni sarà costante


    //gli elementi della classe non saranno modificati
    int edge_number(const unidirected_edge<T>& edge) const {

        auto result = edges.find(edge); //per prima cosa vedo se l'arco esiste O(log(n))
        if (result!=edges.end()) { //se esiste cerco la posizione
            int counter=0; 
            for (auto it = edges.begin(); it!=result; ++it) { //quando it==result il ciclo si ferma. 
                //questo procedimento è più veloce rispetto a controllare se gli archi sono uguali, perché sto confrontando
                //indirizzi di memoria anziché oggetti
                
                counter++;
            }
            return counter;
        }

        std::cerr << "ERRORE: l'arco cercato non esiste nel grafo" << std::endl;
        return -1; // (nel depina il metodo sarà sempre utilizzato ciclando su archi esistenti)

    }


    const unidirected_edge<T>& edge_at(size_t number) const { //usiamo size_t perché .size() resituisce un 
        //oggetto di tipo size_t
        
        if (number >= edges.size()) {// out of range
            std::cerr << "ERRORE: l'indice assegnato non è associato a nessun arco" << std::endl;
        }

        auto counter = edges.begin(); //I put myself at the start of edges
        for (size_t it = 0; it < number; it++) { //and I go on until the index is the same as the input
            ++counter;
        }
        return *counter;
    }


    unidirected_graph<T> operator-(const unidirected_graph<T>& other) const {
        unidirected_graph<T> difference_graph; //questo sarà il risultato
        difference_graph.nodes= nodes; // aggiungo tutti i nodi cosi non perdo i nodi isolati

        for (const auto& first_edge : edges) { //itero sugli archi del primo grafo
            if (other.edges.find(first_edge) == other.edges.end()) {//se l'arco non è presente in g2
                difference_graph.add_edge(first_edge.from(), first_edge.to());//lo aggiungo al risultato
            }
        }
        return difference_graph;
    }
};