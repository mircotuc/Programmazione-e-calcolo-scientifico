#include "conjugategradient.hpp"
#include<Eigen/Dense>
#include <iostream>
#include <cstdlib>

int main() {
    bool all_passed= true;
    std::cout << "--- TEST METODO DEL GRADIENTE CONIUGATO CON MATRICE CORRETTA ---" << std::endl;
    try {
        int n = 50; 

        // I use A = M^T * M + I to generate a symmetric defnite positive matrix from a random M
        Eigen::MatrixXd M = Eigen::MatrixXd::Random(n, n);
        Eigen::MatrixXd A = M.transpose() * M + Eigen::MatrixXd::Identity(n, n);

        // choosing a theorical solution x=[1, 2, 3, 4, 5] (column vector)
        Eigen::VectorXd x_ex = Eigen::VectorXd::LinSpaced(n, 1.0, 5.0);

        // calculate b (b = A * x)
        Eigen::VectorXd b = A * x_ex;
        std::cout.precision(4);


        std::cout << "Soluzione teorica esatta: \n" << x_ex.transpose() << "\n" << std::endl;


        Eigen::VectorXd x_sol = gcd(A, b); //using default parametres for x0, res_tol, iter_max

        std::cout << "Soluzione calcolata con il metodo: \n" << x_sol.transpose() << "\n" << std::endl;

        double abs_err = (x_sol - x_ex).norm();
        
        std::cout << "Norma dell'errore (||x_sol - x_ex||): " << std::scientific << abs_err << std::endl; //in notazione scientifica

        if (abs_err < 1e-10) {
            std::cout << "\nTest SUPERATO! Le soluzioni coincidono." << std::endl;
        } else {
            std::cout << "\nTest FALLITO. Le soluzioni non coincidono." << std::endl;
            all_passed= false;
        }

    } catch (const std::invalid_argument& e) {//this is useful for getting the errors we defined in the method
        std::cerr << "\n" << e.what() << std::endl;
        all_passed= false;
    }

    std::cout << "\n--- TEST CON MATRICE ASIMMETRICA ---" << std::endl;
    try {
        int n = 3;
        Eigen::MatrixXd A_asymm(n, n);
        A_asymm << 4.0, 1.0, 0.0,  //it modifies the matrix
                         5.0, 3.0, 2.0,  
                         0.0, 2.0, 5.0;
        
        Eigen::VectorXd b = Eigen::VectorXd::Ones(n);
        
        gcd(A_asymm, b); //I just call the method because I just want to sse if it finds the error

        // If nothing happens it means the method didn't find the exception
        std::cerr << "ERRORE: La funzione non ha notato l'asimmetria della matrice!" << std::endl;
        std::cerr << "Test FALLITO.\n" << std::endl;
        all_passed= false;
    } 
    catch (const std::invalid_argument& e) {
        // if we're here the method found the error
        std::cout << "Eccezione rilevata: \"" << e.what() << "\"" << std::endl;
        std::cout << "Test SUPERATO\n" << std::endl;
    }

    if (all_passed) {
        return EXIT_SUCCESS;
    }
    else {
        return EXIT_FAILURE;
    }
}