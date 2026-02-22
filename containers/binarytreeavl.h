#ifndef __BINARYTREEAVL_H__
#define __BINARYTREEAVL_H__

#include "binarytree.h"



template <typename T_Traits>
class CBinaryTreeAVL : public CBinaryTree<T_Traits> {
    using T_Nodo = typename CBinaryTree<T_Traits>::T_Nodo;
    using T_Dato = typename T_Traits::T_Dato;

public:
    CBinaryTreeAVL() : CBinaryTree<T_Traits>() {}

    // Sobrescritura de Insert con balanceo
    void Insert(const T_Dato& valor, ref_type ref = -1) override {
        lock_guard<mutex> lock(this->m_mtx);
        insAVL(this->m_raiz, nullptr, valor, ref);
    }

    // Punto: Remove en AVL
    void Remove(const T_Dato& valor) {
        lock_guard<mutex> lock(this->m_mtx);
        remAVL(this->m_raiz, valor);
    }

protected:
    // Utilidades de balanceo personalizadas
    int getAlt(T_Nodo* n) { return n ? 1 + max(getAlt(n->hijos[0]), getAlt(n->hijos[1])) : 0; }
    int getFB(T_Nodo* n) { return n ? getAlt(n->hijos[0]) - getAlt(n->hijos[1]) : 0; }

    void rotDer(T_Nodo*& p) {
        T_Nodo* a = p->hijos[0];
        p->hijos[0] = a->hijos[1];
        if (a->hijos[1]) a->hijos[1]->padre = p;
        a->padre = p->padre;
        a->hijos[1] = p;
        p->padre = a;
        p = a;
    }

    void rotIzq(T_Nodo*& p) {
        T_Nodo* a = p->hijos[1];
        p->hijos[1] = a->hijos[0];
        if (a->hijos[0]) a->hijos[0]->padre = p;
        a->padre = p->padre;
        a->hijos[0] = p;
        p->padre = a;
        p = a;
    }

    void balancear(T_Nodo*& n) {
        if (!n) return;
        int fb = getFB(n);
        if (fb > 1) {
            if (getFB(n->hijos[0]) < 0) rotIzq(n->hijos[0]);
            rotDer(n);
        } else if (fb < -1) {
            if (getFB(n->hijos[1]) > 0) rotDer(n->hijos[1]);
            rotIzq(n);
        }
    }

    void insAVL(T_Nodo*& n, T_Nodo* p, const T_Dato& v, ref_type r) {
        if (!n) { n = new T_Nodo(v, r, p); return; }
        bool d = T_Traits::comparar(n->valor, v);
        insAVL(n->hijos[d], n, v, r);
        balancear(n);
    }

    void remAVL(T_Nodo*& n, const T_Dato& v) {
        if (!n) return;
        if (v < n->valor) remAVL(n->hijos[0], v);
        else if (v > n->valor) remAVL(n->hijos[1], v);
        else {
            if (n->hijos[0] && n->hijos[1]) {
                T_Nodo* s = n->hijos[1];
                while (s->hijos[0]) s = s->hijos[0];
                n->valor = s->valor;
                remAVL(n->hijos[1], s->valor);
            } else {
                T_Nodo* t = n;
                n = (n->hijos[0]) ? n->hijos[0] : n->hijos[1];
                if (n) n->padre = t->padre;
                delete t;
            }
        }
        if (n) balancear(n);
    }
};
#endif