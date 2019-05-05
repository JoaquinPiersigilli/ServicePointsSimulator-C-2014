#ifndef INTERFACELISTA_H_INCLUDED
#define INTERFACELISTA_H_INCLUDED

struct Nodo;
typedef struct Nodo tNodo;

struct Lista {
    tNodo* ppio; //puntero al primer nodo de la lista
    tNodo* fin; //puntero al último nodo de la lista
    int cant; // cantidad de elementos de la lista
    int (*comparador)(void*,void*);
};
typedef struct Lista tListaOrdenada;

tListaOrdenada crearLista(int (*f)(void*,void*));

void insertar(tListaOrdenada* L, void* x);

tNodo* siguiente(tNodo* pos);

tNodo* primera(tListaOrdenada L);

tNodo* ultima(tListaOrdenada L);

void* elemento(tNodo* pos);

int listaVacia(tListaOrdenada L);

#endif // INTERFACELISTA_H_INCLUDED
