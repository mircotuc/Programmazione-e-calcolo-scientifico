#pragma once
#include <iostream>
#include <eigen3/Eigen/Dense>

Eigen::VectorXd gcd(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, Eigen::VectorXd x0 = Eigen::VectorXd(), double res_tol= 1e-15, int iter_max=100000) {
    // tol e itermax sono messi come parametri di default
    
    // A deve essere una matrice quadrata
    if (A.rows() != A.cols()) {
        std::cerr << "ERRORE: la matrice A deve essere quadrata." << std::endl;
        return Eigen::VectorXd();
    }
    // A deve essere una matrice simmetrica (A=A^T)
    if (!A.isApprox(A.transpose())) { // isApprox permette di evitare che il codice si blocchi per minimi errori (dovuti ad esempio a piccole approssimazioni)
        std::cerr << "ERRORE: la matrice A deve essere simmetrica." << std::endl;
        return Eigen::VectorXd();
    }
    
    // A e b devono avere dimensioni compatibili
    if (b.size() != A.rows()) {
        std::cerr << "ERRORE: le dimensioni di A e b devono essere compatibili." << std::endl;
        return Eigen::VectorXd();
    }
    
    // A, b e x0 devono avere dimensioni compatibili
    if (x0.size() != 0 && x0.size() != b.size()) {
        std::cerr << "ERRORE: le dimensioni di A, x0 e b devono essere compatibili" << std::endl;
        return Eigen::VectorXd();
    }
    
    // Se non viene specificato x0 come input si inizializza il vettore unitario
    if (x0.size()== 0) {
        x0= Eigen::VectorXd::Ones(b.size());
    }

    Eigen::VectorXd x = x0;
    Eigen::VectorXd r = b - A * x; // residuo r0
    double norm_r0= r.norm(); // norma di r0
    Eigen::VectorXd p = r; // variabile di "supporto" p0
    
    int iter =0;
    // La convergenza in n passi (normalmente garantita dal metodo) in questo caso non è garantita a causa dell'aritmetica floating point,
    // quindi non posso fare un for su n iterazioni, ma impongo una condizione di arresto sulla norma del residuo e sul numero max di iterazioni
    while ( iter < iter_max && r.norm() > res_tol * norm_r0) {

        double pAp = p.dot(A * p);

        // A deve essere definita positiva (p^T * A * p > 0)
        if (pAp <= 1e-14 * p.squaredNorm()) { // (AI) Inseriamo una soglia di tolleranza per gestire l'instabilità numerica
            std::cerr << "ERRORE: La matrice A deve essere definita positiva." << std::endl;
            return Eigen::VectorXd();
        }

        double alpha = p.dot(r) / pAp;
        x = x + alpha * p;
        r = b - A * x;
        double beta= p.dot(A * r) / pAp;
        p = r - beta * p;
        iter++;
    }

    if (iter==iter_max) {
        std::cerr<< "\nIl metodo ha raggiunto il numero di iterazioni massime senza trovare la soluzione"<<std::endl;
    }
    std::cout << "\nIl metodo converge in " << iter << " iterazioni"<<std::endl;
    return x;
}
