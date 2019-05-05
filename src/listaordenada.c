#include "listaordenada.h"
#include <stdlib.h>

struct Nodo
{
    void * elem;
    struct Nodo* next;
};

tListaOrdenada crearLista(int (*f)(void*,void*))
{

    tListaOrdenada miLista;

    miLista.cant = 0;
    miLista.ppio = NULL;
    miLista.fin = NULL;
    miLista.comparador = f;
    return miLista;
}

void insertar(tListaOrdenada* L, void* x)
{
    tNodo* nuevo = (tNodo*) malloc(sizeof(tNodo));
    nuevo->elem=x;
    nuevo->next=NULL;
    int valorComparador;

    if(L->cant==0)
    {
        L->ppio=nuevo;
        L->fin=nuevo;
    }
    else if(L->cant==1)
    {
        valorComparador = L->comparador(L->ppio->elem,x);
        if(valorComparador==1)
        {
            nuevo->next=L->ppio;
            L->ppio=nuevo;
        }
        else
        {
            L->ppio->next=nuevo;
            L->fin=nuevo;
        }
    }
    else //el tamaño es mayor que 1..
    {
        tNodo* actual= L->ppio;
        tNodo* antActual;
        int encontre=0;
        valorComparador= L->comparador(actual->elem,x);
        if(valorComparador==1)
        {
            nuevo->next=actual;
            L->ppio=nuevo;
        }
        else //valorComparador =0 ó -1...
        {
            while((encontre==0)&&(actual->next!=NULL))
            {
                antActual=actual;
                actual=antActual->next;
                valorComparador= L->comparador(actual->elem,x);
                if(valorComparador==1)
                {
                    antActual->next=nuevo;
                    nuevo->next=actual;
                    encontre=1;
                }
            }
            if(encontre==0)
            {
                actual->next=nuevo;
                L->fin=nuevo;
            }
        }
    }
    L->cant=L->cant+1;
}

tNodo* siguiente(tNodo* pos)
{
    if (pos == NULL)
    {
        return NULL;
    }
    return pos->next;
}

tNodo* primera(tListaOrdenada L)
{
    return L.ppio;
}

tNodo* ultima(tListaOrdenada L)
{
    return L.fin;
}

int listaVacia(tListaOrdenada L)
{
    return !(L.cant);
}

void* elemento(tNodo* pos)
{
    if(pos->elem!=NULL)
        return pos->elem;
    else
        return NULL;
}
