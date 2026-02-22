#ifndef __BINARYTREE_H__
#define __BINARYTREE_H__

#include <iostream>
#include <mutex>
#include <utility>
#include <stack>
#include "../general/types.h"
#include "GeneralIterator.h"

using namespace std;

// Estructuras de comparación personalizadas (Traits)
template <typename T>
struct OrdenAsc {
    using T_Dato = T;
    static bool comparar(const T& a, const T& b) { return a < b; }
};

// Nodo con puntero al padre y arreglo de hijos
template <typename T_Traits>
struct NodoBT {
    using Dato = typename T_Traits::T_Dato;
    Dato valor;
    ref_type ref;
    NodoBT *hijos[2]; // 0: Izq, 1: Der
    NodoBT *padre;

    NodoBT(Dato v, ref_type r = -1, NodoBT* p = nullptr) 
        : valor(v), ref(r), padre(p) {
        hijos[0] = hijos[1] = nullptr;
    }
};

template <typename T_Traits>
class CBinaryTree {
public:
    using T_Nodo = NodoBT<T_Traits>;
    using T_Dato = typename T_Traits::T_Dato;
    using forward_iterator = BinaryTreeForwardIterator<CBinaryTree<T_Traits>>;

protected:
    T_Nodo* m_raiz = nullptr;
    mutable mutex m_mtx; // Punto: Concurrencia

public:
    CBinaryTree() : m_raiz(nullptr) {}

    // Constructor Copia y Move
    CBinaryTree(const CBinaryTree& otra) {
        lock_guard<mutex> lock(otra.m_mtx);
        m_raiz = copiar(otra.m_raiz, nullptr);
    }
    CBinaryTree(CBinaryTree&& otra) noexcept {
        lock_guard<mutex> lock(otra.m_mtx);
        m_raiz = std::exchange(otra.m_raiz, nullptr);
    }

    virtual ~CBinaryTree() { vaciar(); }

    // Inserción Iterativa (Punto: Insert)
    virtual void Insert(const T_Dato& dato, ref_type r = -1) {
        lock_guard<mutex> lock(m_mtx);
        if (!m_raiz) { m_raiz = new T_Nodo(dato, r); return; }
        T_Nodo *curr = m_raiz, *prev = nullptr;
        bool lado = 0;
        while (curr) {
            prev = curr;
            lado = T_Traits::comparar(curr->valor, dato);
            curr = curr->hijos[lado];
        }
        prev->hijos[lado] = new T_Nodo(dato, r, prev);
    }

    // Punto: Remove
    void Remove(const T_Dato& dato) {
        lock_guard<mutex> lock(m_mtx);
        eliminarNodo(m_raiz, dato);
    }

    // Iteradores
    forward_iterator begin() {
        T_Nodo* n = m_raiz;
        if (n) while (n->hijos[0]) n = n->hijos[0];
        return forward_iterator(this, n);
    }
    forward_iterator end() { return forward_iterator(this, nullptr); }

    // Punto: Recorridos Variádicos (In/Pre/Post)
    template <typename... Fs>
    void inorden(Fs&&... fns) { 
        lock_guard<mutex> lock(m_mtx); 
        recIn(m_raiz, std::forward<Fs>(fns)...); 
    }

    // Punto: ForEach y FirstThat
    template <typename... Fs>
    void ForEach(Fs&&... fns) { inorden(std::forward<Fs>(fns)...); }

    // Punto: Operador <<
    friend ostream& operator<<(ostream& os, CBinaryTree& t) {
        t.inorden([&os](const T_Dato& v){ os << v << " "; });
        return os;
    }

private:
    void vaciar() { lock_guard<mutex> lock(m_mtx); borrar(m_raiz); m_raiz = nullptr; }
    void borrar(T_Nodo* n) { if(n){ borrar(n->hijos[0]); borrar(n->hijos[1]); delete n; } }
    
    T_Nodo* copiar(T_Nodo* n, T_Nodo* p) {
        if (!n) return nullptr;
        T_Nodo* nuevo = new T_Nodo(n->valor, n->ref, p);
        nuevo->hijos[0] = copiar(n->hijos[0], nuevo);
        nuevo->hijos[1] = copiar(n->hijos[1], nuevo);
        return nuevo;
    }

    template <typename... Fs>
    void recIn(T_Nodo* n, Fs&&... fns) {
        if (!n) return;
        recIn(n->hijos[0], std::forward<Fs>(fns)...);
        (std::forward<Fs>(fns)(n->valor), ...);
        recIn(n->hijos[1], std::forward<Fs>(fns)...);
    }

    void eliminarNodo(T_Nodo*& n, const T_Dato& v) {
        if (!n) return;
        if (v < n->valor) eliminarNodo(n->hijos[0], v);
        else if (v > n->valor) eliminarNodo(n->hijos[1], v);
        else {
            if (n->hijos[0] && n->hijos[1]) {
                T_Nodo* s = n->hijos[1];
                while (s->hijos[0]) s = s->hijos[0];
                n->valor = s->valor;
                eliminarNodo(n->hijos[1], s->valor);
            } else {
                T_Nodo* old = n;
                n = (n->hijos[0]) ? n->hijos[0] : n->hijos[1];
                if (n) n->padre = old->padre;
                delete old;
            }
        }
    }
};
#endif