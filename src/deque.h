#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED

    typedef struct Deque tDeque;   //tDeque es el tipo de la estructura Deque

    // Crea una Deque vacía y la devuelve.
    tDeque* crearDeque();

    //Inserta el elemento x al final de la cola.
    void insertarEnDeque(tDeque* D, void * x);

     // Elimina y devuelve el primer elemento de la Deque.
    void* eliminarAtras(tDeque* D);

    // Elimina y devuelve el último elemento de la Deque.
    void* eliminarAdelante(tDeque* D);

    // Devuelve verdadero (6= 0) si la Deque está vacía, y falso (0) si contiene al menos un elemento.
    int dequeVacio(tDeque* D);

    // Devuelve la cantidad de elementos que tiene la Deque.
    int ocupacionDeque(tDeque * D);

    // Devuelve un arreglo con todos los elementos de la Deque.
    void * deque2array(tDeque * D);


#endif // DEQUE_H_INCLUDED
