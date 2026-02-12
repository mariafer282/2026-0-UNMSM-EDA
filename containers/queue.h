#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <iostream>
#include <mutex> 
#include "../general/types.h"
#include "../util.h"

template <typename T>
class CQueue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* front; // Puntero al inicio (para Pop)
    Node* rear;  // Puntero al final (para Push)
    size_t size;
    mutable std::mutex mtx; // Para el punto de Concurrencia de tu tabla

public:
    // 1. Constructor básico
    CQueue() : front(nullptr), rear(nullptr), size(0) {}

    // 2. Constructor Copia (Punto 1 de tu tabla)
    CQueue(const CQueue& other);

    // 3. Move Constructor (Punto 2 de tu tabla)
    CQueue(CQueue&& other) noexcept;

    // 4. Destructor seguro (Punto 3 de tu tabla)
    ~CQueue();

    // 5. Push (Punto 4: Insertar al final)
    void push(T value);

    // 6. Pop (Punto 5: Quitar del frente)
    void pop();

    // 7. Operadores << y >> (Puntos 6 y 7)
    // Estos se definen usualmente fuera o como friends
};

#endif // __QUEUE_H__