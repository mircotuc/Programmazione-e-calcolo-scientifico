#pragma once
#include <iostream>
#include <Eigen/Dense>
#include <stdexcept>

inline Eigen::VectorXd gcd(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, Eigen::VectorXd x0 = Eigen::VectorXd(), double res_tol= 1e-15, unsigned int iter_max=100000) {//default parametres for tol iter e x0, unsigned holds only positive values
    
    // A must be a square matirx (n x n)
    if (A.rows() != A.cols()) {
        throw std::invalid_argument("Errore: La matrice A deve essere quadrata.");
    }
    // A must be symmetric (A=A^T), we use isApprox to avoid problems with minimal errors
    if (!A.isApprox(A.transpose())) {
        throw std::invalid_argument("Errore: La matrice A non è simmetrica.");
    }
    
    // dimensions of b and A must agree
    if (b.size() != A.rows()) {
        throw std::invalid_argument("Errore: Le dimensioni di A e b non sono compatibili.");
    }
    
    // same for x0 and b
    if (x0.size() != 0 && x0.size() != b.size()) {
        throw std::invalid_argument("Errore: La dimensione del vettore iniziale x0 non è compatibile con b.");
    }
    
    if (x0.size()== 0) { // if the user didn't sepcify x0 we initialize to vector of ones, tanto funziona comunque
        x0= Eigen::VectorXd::Ones(b.size());
    }
    Eigen::VectorXd x = x0; //column vector by default
    Eigen::VectorXd r = b - A*x0; //this is r0
    Eigen::VectorXd p = r; //this is p0

    double normr0= r.norm();
    unsigned int iter =0;
    // I iterate until the residual norm has become a certain fraction smaller than the starting norm, I add a condition on iter in order to avoid inifnite cycles
    while ( iter< iter_max && r.norm()>res_tol*normr0) {
        Eigen::VectorXd Ap= A*p;
        double pAp = p.dot(Ap);

        // Check A definite positive (p^T * A * p > 0)
        if (pAp <= 1e-14 * p.squaredNorm()) {
            throw std::invalid_argument("Errore: La matrice A non è definita positiva.");
        }

        double alpha_k= p.dot(r)/pAp;
        x=x+alpha_k*p;
        r=b-A*x; // could be r=r-alpha_k*Ap to be more efficient
        double beta_k= p.dot(A*r)/pAp;
        p=r-beta_k*p;
        iter++;
    }
    if (iter==iter_max) {
        std::cerr<< "\nIl metodo ha raggiunto il numero di iterazioni massime senza trovare la soluzione"<<std::endl;
    }
    return x;
}
