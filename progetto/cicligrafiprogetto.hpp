#pragma once
#include "classegrafiprogetto.hpp"
#include "visitegrafiprogetto.hpp"
#include <set>
#include <map>
#include <iostream>
#include <vector>

template<typename T>
// non le facciamo restituire nulla, le facciamo direttamente modificare B_i e il vettore di vettori aggiungendo i cicli minimi
void find_minimal_cycle(const std::vector<bool>& S_i, const unidirected_graph<T>& G, std::vector<bool>& B_i, std::vector<T>& ciclo_min) { 

    std::set<unidirected_edge<T>> archi_grafo= G.all_edges();
    std::set<T> nodi_grafo = G.all_nodes();

    // vogliamo 'sdoppiare' il grafo. Per farlo in modo robusto rispetto alla numerazione dei nodi, aggiungiamo a ogni 
    // nodo del grafo originale il numero del nodo più grande +1.
    // Salviamo tramite rbegin (reverse begin) l'indirizzo di memoria dell'ultimo nodo del set,
    // ovvero il più grande (perchè il set è già ordinato dal più piccolo al più grande) e
    // poi usiamo il puntatore * per accedere al numero del nodo
    T N = *nodi_grafo.rbegin() + 1; // N è il nodo max

    unidirected_graph<T> G1; // inizializziamo il grafo ausiliario vuoto
    // costruiamo il grafo ausiliario
    for (const unidirected_edge<T>& arco : archi_grafo) { 
        int indice_arco = G.edge_number(arco); 
        T u = arco.from();
        T v = arco.to();

        if (S_i[indice_arco] == true) { // se l'arco (u,v) è attivo, costruiamo gli archi che collegano i due nodi "cambiando" piano
            G1.add_edge(u, v + N); 
            G1.add_edge(u + N, v);
        }
        else { // se l'arco (u,v) è inattivo costruiamo gli archi che collegano i nodi restando sullo stesso piano
            G1.add_edge(u,v);
            G1.add_edge(u + N, v + N);
        }
    }

    // Cerchiamo per tutti i vertici il cammino minimo tra i nodi doppioni e tra questi conserviamo quello con il numero minimo di elementi
    bool flag = true; // serve per la prima iterazione
    std::vector<T> miglior_cammino;
    size_t lunghezza_minima=0;

    for (const T& nodo : nodi_grafo) { // per ogni nodo

        std::vector<T> cammino = dijkstra_modificato(G1,nodo,nodo + N); // Troviamo il cammino minimo con il dijkstra tra i doppioni
        //abbiamo modificato il dijkstra per fargli trovare direttamente il cammino
        //minimo tra due nodi e non fargli calcolare a ogni iterazione l'albero dei cammini minimi

        if (cammino.empty()) { // se non esiste il cammino passo al prossimo nodo
            continue;
        }

        size_t lunghezza_cammino = cammino.size();

        if (flag==true) { //alla prima iterazione entra qua dentro e inizializza i valori
            lunghezza_minima = lunghezza_cammino;
            miglior_cammino = cammino;
            flag = false;
        }
        if (lunghezza_cammino < lunghezza_minima) { //dalla seconda iterazione in poi entra direttamente qua dentro grazie al flag=false
            lunghezza_minima = lunghezza_cammino;
            miglior_cammino= cammino; // aggiorniamo il cammino con numero minimo di elementi
        }
    }


    // dato il vettore contenente i nodi del ciclo minimo
    // lo trasformiamo nel formato adatto alla matrice B (1 se l'arco i appartiene al ciclo j, 0 altrimenti)
    // e intanto creiamo il vettore di output
    for (size_t j = 0; j < miglior_cammino.size()-1; ++j) {
        
        // Siccome avremo nodi di entrambi i piani nel cammino minimo, li devo riportare in un unico piano
        // e per fare questo sottraggo N trasformando il nodo "doppione" nel nodo "iniziale"

        T u = miglior_cammino[j]; // per ogni nodo del cammino
        if (u >= N) { // se il nodo è un "doppione"
            u = u - N;
        }

        T v = miglior_cammino[j+1]; 
        if (v >= N) { // se il nodo è un "doppione"
            v = v - N;
        }

        ciclo_min.push_back(u); //aggiungiamo il nodo u al vettore di output (il nodo v sarà aggiunto alla prossima iterazione)

        // non è possibile mai incontrare nel cammino minimo lo stesso nodo più volte, tranne quello di inizio, perchè il ciclo è minimo.
        // Perciò non visiterà mai un nodo 'piano di sotto' e il suo "doppione" del 'piano di sopra' (solo quello di partenza che si congiunge con il doppione alla fine)
        // Infatti non ci sono archi verticali tra nodi corrispondenti dei due piani


        unidirected_edge<T> arco_G(u,v);
        int indice_arco_G = G.edge_number(arco_G); // serve l'indice per capire dove operare su B
        B_i[indice_arco_G] = !B_i[indice_arco_G]; // aggiorniamo B_ij mettendo true (1) cambiando il bit
    }

    // Ora aggiungiamo l'ultimo nodo rimasto (quello di partenza) ripetendolo per chiudere il ciclo e aggiungerlo al vettore in output
    T ultimo_nodo = miglior_cammino.back(); // prendiamo l'ultimo nodo
    if (ultimo_nodo >= N) {
        ultimo_nodo = ultimo_nodo - N;
    }
    ciclo_min.push_back(ultimo_nodo); // lo inseriamo nel ciclo
}



// diamo in input la radice, scelta casualmente nel main. mi serve per la dfs
// il numero di cicli minimi è m - n + 1
template <typename T>
std::vector<std::vector<T>> depina(const unidirected_graph<T>& grafo, const T& radice)  { // mi restituisce un vettore di vettori contenente i cicli

    unidirected_graph<T> albero = recursive_dfs(grafo,radice); // ricaviamo l'albero di visita con la dfs (ha n-1 archi)

    unidirected_graph<T> coalbero = grafo - albero; // il coalbero ha gli archi assenti nell'albero dei percorsi della dfs. sono quelli che completano i cicli
    std::set<unidirected_edge<T>> archi_coalbero = coalbero.all_edges();

    size_t k = archi_coalbero.size(); // numero degli archi del coalbero, corrispondente al numero dei cicli minimi m-(n-1)
    size_t m = grafo.all_edges().size(); // numero degli archi del grafo di partenza


    // creiamo k vettori S_i (1 nella posizione pari all'indice dell'arco nel grafo di partenza, 0 fuori) di dimensione m
    // e li mettiamo a loro volta dentro un vettore S di dimensione k
    std::vector<std::vector<bool>> S(k,std::vector<bool>(m,false)); // li inizializziamo a tutti false (0), poi inseriamo i true (1) dopo
  
    size_t i=0;
    for (const unidirected_edge<T>& arco : archi_coalbero) { // for i=1,..,k cioè per tutti gli archi nel coalbero
        int numero_arco= grafo.edge_number(arco); // salviamo l'indice dell'arco del coalbero nel grafo di partenza
        S[i][numero_arco]= true; // poniamo true (1) nella posizione dell'indice
        i++;
    }


    // creiamo k vettori B_i (1 se l'arco i è nel ciclo j, 0 altrimenti) di dimensione m
    // e li mettiamo a loro volta dentro un vettore B di dimensione k
    std::vector<std::vector<bool>> B(k,std::vector<bool>(m, false)); // li inizializziamo a tutti false (0), poi inseriamo i true (1) dopo

    // inizializziamo in contemporanea il vettore di vettori che costituirà poi l'output
    std::vector<std::vector<T>> cicli(k); // il numero dei nodi per ogni ciclo varia, quindi non inizializziamo la dimensione

    for (size_t r=0; r<k; ++r) { // for r=1,..,k cioè per tutti gli archi nel coalbero
        find_minimal_cycle(S[r], grafo, B[r], cicli[r]); // Aggiorniamo B[r] e aggiungiamo il ciclo minimo anche nel vettore di vettori

        // ora verifichiamo che per tutti gli archi non ancora visitati (j=r+1,..,k) che non valga la condizione sul prodotto scalare = 1
        // altrimenti sarebbe possibile trovare il ciclo minimo i-esimo di nuovo
        for (size_t j=i+1; j<k; ++j) { 

            // Definiamo il prodotto scalare come la sommatoria dei B_r * S_r
            bool prodotto_scalare = false; // false indica =0 e true indica =1. Inizializziamo il prodotto scalare =0
            for (size_t l=0; l<m; ++l) { //per ogni elemento dei vettori B_r e S_r 
                if (B[r][l] && S[j][l]) { // se sono entrambi true (1)
                    prodotto_scalare = !prodotto_scalare; // invertiamo il bit
                }
            }
            
            // Se vale la condizione prodotto scalare = true (1),
            // allora si riortogonalizza la base delle S_i in modo tale che quel ciclo minimo non sia più trovabile
            if (prodotto_scalare==true) { 
                for(size_t l=0; l<m; ++l) { //per ogni elemento dei vettori B_r e S_r 
                    S[j][l]= (S[j][l] != S[r][l]); // usiamo lo xor chiedendoci se i valori sono diversi (avremo true (1) se diversi, false (0) se uguali)
                }
            }
        }
    }
    return cicli; // B avrà il primo ciclo sulla prima riga (B[0]) e così via
}



template<typename T>
bool findpath_recursive(const unidirected_graph<T>& albero_dfs, const T& u, const T& v, std::map<T,bool>& reached, std::vector<T>& path) {
    reached[u] = true;
    path.push_back(u);

    if (u == v) { // se arriva a v restituisce true
        return true;
    }

    for (const auto& n : albero_dfs.neighbours(u)) {
        if (!reached[n]) {
            if (findpath_recursive(albero_dfs,n,v,reached,path)) { // esplora i vicini finchè non arriva a v
                return true;
            }
        }
    }

    path.pop_back(); // se non trova il path toglie u e restituisce falso
    return false;
}

template<typename T>
std::vector<T> findpath(const unidirected_graph<T>& albero_dfs, const T& u, const T& v) {
    std::vector<T> path;
    std::map<T,bool> reached;

    if (findpath_recursive(albero_dfs, u, v, reached, path)) {
        return path;
    }

    return {}; // se non esiste un cammino restituiamo un vettore vuoto
}


template<typename T>
std::vector<std::vector<T>> metodo_cicli_dfs(const unidirected_graph<T>& G, const T& nodo_start) { // Richiediamo un nodo start qualsiasi su cui calcolare l'albero dfs
    unidirected_graph<T> tree = recursive_dfs(G, nodo_start); // Creiamo l'albero di visita
    unidirected_graph<T> C = G-tree; // Determiniamo il coalbero

    std::vector<std::vector<T>> cicli; //inizializziamo un vettore fatto dai vettori dei nodi appartenenti al ciclo

    // Ciclando sugli archi del coalbero, i percorsi tra u e v necessariamente esistono
    // e quel percorso più l'arco del coalbero ci danno il ciclo
    std::set<unidirected_edge<T>> archi = C.all_edges();
    for (const unidirected_edge<T>& e : archi) {

        T u = e.from();
        T v = e.to();

        std::vector<T> path = findpath(tree,u,v); // Troviamo il percorso tra u e v

        // se non trova il percorso diamo errore (perchè in teoria dovrebbe sempre esistere) e restituiamo vettore vuoto
        // altrimenti aggiungiamo ai cicli
        if (!path.empty()) {
            path.push_back(u); // Aggiungiamo il nodo iniziale alla fine del vettore per chiudere il ciclo
            cicli.push_back(path); // Inseriamo il ciclo nel vettore contenente tutti i cicli
        }
        else {
            std::cerr << "ERRORE: cammino tra u e v non trovato. Grafo non connesso." << std::endl;
            return std::vector<std::vector<T>>(); 
        }
    }

    return cicli;
}



// Ora creo una funzione che, sulla base di un input fornito dall'utente, applica il metodo basato sulla dfs oppure il de pina rispettivamente
// per trovare cicli e cicli minimi
// Se l'utente inserisce 0 utilizzo il metodo basato sulla dfs per trovare i cicli
// Se l'utente inserisce 1 utilizzo il metodo di de pina per trovare i cicli minimi

// mi assicuro già nel main che l'input sia corretto

template<typename T>
std::vector<std::vector<T>> find_cycles(const unidirected_graph<T>& G, const T& nodo_start, const std::string& input_utente) {
    
    if (input_utente == "0") {
        return metodo_cicli_dfs(G,nodo_start);
    }
    
    else { //tanto nel main ci siamo assicurati che in input posso avere solo 0 o 1
        return depina(G,nodo_start);
    }
}