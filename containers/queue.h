#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <iostream>
#include <mutex>
#include <stdexcept>
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
    mutable std::mutex mtx; // Requisito: Concurrencia (mutex)

public:
    // 1. Constructor básico
    CQueue() : front(nullptr), rear(nullptr), size(0) {}

    // 2. Constructor Copia (Punto de la tabla)
    CQueue(const CQueue& other) : front(nullptr), rear(nullptr), size(0) {
        std::lock_guard<std::mutex> lock(other.mtx);
        Node* temp = other.front;
        while (temp) {
            this->push(temp->data);
            temp = temp->next;
        }
    }

    // 3. Move Constructor (Punto de la tabla)
    CQueue(CQueue&& other) noexcept : front(nullptr), rear(nullptr), size(0) {
        std::lock_guard<std::mutex> lock(other.mtx);
        front = other.front;
        rear = other.rear;
        size = other.size;
        
        other.front = nullptr;
        other.rear = nullptr;
        other.size = 0;
    }

    // 4. Destructor seguro (Punto de la tabla)
    ~CQueue() {
        while (front) {
            pop();
        }
    }

    // 5. Push - Insertar al final (Punto de la tabla)
    void push(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* newNode = new Node(value);
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    // 6. Pop - Eliminar del frente (Punto de la tabla)
    void pop() {
        std::lock_guard<std::mutex> lock(mtx);
        if (!front) return;
        Node* temp = front;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        size--;
    }

    // 7. Operador << para imprimir (Punto de la tabla)
    friend std::ostream& operator<<(std::ostream& os, const CQueue& q) {
        std::lock_guard<std::mutex> lock(q.mtx);
        Node* temp = q.front;
        os << "[";
        while (temp) {
            os << temp->data << (temp->next ? ", " : "");
            temp = temp->next;
        }
        os << "]";
        return os;
    }

    // Métodos extra útiles
    bool empty() const { 
        std::lock_guard<std::mutex> lock(mtx);
        return size == 0; 
    }
    
    size_t getSize() const { return size; }
};

#endif // __QUEUE_H__