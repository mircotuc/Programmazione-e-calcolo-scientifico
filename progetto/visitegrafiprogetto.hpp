#pragma once
#include "classegrafiprogetto.hpp"
#include <set> 
#include <limits> //per infinite
#include <map>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> //per reverse

template<typename T>
void recursive_dfs_fake(const unidirected_graph<T>& g, const T& u, std::map<T,bool>& reached, unidirected_graph<T>& solution_tree) {
    reached[u]=true;
    for (const auto& w : g.neighbours(u)) { //consideriamo solo grafi non diretti, non come negli esempi del professore
        if (!reached[w]) { //controlliamo non sia ancor stato visitato
            solution_tree.add_edge(u,w); // aggiungiamo arco e nodi al grafo soluzione
            recursive_dfs_fake(g,w,reached,solution_tree); // se w ha solo il nodo u come vicino questa funzione non fa nulla
        }  // stiamo quindi esplorando per rami
    }
};

template<typename T>
unidirected_graph<T> recursive_dfs(const unidirected_graph<T>& g, const T& source) {
    unidirected_graph<T> solution_tree;
    std::set<T> nodes= g.all_nodes();
    if (nodes.find(source) == nodes.end()) { // controlliamo che il nodo source sia nel grafo, altrimenti restituiamo grafo vuoto
        std::cerr <<"ERRORE: la radice data in input non è nel grafo" << std::endl;
        return unidirected_graph<T>();
    }
    std::map<T,bool> reached;
    for (const auto& u : nodes) {
        reached[u]=false; // inizializziamo a tutti false
    }

    recursive_dfs_fake(g,source,reached, solution_tree);

    return solution_tree;
    
};




template<typename T>
std::vector<T> dijkstra_modificato(const unidirected_graph<T>& G, const T& nodo_start, const T& nodo_end) { 
    
    std::vector<T> cammino_minimo; //inizializziamo
    // controlliamo che i nodi start e end siano nel grafo, altrimenti restituiamo vettore vuoto
    std::set<T> nodi = G.all_nodes();
    if (nodi.find(nodo_start) == nodi.end() || nodi.find(nodo_end) == nodi.end()) { 
        std::cerr <<"ERRORE: uno o entrambi dei nodi dati in input non sono nel grafo" << std::endl;
        return std::vector<T>();
    }

    // Gestione del caso base: partenza e arrivo coincidono
    if (nodo_start == nodo_end) {
        cammino_minimo.push_back(nodo_start);
        return cammino_minimo;
    }

    std::map<T,T> pred; // mappa con (nodo,predecessore) 
    std::map<T,int> dist; //mappa con (nodo, distanza dal nodo sorgente) e poichè il grafo ha pesi unitari usiamo int
    // possiamo usare eventualmente una unordered_map

    int inf = std::numeric_limits<int>::max();
    for (const auto& u : nodi) { // inizializziamo la mappa con tutti inf (in realtà con i max)
        dist[u] = inf;
    }

    dist[nodo_start] = 0; // il nodo sorgente ha distanza nulla da sè stesso

    // inizializziamo la coda con priorità (di pair <distanza,nodo>)
    // la priority queue è implementata tramite un heap (albero) che ha di default come radice il nodo con il valore più elevato (con criterio std::less)
    // e dove i nodi con valori più alti stanno in alto nell'albero ( i figli hanno sempre valori più piccoli dei padri)
    // Quindi se si decide di estrarre da essa un elemento viene estratto il nodo con distanza più alta (la radice) che si trova in cima all'albero
    // Poichè noi vogliamo il contrario, utilizziamo il criterio std::greater che inverte la priorità, in modo da estrarre il nodo con distanza minima
    // è anche questo il motivo per cui si inializza la map con tutti inf (almeno i nodi con dist=inf non vengono estratti e si troveranno
    // in cima all'albero)
    std::priority_queue<
    std::pair<int,T>, // struttura di ogni elemento della pq
    std::vector<std::pair<int, T>>, //contenitore degli elementi. Di default è già vector, ma ne abbiamo bisogno perchè dobbiamo specificare
    // il terzo parametro. Se non mettessimo questo, il terzo sarebbe letto come secondo e darebbe errore.
    // viene usato un vector per immagazzinare le info perchè è velocissimo per saltare da un elemento all'altro 
    std::greater<std::pair<int,T>> //comparatore (rispetto alla distanza int, infatti se non specificato guarda sempre il primo elemento della pair, altimenti passa al secondo)
    > pq;

    pq.push({dist[nodo_start],nodo_start}); //enqueue del nodo sorgente

    bool end_found = false; // inizializziamo un booleano che diventa true quando ho trovato il nodo_end

    while (!pq.empty() && !end_found) {
        auto [p, u] = pq.top(); // dequeue della coppia con priorità (distanza minima)
        pq.pop();

        if (p > dist[u]) continue; // controllo di validità della priorità (se la condizione non è verificata, allora significa che la distanza di u non è
        // minore della priorità corrente, cioè u è un nodo che è già stato visitato ed inserito nel cammino minimo che non vogliamo più considerare)

        std::set<T> vicini_u = G.neighbours(u);
        for (const auto& w : vicini_u) {

            int peso_uw = 1; // il grafo ha peso unitario

            if (dist[w]>dist[u]+peso_uw) { // se w ha ancora dist = inf 
                dist[w]=dist[u]+peso_uw; // aggiorniamo la distanza
                pred[w]=u; // aggiorniamo il predecessore
                pq.push({dist[w],w}); // inseriamo la nuova coppia (inseriamo quindi tutte le coppie dei vicini nella pq, da cui estrarremo quello con distanza minore alla prox iterazione di ciclo)
                
                if (w == nodo_end) { // se abbiamo trovato il nodo_end, interrompiamo il ciclo for ed usciamo
                    end_found = true;
                    break;
                }
            }
        }
    }

// siamo usciti dal ciclo e sfruttiamo la mappa dei predecessori (stoppata al nodo_end) per ricostruire il cammino minimo tra nodo_start e nodo_end
// procedendo all'indietro ci verrà il percorso inverso, quindi a sua volta lo ribaltiamo
    if (end_found) {
        T nodo_corrente = nodo_end;
        while (nodo_corrente != nodo_start) {
            cammino_minimo.push_back(nodo_corrente); // inseriamo il nodo del vettore
            nodo_corrente = pred[nodo_corrente]; // riaggiorniamo il nodo con il predecessore per andare all'indietro
        }
        cammino_minimo.push_back(nodo_start); // inseriamo alla fine il nodo start
        std::reverse(cammino_minimo.begin(), cammino_minimo.end()); // invertiamo il cammino costruito.
        // reverse mette un iteratore all'inizio e uno alla fine. A ogni iterazione scambia i due elementi corrispondenti
        // e manda gli iteratori verso il centro, finchè non si incontrano
        return cammino_minimo;
    }

    else { // se non troviamo il nodo_end
    std::cout << "Cammino non trovato" << std::endl;
    return std::vector<T>(); // returniamo vettore vuoto
    }

}



// Creiamo una funzione che verifichi se il grafo sia connesso, o fornisca le componenti connesse
template <typename T>
std::vector<unidirected_graph<T>> find_componenti_connesse(const unidirected_graph<T>& G) {

    std::vector<unidirected_graph<T>> componenti_connesse; // Inizializziamo il vettore di output

    // Inizializziamo a tutti false la mappa che associa true se il nodo è visitato
    std::map<T,bool> reached;
    for (const auto& nodo : G.all_nodes()) { 
            reached[nodo] = false;
            }

    // Usiamo la struttura di bfs per costruire un grafo per ogni componente connessa
    for (const auto& nodo : G.all_nodes()) { // Per ogni nodo

        if (!reached[nodo]) { 

            unidirected_graph<T> componente;
            std::queue<T> q;

            q.push(nodo);
            reached[nodo] = true;

            while (!q.empty()) {

                T u = q.front(); // dequeue
                q.pop();

                for (const auto& w : G.neighbours(u)) {

                    componente.add_edge(u,w); // non ci sono problemi se, iterando, si aggiunge lo stesso arco più volte

                    if (!reached[w]) {
                        reached[w] = true;
                        q.push(w);
                    }
                }
            }
            componenti_connesse.push_back(componente);
        }
    }
    return componenti_connesse;
}