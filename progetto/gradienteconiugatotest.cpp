#include <iostream>
#include <cstdlib>
#include <vector>
#include "gradienteconiugatoprogetto.hpp"
#include <eigen3/Eigen/Dense>

// la funzione genera una matrice nxn simmetrica definita positiva a coeff reali per poter applicarci il metodo del gradiente coniugato
Eigen::MatrixXd genero_simmdefpos(const int& n) {
    Eigen::MatrixXd A = Eigen::MatrixXd::Random(n, n);
    Eigen::MatrixXd B = A.transpose() * A; // (A^t)A è simmetrica e definita positiva per definizione
    B += n * Eigen::MatrixXd::Identity(n, n); // (AI) sommo l'identità per far si che la matrice B diventi ben condizionata 
    return B;
}

int main() {

    int n = 100;
    int num_test = 20;
    double tol = 1e-12; // Impostiamo una tollerenza abbastanza bassa

    Eigen::VectorXd exactSolution = Eigen::VectorXd::Ones(n); // impostiamo la sol esatta a quella unitaria per ricavare il termine noto b a partire da questa
    double norm_exact = exactSolution.norm();

    const Eigen::VectorXd x0 = Eigen::VectorXd::Zero(n); // impostiamo il vettore iniziale a quello nullo

    // testiamo il metodo del gradiente coniugato su 20 matrici 100x100
    for (int i=0; i<num_test; i=i+1) {

        Eigen::MatrixXd A = genero_simmdefpos(n);
        Eigen::VectorXd b = A * exactSolution;
 
        Eigen::VectorXd sol = gcd(A, b, x0);

        double err_rel = (sol - exactSolution).norm() / norm_exact; // calcoliamo l'errore relativo rispetto alla soluzione esatta
        
        if (err_rel > tol) {
            std::cerr << "Test fallito. \n";
            return EXIT_FAILURE;
        }
    }
    std::cerr << "\nTest riuscito. \n";
    return EXIT_SUCCESS;
}