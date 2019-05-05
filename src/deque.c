#include <stdio.h>
#include <stdlib.h>
#include "deque.h"


typedef struct Nodo {
    void* elem;
    struct Nodo* sig;
    struct Nodo* ant;
} tNodo;

typedef struct Deque {
    tNodo* ppio; //puntero al primer nodo de la cola
    tNodo* fin; //puntero al último nodo de la cola
    int cant;
}tDeque;

    tDeque* crearDeque(){
        tDeque* deque = (tDeque*)malloc(sizeof(tDeque));
        deque->cant=0;
        deque->fin=NULL;
        deque->ppio=NULL;

        return deque;
    }

    void insertarEnDeque(tDeque* D, void * x){

        //Asignamos memoria para el nodo a insertar.
        tNodo* nodo = (tNodo *)malloc(sizeof(tNodo));

        nodo->sig=NULL;
        nodo->elem=x;

        if(D->cant==0){
            nodo->ant=NULL;
            D->ppio=nodo;
            D->fin=nodo;
        }
        else{
            D->fin->sig=nodo;
            nodo->ant=D->fin;
            D->fin=nodo;

        }
        D->cant++;

    }

    void* eliminarAtras(tDeque* D){

        void* toReturn;

        if(D->cant==0)
            toReturn = NULL;
        else{
            toReturn = D->ppio->elem;
            if(D->cant==1){
                free(D->fin);
                D->fin=NULL;
                D->ppio=NULL;
                D->cant--;
            }
            else{
                tNodo* aux = D->ppio->sig;
                aux->ant=NULL;
                free(D->ppio);
                D->ppio=aux;
                D->cant--;
            }
        }

        return toReturn;
    }

    void* eliminarAdelante(tDeque* D){

        void* toReturn;
        if(D->cant==0)
            toReturn = NULL;
        else{
            toReturn = D->fin->elem;
            if(D->cant==1){
                free(D->fin);
                D->fin=NULL;
                D->ppio=NULL;
                D->cant--;
            }
            else{
                tNodo* aux = D->fin->ant;
                //aux->sig==NULL;
                free(D->fin);
                D->fin=aux;
                D->cant--;
            }
        }
        return toReturn;
    }

    int dequeVacio(tDeque* D){
        int toReturn;
        if(D->cant==0)
            toReturn = 1;
        else toReturn = 0;

        return toReturn;
    }


    int ocupacionDeque(tDeque * D){
         return D->cant;
    }


    void* deque2array(tDeque * D){

        void** arr = malloc(sizeof(void*)*D->cant);
        void *d;

        int i;

        tNodo *aux = D->ppio;

        for(i=0; i < D->cant; i++){

            arr[i] = aux->elem;
            aux = aux->sig;

        }

        d = arr;
        return d;
    }

