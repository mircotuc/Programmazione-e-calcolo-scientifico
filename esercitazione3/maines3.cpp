#include "rational.hpp"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, const char *argv[]) {
if (argc != 5) { // I want to rationals in order to test the operators
        cout << "Errore. Uso corretto del programma:" << endl;
        cout << argv[0] << " <num1> <den1> <num2> <den2>" << endl;
        cout << "Esempio: " << argv[0] << " 1 2 3 4 (crea 1/2 e 3/4)" << endl;
        return 1; // 1 stands for error
    }

int num1 = stoi(argv[1]); // I use stoi to convert the string arguments to integers
int den1 = stoi(argv[2]);
int num2 = stoi(argv[3]);
int den2 = stoi(argv[4]);

rational r1(num1, den1); // I create the two rationals using the user-defined constructor
rational r2(num2, den2);

cout << "Hai inserito: " << endl;
cout << "Razionale 1: " << r1 << endl;
cout << "Razionale 2: " << r2 << endl;

cout << "RISULTATI DELLE OPERAZIONI:" << endl;

rational somma = r1 + r2; 
cout << "Somma: " << somma << endl;

rational differenza = r1 - r2;
cout << "Differenza: " << differenza << endl;

rational prodotto = r1 * r2;
cout << "Prodotto: " << prodotto << endl;

rational quoziente = r1 / r2;
cout << "Quoziente: " << quoziente << endl;

return 0;
}