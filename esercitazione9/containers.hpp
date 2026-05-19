#pragma once
#include <stack> // STL container for lifo
#include <queue> // STL container for fifo

template<typename T>
class fifo {
    private:
        std::queue<T> coda;
    public:
        fifo()= default;

        void put(const T& node) {
            coda.push(node); //push inserts the node at the end of the queue
        }

        T get() {
            T temp = coda.front(); // for retrieving
            coda.pop(); // for removing
            return temp;
        }

        bool empty() const { // i use const because I'm not modifying anything
            return coda.empty(); // empty verifies the queue is empty
        }

};

template<typename T>
class lifo {
    private:
        std::stack<T> pila;
    public:
        lifo()= default;
    
        void put(const T& node) {
            pila.push(node);
        }

        T get() {
            T temp= pila.top(); //like front
            pila.pop();
            return temp;
        }
        
        bool empty() const {
            return pila.empty();
        }
};

// I ridefine the priority queue in order for the methods to be the same as lifo and fifo
template<typename T, typename W>
struct PQelement {
    T node;
    W dist;
    // I want the element with the lower weight to be at the bottom (first in queue), so I need to redifine my operator<
    // otherwise std::priority_queue automatically put the higher weight first
    bool operator<(const PQelement& other) const {
        return dist>other.dist;
    }
};

template<typename T, typename W>
class PQ {
    private:
        std::priority_queue<PQelement<T,W>> pq;
    public:
        PQ()= default;
    
        void put(const T& node, const W& dist) {
            pq.push({node, dist});
        }

        PQelement<T,W> get() {
            auto temp= pq.top(); //like front
            pq.pop();
            return temp;
        }
        
        bool empty() const {
            return pq.empty();
        }
};

