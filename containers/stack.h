#ifndef __STACK_H__
#define __STACK_H__

#include <iostream>
#include <mutex>
#include <stdexcept>
#include "../general/types.h"
#include "../util.h"

template <typename T>
class CStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
    };

    Node* _top;
    size_t _size;
    mutable std::mutex mtx; // Requerimiento: Concurrencia (mutex)

public:
    // Constructor por defecto
    CStack() : _top(nullptr), _size(0) {}

    // 1. Constructor Copia (Requerimiento)
    CStack(const CStack& other) : _top(nullptr), _size(0) {
        std::lock_guard<std::mutex> lock(other.mtx);
        if (other._top) {
            Node* current = other._top;
            Node* last = nullptr;
            while (current) {
                Node* newNode = new Node(current->data);
                if (!_top) _top = newNode;
                else last->next = newNode;
                last = newNode;
                current = current->next;
            }
            _size = other._size;
        }
    }

    // 2. Move Constructor (Requerimiento)
    CStack(CStack&& other) noexcept : _top(nullptr), _size(0) {
        std::lock_guard<std::mutex> lock(other.mtx);
        _top = other._top;
        _size = other._size;
        other._top = nullptr;
        other._size = 0;
    }

    // 3. Destructor seguro (Requerimiento)
    ~CStack() {
        while (_top) {
            pop();
        }
    }

    // 4. Push (Requerimiento)
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        _top = new Node(value, _top);
        _size++;
    }

    // 5. Pop (Requerimiento)
    void pop() {
        std::lock_guard<std::mutex> lock(mtx);
        if (!_top) return; 
        Node* temp = _top;
        _top = _top->next;
        delete temp;
        _size--;
    }

    // 6. Operator << (Requerimiento)
    friend std::ostream& operator<<(std::ostream& os, const CStack& s) {
        std::lock_guard<std::mutex> lock(s.mtx);
        Node* current = s._top;
        os << "[Top] -> ";
        while (current) {
            os << current->data << " -> ";
            current = current->next;
        }
        os << "NULL";
        return os;
    }

    // 7. Operator >> (Requerimiento)
    friend std::istream& operator>>(std::istream& is, CStack& s) {
        T value;
        if (is >> value) {
            s.push(value);
        }
        return is;
    }
};

#endif // __STACK_H__