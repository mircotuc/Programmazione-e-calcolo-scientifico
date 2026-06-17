#include "classegrafiprogetto.hpp"
#include <iostream>
#include <string>

using namespace std;

//questa funzione stampa il grafo in modo da poterlo vedere
//su graphviz
//funzione ddi stampa modificata per dare l'output giusto per graphviz
template<typename T>
void stampa_grafo(const unidirected_graph<T>& g) {
    for (const unidirected_edge<T>& arco : g.all_edges()) {
        std::cout << " " << arco.from() << " -- " << arco.to() << "\n";
    }        
}

//Test inser: verifica che add edge aggiunga l'arco e gli estremi, se questi non sono presenti
bool test_inser() {
    unidirected_graph<int> g;
    g.add_edge(1,2);

    if (g.all_edges().size() == 1 && 
        g.all_nodes().size() == 2 && 
        g.all_nodes().contains(1) && 
        g.all_nodes().contains(2)) //verifica che i nodi 1 e 2 siano stati aggiunti
    {
        cout << "Test inser: PASSED" << endl;
        return true;
    } 
    else {
        cout << "Test inser: FAILED!" << endl;
        return false;
    }
}



//test edge_number, edge.at()
bool test_bijective_mapping() {
    unidirected_graph<int> g1; /*inizializzo un grafo<int> vuoto*/
    
    g1.add_edge(0, 1); //sfrutto il fatto che add edge aggiunge anche i nodi se questi
    //non sono già nel grafo
    g1.add_edge(0, 2); 
    g1.add_edge(1, 2);
    g1.add_edge(2, 3);
    g1.add_edge(3, 4);
    g1.add_edge(4, 5);
    
    for (const unidirected_edge<int> e1 : g1.all_edges()){ //per ogni edge
        size_t id = g1.edge_number(e1); //estraggo il numero dell'arco

        if (id > g1.all_edges().size()-1) { // Gestiamo il caso in cui l'id è out of range (non lo gestisco direttamente nella classe graph)
            cout << "Test Mapping: FAILED!" << endl;
            return false;
        }
        unidirected_edge<int> e2= g1.edge_at(id); //vado a trovare l'arco con quel numero 

        if (e1==e2){//se l'arco iniziale è uguale all'arco indicato dal suo numero 
            //passo avanti
        } 
        
        else{
            cout << "Test Mapping: FAILED!" << endl;
            return false; //se sono diversi ritorno falso e si arresta la funzione
        }
          
    }
    cout << "Test Mapping: PASSED" << endl;
    return true; //se sono tutti uguali ritorno true 
}

//Test neighbours: 
bool test_neighbours() {
    unidirected_graph<int> g;
    g.add_edge(1, 2); 
    g.add_edge(1, 3);

    auto v = g.neighbours(1);

    if (v.size() == 2 && v.contains(2) && v.contains(3)) {
        //se v ha la dimensione giusra e contiene 2 e 3 supera il test
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
    g1.add_edge(0, 1); //sfrutto il fatto che add edge aggiunge anche i nodi se questi
    //non sono già nel grafo
    g1.add_edge(0, 2); 
    g1.add_edge(1, 2);
    g1.add_edge(2, 3);
    g1.add_edge(3, 4);
    g1.add_edge(4, 5);
    std::cout << "\n g1: \n";
    stampa_grafo(g1);

    g2.add_edge(0, 2); // Arco in comune
    g2.add_edge(3, 6); // Arco inesistente in g1
    //il grafo differenza non deve avere l'arco (0,2) né l'arco (3,6)
    std::cout << "\n g2: \n";
    stampa_grafo(g2);

    unidirected_graph<int> difference = g1 - g2;
    std::cout << "\n difference: \n";
    stampa_grafo(difference);

    const auto& diff_edges = difference.all_edges();

    if (difference.all_edges().size() == 5 && 
        difference.edge_number(unidirected_edge<int>(1, 2)) == 1 && 
        diff_edges.find(unidirected_edge<int>(3, 6)) == diff_edges.end()
        ) 
        //se nel grafo dufferenza ci sono 5 archi, l'arco (1,2) è in posizione 1 e 
        // l'arco (1,2) non è presente
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
    if(!test_inser()||!test_bijective_mapping()||!test_neighbours()||!test_difference()) {
        success = false;
        //se almeno un test fallisce ritorna false
    }
    
    if(success){ //sarebbe success ==true
        cout << "ALL TEST PASSED" <<endl;
        return EXIT_SUCCESS;
    }
    else
    {
        cout<< "FAILURE"<<endl;
        return EXIT_FAILURE;
    }
}