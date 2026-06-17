#include "visitegrafiprogetto.hpp"
#include "classegrafiprogetto.hpp"
#include "gradienteconiugatoprogetto.hpp"
#include "cicligrafiprogetto.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <iomanip> // per stampare con solo 2 cifre decimali (AI)

// Struttura che rappresenta il nostro "arco" del grafo
struct Componente {
    std::string tipo;       // 'R' per resistenza, 'G' per generatore

    std::string nome; // contiene il tipo + il numero della componente
    // è robusto rispetto al fatto che si possano chiamare i componenti anche con lettere,float,.. (ad es. Rt,R5.0)
    // intanto noi gestiremo le componenti tramite una numerazione "interna"

    int num_interno; //diamo ad ogni componente un numero crescente in modo da evitare problemi di 
    // righe e colonne vuote quando generiamo la matrice R (B^tRB sarebbe singolare in tal caso!)
    // così facendo il programma è robusto rispetto a numerazioni non sequenziali di R e V in input

    double valore;   // valore Resistenza o Tensione
    bool b; //verso del generatore e delle resistenze. 

    // CONVENZIONE SULL'INIZIALIZZAZIONE DEI BOOL PER I GENERATORI:
    //-----------------
    // per i generatori mettiamo false se percorrendo il generatore dal nodo piu piccolo al nodo piu grande,
    // il generatore darebbe contributo negativo (da + a -). Questo corrisponde ad avere il nodo più piccolo prima nell'input
    // mettiamo true se percorrendo il generatore sempre dal nodo piu piccolo al nodo piu grande,
    // il generatore darebbe contributo positivo (da - a +). Questo corrisponde ad avere il nodo più grande prima nell'input
    //------------------
    // poi andremo a confrontare con il verso di percorrenza dei cicli.
    // se comp.b che abbiamo messo noi e il verso di percorrenza sono concordi (il verso è true se è dal nodo più piccolo a quello più grande) allora 
    // daremo + sennò -

    // per le resistenze inizializzo tutto a true ma tanto non mi serve perchè in base al verso di percorrenza dei cicli andremo a modificare direttamente
    // la matrice B. Se il verso è dal nodo più piccolo a quello più grande metteremo +1 altrimenti -1
};


int main(int argc, char* argv[]) {
// argc è il numero di input dati da terminale 
// argv è un array di stringhe contenenti gli input dati da terminale

    // Apriamo il file

    // Controllo di sicurezza
    if (argc < 2) {
        std::cerr << "ERRORE: Nessun file di input specificato.\n";
        // argv[0] contiene sempre il nome dell'eseguibile
        std::cerr << "Uso corretto: " << argv[0] << " nome_file_netlist.txt\n";
        return 1; 
    }

    // Salviamo il nome del file in una stringa (il primo argomento è argv[1])
    std::string nome_file = argv[1];

    // Apriamo e inizializziamo il file usando il nome appena estratto da terminale
    std::ifstream file(nome_file);
    
    // Gestiamo se il file si apre correttamente
    if (!file.is_open()) {
        std::cerr << "ERRORE: impossibile aprire il file '" << nome_file << "'. Verifica che esista.\n";
        return 1;
    }

    std::map<unidirected_edge<int>,Componente> circuito; // Inizializziamo la mappa arco->caratteristiche
    unidirected_graph<int> g; // Inizializziamo il grafo associato al circuito
    unidirected_graph<int> g_gen; // Inizializziamo il grafo associato ai soli generatori

    int n_R=0; //numero interno (crescente) per le resistenze - parte da 0
    int n_G=0; //numero interno (crescente) per i generatori - parte da 0
    
    
    // Leggiamo il file riga per riga. aspira i dati dallo stream del file
    // carattere per carattere finché non incontra il carattere di "a capo"
    // e mette tutto nella stringa linea. A ogni riga getline restituisce true se l'operazione di lettura ha avuto successo
    // false altrimenti. Nell'ultima riga, se dopo l'a capo non ha più nulla da leggere restituisce false, interrompendo il while
    std::string linea;
    while (std::getline(file, linea)) {
        
        // Costruiamo uno stringstream dalla riga appena letta (legge tutto come se fosse un flusso di parole ... >> ... >> ... ;)
        // in questo modo lo rendiamo robusto rispetto a righe vuote e spaziature tra colonne
        std::istringstream iss(linea);  
        
        std::string nome_componente;
        double val;
        int nodo1;
        int nodo2;
        

        // Proviamo ad estrarre esattamente i 4 campi richiesti. 
        // L'operatore >> salta automaticamente gli spazi multipli tra le colonne
        // Inoltre se la riga è vuota non entra nell'if perchè non ha esattamente 4 campi e passa alla riga successiva
        if (iss >> nome_componente >> val >> nodo1 >> nodo2) {
            
            Componente comp; // Inizializziamo i valori associati all'arco di tipo Componente

            // Il primo carattere della stringa "nome_componente" è il tipo (R o V) 
            
            if (nome_componente[0]=='V'){
                comp.tipo="V";

                g_gen.add_edge(nodo1,nodo2); // Aggiungiamo l'arco nel grafo dei generatori
                
                comp.num_interno=n_G; //assegnamo il numero interno al generatore
                n_G=n_G+1; //aggiorniamo il numero interno da assegnare al prossimo  generatore


                if (nodo1>nodo2){
                    comp.b=true; // true se si va da nodo grande a piccolo (il generatore va da - a +, quindi contributo positivo)
                }
                else {
                    comp.b=false; // false se si va da nodo piccolo a grande (il generatore va da + a -, quindi contributo negativo)
                }
                // dopo verificheremo se effettivamente il ciclo si muove da nodo piccolo a grande o viceversa
            }
            else if (nome_componente[0]=='R') {
                comp.tipo="R";
                
                comp.b=true; // Li inizializziamo tutti a true tanto dopo non li utilizzeremo
                
                comp.num_interno = n_R; //assegnamo il numero interno alla resistenza 
                n_R=n_R+1; //aggiorniamo il numero interno da assegnare alla prossima resistenza
            }
            
            else {
                std::cerr << "\nERRORE: tipo di componente non riconosciuto.\n" << std::endl;
                return 1;
            }
            

            comp.nome = nome_componente;
            
            comp.valore = val;

           // Aggiungiamo l'arco al grafo, se invece l'arco è già esistente esco dal programma perchè l'input è sbagliato
           // Quando entro nell'if, add_edge viene eseguito e l'arco viene aggiunto, dopodichè si controlla il valore booleano
            if (g.add_edge(nodo1,nodo2) == false) {
                std::cerr << "\nERRORE: lo stesso arco contiene due o più componenti. Input non ammissibile. \n" << std::endl;
                return 1;
            }

            circuito[unidirected_edge<int>(nodo1,nodo2)]=comp; //aggiorniamo la mappa

        }
    }

    file.close();

    
    //testiamo che non ci siano cicli formati da soli generatori (anche non minimi) nel grafo originale
    // (se esistono allora la matrice A finale non è definita positiva e il problema non è ben posto)
    //abbiamo creato il grafo g_gen composto dai soli generatori per farlo
    std::vector<unidirected_graph<int>> comp_connesse_gen = find_componenti_connesse(g_gen); //troviamo le sue componenti connesse
    //componenti per calcolare il numero dei cicli:
    size_t n_comp_connesse_gen= comp_connesse_gen.size();
    size_t n_nodi_gen = g_gen.all_nodes().size(); //prendiamo il numero i nodi totali
    size_t n_archi_gen = g_gen.all_edges().size(); //prendiamo il numero di archi totali
    
    //calcolo del numero dei cicli:
    int numero_cicli_gen = n_archi_gen - n_nodi_gen + n_comp_connesse_gen; // ci sono dei cicli se e solo se |E|-|V|+n
    //se c'è almeno un ciclo, gestiamo l'errore uscendo dal programma
    if (numero_cicli_gen>0) { 
        std::cerr << "\nERRORE: ci sono cicli formati da soli generatori, quindi la matrice B^tRB non è definita positiva e il problema non è ben definito.\n" << std::endl; 
        return 1;
    }
    

    //Stampa di verifica per assicurarci che la lettura sia corretta
    std::cout << "\nComponenti letti con successo:\n";
    for (const auto& [edge,comp] : circuito) {
        std::cout << "Tipo e ID: " << comp.nome
                  << " | Valore: " << comp.valore 
                  << " | Nodi: " << edge.from() << " -> " << edge.to() << "\n";
    }


    // Data una componente connessa, se ci sono solo generatori abbiamo già dato errore prima (esiste un ciclo di soli generatori);
    // Se invece ci sono solo resistenze il termine noto nel sistema lineare
    // sarà nullo e (dato che A non è singolare) le correnti finali nelle maglie saranno tutte nulle

    // Verifichiamo se il grafo è connesso o quanti componenti connesse ha
    std::vector<unidirected_graph<int>> componenti_connesse = find_componenti_connesse(g);
    int num_componenti_connesse = componenti_connesse.size();
    
    if (num_componenti_connesse == 1) {
        std::cout << "\nIl circuito fornito in input è connesso.\n" << std::endl;
    }
    else {
        std::cout << "\nIl circuito fornito in input è composto da " << num_componenti_connesse << " sottocircuiti indipendenti.\n" << std::endl;
    }


    // Richiesta all'utente di un input 0/1 per capire quale metodo utilizzare per trovare i cicli
    // Se l'utente inserisce 0 utilizzo il metodo basato sulla dfs per trovare i cicli
    // Se l'utente inserisce 1 utilizzo il metodo di de pina per trovare i cicli minimi

    std::string input_utente = ""; // inizializziamo una stringa vuota per entrare nel ciclo

    // Si cicla finchè l'utente non fornisce un input corretto
    while (input_utente != "0" && input_utente != "1") {
        std::cout << "Inserire 0 per trovare cicli tramite il metodo basato sul DFS.\n"
        << "Inserire 1 per trovare cicli minimi tramite il De Pina.\n"
        << "Inserire 0/1: ";
        std::cin >> input_utente;
    }

    // Si avvisa l'utente della scelta
    if (input_utente == "0") {
        std::cout << "\nRisolviamo il problema utilizzando il metodo basato sul DFS.\n";
    }
    else {
        std::cout << "\nRisolviamo il problema utilizzando De Pina.\n";
    }


    std::vector<std::vector<int>> cicli; // Inizializziamo il vettore dei cicli
    size_t num_cicli_attesi_totale = 0;


    // per ogni componente connessa
    for (int i=0; i<num_componenti_connesse; i++) {

        unidirected_graph<int> g_comp = componenti_connesse[i];

        // controlliamo che il sottocircuito abbia cicli, altrimenti l'input non è una configurazione corretta
        size_t num_nodi = g_comp.all_nodes().size();
        size_t num_archi = g_comp.all_edges().size();
        size_t numero_cicli_atteso = 0;
        // In un singolo grafo connesso, i cicli iniziano a formarsi solo 
        // se gli archi sono sufficienti (m >= n - 1), ovvero m + 1 >= n. Altrimenti teniamo 0. Questo ci serve perchè sennò il numero di cicli rischia di 
        // essere negativo e per un size_t non va bene
        if (num_archi + 1 >= num_nodi) {
            numero_cicli_atteso = num_archi - num_nodi + 1;
        }
        else {
            std::cerr << "\nERRORE: il sottocircuito non ha cicli. La configurazione non è ammissibile.\n" << std::endl;
            return 1;
        }

        num_cicli_attesi_totale = num_cicli_attesi_totale + numero_cicli_atteso; // per ogni componente, aggiorno il numero totale di cicli nel grafo

        // Si utilizza una qualsiasi radice e si applica il metodo scelto per trovare i cicli, l'output sarà un vettore di vettore contenente i cicli

        int radice = *g_comp.all_nodes().begin(); // Si prende il primo nodo come radice in modo da inizializzare i metodi per trovare i cicli (non importa quale sia la radice)
        // * è un puntatore poichè il metodo begin() ti fornisce l'indirizzo di memoria del primo nodo del set all_nodes

        std::vector<std::vector<int>> cicli_comp = find_cycles(g_comp,radice,input_utente); // la funzione è ben definita perchè abbiamo controllato che ci siano cicli
        for (size_t u=0; u<cicli_comp.size(); u++) {
            cicli.push_back(cicli_comp[u]);
        }

    }

    size_t numero_cicli = cicli.size(); // Troviamo il numero di cicli per inizializzare le matrici

    //controlliamo che il numero di cicli corrisponda a quello teorico (e-v+1)
    if (numero_cicli != num_cicli_attesi_totale) {
        std::cerr << "\nERRORE: il numero di cicli trovati nel grafo non corrisponde a quello teorico.\n"<< std::endl;
        return 1;
    }
    std::cout << "\nCicli trovati:\n";
    for (size_t i = 0; i < numero_cicli; ++i) {
        std::cout << "Ciclo " << i << ": [ ";
        
        // Cicliamo sui nodi del ciclo i-esimo
        for (size_t j = 0; j < cicli[i].size(); ++j) {
            std::cout << cicli[i][j];
            
            // Stampiamo la virgola se non siamo all'ultimo nodo
            if (j < cicli[i].size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << " ]\n";
    }

    //abbiamo eliminato la variabile numero_resistenze e l'abbiamo rimpiazzato con n_R, 
    //che fa lo stesso lavoro. oss: n_R parte da 0, quindi se ci sono 10 resistenze (R deve essere 
    //10x10) n_R arriva a 9
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(n_R+1, numero_cicli);
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(n_R+1, n_R+1);
    Eigen::VectorXd v = Eigen::VectorXd::Zero(numero_cicli); //ogni elemento è fatto dalla somma dei generatori presenti nel ciclo con i dovuti segni
    
    for (size_t ciclo=0; ciclo < numero_cicli; ciclo++) { //per ogni ciclo

        for (size_t nodo=0; nodo < cicli[ciclo].size()-1; nodo++ ) { //per ogni elemento del ciclo i-esimo (sono di lunghezza diversa)
            // Utilizziamo size_t in modo da poter confrontare i contatori del ciclo con cicli[ciclo].size()

            // Inizializziamo nodo corrente e nodo successivo in modo da non perdere questa informazione (necessaria per assegnare il segno dei generatori
            // delle resistenze) quando poi si crea l'arco (la classe unidirected_edge mette sempre prima il nodo più piccolo)
            int nodo_corr = cicli[ciclo][nodo];
            int nodo_succ = cicli[ciclo][nodo+1];

            // Inizializziamo l'arco per cercarlo nella mappa con il metodo at()
            unidirected_edge<int> arco(nodo_corr,nodo_succ);
            const Componente& comp = circuito.at(arco);

            // Salviamo se il verso è giusto o meno con un bool
            bool verso_giusto= nodo_corr <= nodo_succ;
            
            if (comp.tipo == "R") {
                int num_resistenza = comp.num_interno;
                R(num_resistenza,num_resistenza) = comp.valore; // qui usiamo il numero interno
                //in modo che se gli id assegnati nell'input sono sparsi abbiamo comunque un'indicizzazione
                //interna "densa". num_interno parte da 0, come gli indici della matrice

                if (verso_giusto==true){
                    B(num_resistenza,ciclo)=1;
                }
                else {
                    B(num_resistenza,ciclo)=-1;
                }
            }

            else if (comp.tipo== "V") {

                /*if (verso_giusto==true){
                    if (comp.b==true){
                        v[ciclo]=v[ciclo]+comp.valore;
                    }
                    else{
                        v[ciclo]=v[ciclo]-comp.valore;
                    }
                }
                else{
                    if (comp.b==true){
                        v[ciclo]=v[ciclo]-comp.valore;
                    }
                    else{
                        v[ciclo]=v[ciclo]+comp.valore;
                    }
                }*/

               if (comp.b==verso_giusto){//comp.b e verso_giusto sono concordi
                    v(ciclo)=v(ciclo)+comp.valore;
               }
               else{ //comp.b e verso_giusto sono discordi
                v(ciclo)=v(ciclo)-comp.valore;
               }
            }   
        }
    }

    // ora che abbiamo tutte le matrici costruiamo e risolviamo il sistema lineare con il metodo del gradiente coniugato
    Eigen::MatrixXd A = B.transpose()*R*B; // dove B.transpose è la trasposta
    Eigen::VectorXd I_soluzione = gcd(A, v); // tol e itermax sono già di default, x0 è un vettore vuoto
    // la soluzione sarà un vettore con le correnti di ciascuna maglia (quindi un vettore di dimensione numero_cicli)

    // Se c'è qualche problema nell'applicazione del metodo del gradiente coniugato, esso returna un vettore vuoto
    if (I_soluzione.size()==0) { // in eigen non esiste .empty(), tanto se è vuoto ha dimensione zero
        std::cerr << "Errore: le matrici B, R e il vettore v non soddisfano i requisiti per poter applicare il metodo del gradiente coniugato.\n" << std::endl;
        return 1;
    }
    // se non ci sono problemi, vuol dire che il sistema lineare, le matrici e il vettore
    // sono ben definiti (tutti i i controlli sono stati fatti nella funzione gcd)

    // ora dobbiamo calcolare le tensioni sui resistori
    // il segno delle correnti è dato dall'informazione contenuta in B (il verso di percorrenza dei cicli)
    Eigen::VectorXd I_resistenza = B*I_soluzione;

    // abbiamo automaticamente selezionato le correnti da moltiplicare alla resistenza i-esima per trovare V
    Eigen::VectorXd V_resistenza = R*I_resistenza; // legge di ohm

    // stampiamo i risultati
    std::cout << std::fixed << std::setprecision(2);
    for (const auto& [arco, comp] : circuito) {
        // L'ordine di stampa è determinato dal ciclo sugli elementi della mappa (non è garantito l'ordine sequenziale)
        
        if (comp.tipo == "R") {
            int numero_resistenza = comp.num_interno; //ci serve ancora il numero
            //interno "sequenziale"
        
            // Estraiamo i valori finali dai vettori appena calcolati
            double corrente = I_resistenza(numero_resistenza);
            double tensione = V_resistenza(numero_resistenza);

            std::cout << comp.nome << ":"
                      << " V = " << tensione << " volts,"
                      << " I = " << corrente << " amps.\n";
        } // le resistenze potrebbero non essere stampate in ordine progressivo. Vengono stampate secondo
        //l'ordine con cui gli archi corrispondenti vengono salvati nella mappa.
        // std::map è un black-red tree e salva gli archi usando l'ordine lessico-grafico da noi definito
        // quando poi vado a visitare il red-black tree preleverà gli elementi secondo quest'ordine.
        // quindi le resistenze vengono stampate a schermo secondo l'ordine lessicografico degli archi corrispondenti
    }
    return 0;
}
