#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"
#include "tramite.h"
#include "listaordenada.h"

FILE *archivo_registro;

/**
 * Funcion encargada de ordenar los elementos de la lista
 * @param X elemento x a comparar
 * @param Y elemento y a comparar
 * @return devuelve 1 si el primer elemento es mayor al segundo, -1 si el primer elemento es menor, 0 si son iguales.
 *
 */
int comp(tTramite *x, tTramite *y)
{
    if(getLlegada(x)<getLlegada(y))
        return -1;
    if(getLlegada(x)>getLlegada(y))
    {
        return 1;
    }
    return 0;
}

/**
 * Funcion encargada de liberar puestos de atencion
 * @param puestos arreglo de tTramites
 * @param cantPuestos cantidad de puestos de atencion
 * @param tiempoActual tiempo actual
 * @param cambios notifica si se produjo una liberacion
 *
 */
void liberacionPuestos(tTramite** puestos, int cantPuestos, int tiempoActual, int *cambios)
{
    int nro;

    for (nro = 0; nro< cantPuestos; nro++)
    {
        if(puestos[nro]!=NULL)
        {
            int duracionActual = getDuracion(puestos[nro]);
            if (duracionActual > 0)
            {
                //El puesto esta atendiendo todavia el tramite
                setDuracion(puestos[nro],duracionActual-1);
            }
            //Libero si esta en Cero
            duracionActual = getDuracion(puestos[nro]);
            if (duracionActual == 0)
            {
                puestos[nro] = NULL;
                fprintf(archivo_registro,"Tiempo: %d | Se Libera el puesto %d\n"  ,tiempoActual,nro+1);

                //Hubo cambios

                *cambios = 1;
            }
        }
    }
}

/**
 * Funcion encargada de comenzar a atender al primer cliente de la cola, cuando el puesto no está atendiendo.
 * @param puestos arreglo de tTramite
 * @param cantPuestos cantidad de puestos de atencion
 * @param colasEsperas arreglo de tDeque
 * @param tiempoActual tiempo actual
 * @param cambios notifica si se comenzó a atender a un cliente
 *
 */
void comienzoAtencion(tTramite** puestos, int cantPuestos, tDeque** colasDeEspera, int tiempoActual, int *cambios)
{
    //Recorre los puestos de atencion. Si el puesto no esta atendiendo a nadie
    //entonces observa si hay gente esperando en su cola de espera
    //de haber gente, atiende al primero de ellos, siendo removido de la cola de espera

    int nro;
    for (nro = 0; nro< cantPuestos; nro++)
    {
        if (puestos[nro] == NULL)
        {

            if(ocupacionDeque(colasDeEspera[nro]) > 0)
            {
                puestos[nro] = eliminarAtras(colasDeEspera[nro]);
                fprintf(archivo_registro,"Tiempo: %d | El puesto %d atiende al siguiente de la cola\n"  ,tiempoActual,nro+1);
                *cambios = 1;
            }
        }
    }
}

/**
 * Funcion encargada de comenzar robar cliente a la cola vecina.
 * @param puestos arreglo de tTramite
 * 2param cantPuestos cantidad de puestos de atencion
 * @param colasEsperas arreglo de tDeque
 * @param tiempoActual tiempo actual
 * @param cambios notifica si se robó algún cliente
 *
 */
void roboClientes(tTramite** puestos, int cantPuestos, tDeque** colasDeEspera, int tiempoActual,int *cambios)
{
    int nro;
    int cliColaAnterior;
    int cliColaSiguiente;
    for (nro = 0; nro<cantPuestos-1; nro++)
    {
        if (puestos[nro] == NULL)
        {
            if (nro == 0)
            {
                //caso especial
                cliColaSiguiente = ocupacionDeque(colasDeEspera[nro+1]);
                if (cliColaSiguiente > 0 )
                {
                    puestos[nro] = eliminarAdelante(colasDeEspera[nro+1]);
                    fprintf(archivo_registro,"Tiempo: %d | El puesto %d le roba un cliente al puesto %d\n"  ,tiempoActual,nro+1,nro+2);
                    *cambios = 1;
                }
            }
            else
            {
                if (nro>0 && nro<cantPuestos-1)
                {
                    cliColaAnterior = ocupacionDeque(colasDeEspera[nro-1]);
                    cliColaSiguiente = ocupacionDeque(colasDeEspera[nro+1]);
                    if (cliColaAnterior > cliColaSiguiente )
                    {
                        puestos[nro] = eliminarAdelante(colasDeEspera[nro-1]);
                        fprintf(archivo_registro,"Tiempo: %d | El puesto %d le roba un cliente al puesto %d\n"  ,tiempoActual,nro+1,nro);
                        *cambios = 1;
                    }
                    else
                    {
                        if (cliColaSiguiente > 0 )
                        {
                            puestos[nro] = eliminarAdelante(colasDeEspera[nro+1]);
                            fprintf(archivo_registro,"Tiempo: %d | El puesto %d le roba un cliente al puesto %d\n"  ,tiempoActual,nro+1,nro+2);
                            *cambios = 1;
                        }
                    }
                }
            }
        }
    }
    if (puestos[nro] == NULL)
    {
        if (cantPuestos>1)
        {
            //Sali del ciclo sin evaluar el ultimo puesto
            cliColaAnterior = ocupacionDeque(colasDeEspera[nro-1]);
            if (cliColaAnterior > 0 )
            {

                puestos[nro] = eliminarAdelante(colasDeEspera[nro-1]);
                fprintf(archivo_registro,"Tiempo: %d | El puesto %d le roba un cliente al puesto %d\n"  ,tiempoActual,nro+1,nro);
                *cambios = 1;
            }
        }
    }
}

/**
 * Funcion encargada de indicar el primer puesto libre que encuentra.
 * @param puestos arreglo de tTramite
 * @param cantPuestos cantidad de puestos de atencion
 * @param colasEsperas arreglo de tDeque
 *
 */
int puestoLibre(tTramite** puestos, int cantPuestos, tDeque** colasDeEspera)
{
    int i;
    int termine = 0;
    for (i = 0; i<cantPuestos && termine == 0; i++)
    {
        if (puestos[i] == NULL && ocupacionDeque(colasDeEspera[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

/**
 * Funcion encargada de ubicar a los clientes en las colas de espera o en puestos vacíos
 * @param tiempoActual tiempo actual
 * @param listaLlegadas tListaOrdenada con los trámites ordenados segun su orden de llegada, menor a mayor.
 * @param puestos arreglo de tTramite
 * @param cantPuestos cantidad de puestos de atencion
 * @param colasEsperas arreglo de tDeque
 * @param cambios notifica si arribó al menos un nuevo cliente.
 *
 */
void arriboClientes(int tiempoActual, tListaOrdenada *listaLlegadas, tTramite** puestos, int cantPuestos, tDeque** colasDeEspera, int* cambios)
{
    tNodo* actual = primera(*listaLlegadas);
    tTramite* miTramite;
    int termine = 0;

    while(termine == 0)
    {
        miTramite =  elemento(actual);
        if(getLlegada(miTramite) <= tiempoActual)
        {
            if (getLlegada(miTramite) == tiempoActual)
            {
                //Existe al menos un cliente que llego en este momento
                int lugar = puestoLibre(puestos, cantPuestos, colasDeEspera);
                if (lugar>=0)
                {
                    puestos[lugar] = miTramite;
                    fprintf(archivo_registro,"Tiempo: %d | Llega %s a puesto %d\n"  ,tiempoActual,getNombre(miTramite),lugar+1);
                    *cambios = 1;
                }
                else
                {
                    //No encontro puesto libre, busca la cola de Espera con menos gente
                    int nroCola;
                    int menorCantidad = ocupacionDeque(colasDeEspera[0]);
                    int nroColaMenor = 0;
                    for(nroCola = 0; nroCola<cantPuestos; nroCola++)
                    {
                        if (menorCantidad > ocupacionDeque(colasDeEspera[nroCola]))
                        {
                            menorCantidad = ocupacionDeque(colasDeEspera[nroCola]);
                            nroColaMenor = nroCola;
                        }
                    }
                    //Insertar cliente en la cola de menos gente
                    insertarEnDeque(colasDeEspera[nroColaMenor],miTramite);
                    fprintf(archivo_registro,"Tiempo: %d | Llega %s a puesto %d\n"  ,tiempoActual,getNombre(miTramite),nroColaMenor+1);
                    *cambios = 1;
                }
            }
            actual = siguiente(actual);
            if (actual == NULL)
            {
                termine = 1;
            }
        }
        else
        {
            //El tiempo del cliente de llegada no es el actual por ende termino
            termine = 1;
            actual = NULL;
        }
    }

}

/**
 * Funcion encargada de mostrar la ayuda del sistema
 *
 */
void ayuda()
{
    printf("\nBienvenido al simulador de puestos de atencion.\n");
    printf("La sintaxis para utilizar el programa es la siguiente:\n\t");
    printf("-f numero entero :: Especifica el tiempo que durara el simulador. Este parametro es opcional\n\t");
    printf("-i archivo de texto :: Carga en el simulador el archivo que contiene los tramites disponibles con su respectiva duracion. El archivo DEBE llamarse 'Tramites.txt'\n\t");
    printf("-n entero :: Especifica la cantidad de puestos de atencion del simulador (debe ser mayor que cero).\n\t");
    printf("-o archivo de texto :: Especifica el archivo de salida, en el que se vera el estado del simulador. Este parametro es opcional. De no especificarse se mostrara el resultado por pantalla.\n\t");
    exit(0);
}

/**
 * Funcion encargada de cargar los trámites en una lista ordenada de menor a mayor
 * @param filename arreglo de caracteres con el nombre del archivo que debe procesar
 * @param duracion entero que indica la duración del trámite que se está insertando
 * @param listaLlegadas tListaOrdenada donde se cargarán los trámites
 *
 */
void asignarLlegadas(char filename[], int duracion, tListaOrdenada *listaLlegadas)
{
//Inserta en listaLlegadas, en orden las llegadas del tramite que recibio
//sabemos que son txt
//filename viene sin el .txt
    FILE *archivo;
    char *nombreT = malloc(250);
    strcpy(nombreT,filename);
    //le agrego el .txt
    strcat(filename,".txt");
    archivo = fopen(filename,"r");
    if (archivo == NULL)
    {
        printf("\nError de apertura del archivo. \n\n");
        ayuda();
    }
    else
    {
        //Lee hasta que termine el archivo

        while (!feof(archivo))
        {
            int llegada;
            //Lee hasta fin de linea y guarda el valor en llegada
            fscanf(archivo, "%d\n", &llegada);
            tTramite *miTramite = crearTramite(nombreT,duracion,llegada);
            insertar(listaLlegadas,miTramite);
        }
    }
    fclose(archivo);
}

/**
 * Funcion encargada de generar las colas de espera para cada puesto de atencion
 * \param cantPuestos cantidad de puestos de atencion
 * \return arreglo de tDeque
 *
 */
tDeque** generarColasDeEspera(int cantPuestos)
{
    //Genera de 0 a n-1 la cantidad de colasDeEspera
    tDeque** array = malloc(sizeof(tDeque*)*cantPuestos);
    int i;
    for (i=0; i<cantPuestos; i++)
    {
        array[i] = crearDeque();
    }
    return array;
}

/**
 * Funcion encargada de generar los puestos de atencion
 * @param cantPuestos cantidad de puestos de atencion
 * @return arreglo de tTramite
 *
 */
tTramite** generarPuestos(int cantPuestos)
{
    //Genera de 0 a n-1 la cantidad de puestosDeAtencion
    //array vacio
    tTramite** array2=malloc(sizeof(tTramite*)*cantPuestos);
    int i;
    for(i=0; i<cantPuestos; i++)
    {
        array2[i]=NULL;
    }
    return array2;
}

/**
 * Funcion encargada de armar un string que indicara que el simulador realizo alguna accion
 * @param cola arreglo de tDeque
 * @param visual String a modificar
 * @return devuelve el string modificado
 *
 */
void verCola(tDeque* cola, char* visual)
{

    tTramite** array = deque2array(cola);
    strcat(visual,"[ ");
    int n;
    for(n = 0; n<ocupacionDeque(cola); n++)
    {
        strcat(visual,getNombre(array[n]));
        strcat(visual," ");
    }
    strcat(visual,"]");
}

/**
 * Funcion encargada de escribir en el archivo_registro el estado actual
 * de los puestos y de sus colas de espera
 * @param puestos arreglo de tTramite
 * @param cantPuestos cantidad de puestos de atencion
 * @param colasEsperas arreglo de tDeque.
 */

void mostrar(tTramite** puestos, int cantPuestos,tDeque** colasDeEspera)
{
    int nro;
    for(nro=0; nro<cantPuestos; nro++)
    {
        char vista[100] = "";
        vista[0]='\0';
        verCola(colasDeEspera[nro],vista);
        if (puestos[nro] != NULL)
        {
            fprintf(archivo_registro,"\tPuesto: %d | %s | %s\n"  ,nro+1, getNombre(puestos[nro]) ,vista) ;
        }
        else
        {
            fprintf(archivo_registro,"\tPuesto: %d | %s | %s\n"  ,nro+1, " " ,vista) ;
        }
    }
}

/**
 * Funcion encargada de determinar si debe finalizar la simulacion
 * @param puestos arreglo de tTramite
 * @param colasEsperas arreglo de tDeque
 * @param cantPuestos cantidad de puestos de atencion
 * @param continuar bandera donde se guarda el resultado de la ejecucion de la funcion
 *
 */
void chequearTiempo(tTramite** puestos, tDeque** colasDeEspera,tListaOrdenada *listaLlegadas , int tactual,int cantPuestos, int *TERMINAR)
{
    int i;
    int seguir = 1;
    for(i = 0; i<cantPuestos && seguir == 1; i++)
    {

        //Falta controlar la lista de llegadas

        if((!(puestos[i] == NULL)) || (!(ocupacionDeque(colasDeEspera[i]) == 0)))
        {
            seguir = 0;
        }
        else
        {
            int bandera=0;
            tNodo* actual = primera(*listaLlegadas);
            while(actual!=NULL && bandera==0)
            {
                tTramite* t=elemento(actual);
                if(getLlegada(t)>tactual )
                {
                    seguir=0;
                    bandera=1;
                }
                if(bandera==0)
                    actual=siguiente(actual);
            }
        }
    }
    if(seguir == 1)
    {
        *TERMINAR = 0;
    }
}

/**
 * Metodo principal del simulador encargado de procesar el archivo de tramites según
 * la especificación dada por el usuario
 *@param argc indica la cantidad de parametros introducidos en la linea de comandos
 *@param argv contiene cada uno de los parametros recibidos de la linea de comandos
 *
 */
int main(int argc, char *argv[])
{

    int tiempoAux = 0;
    int pasoTiempo = 0;
    int cantPuestos = 0;
    char *filename_entrada;
    char *filename_salida;
    int bandera_archivo=0;

    if (argc == 1)//si tiene un solo argumento (el nombre del programa)
        ayuda();
    else
        // si tiene dos argumentos
        if(argc>=5)
        {
            if(strcmp(argv[1],"-h") ==0)//si el argumento no es "-h"
                ayuda();
            if(strcmp(argv[1],"-f") == 0 )
            {
                tiempoAux = atoi(argv[2]); //Sumamos 1 para que el tiempo llegue a lo indicado
                pasoTiempo = 1;

                if(strcmp(argv[3],"-i") == 0)  //Se indica archivo entrada
                {
                    if(strcmp(argv[4],"Tramites.txt")==0)
                        filename_entrada = argv[4];
                    else
                        ayuda();

                }
                else
                    ayuda();


                if(strcmp(argv[5],"-n") == 0)   //Indica cantidad Puestos
                {
                    cantPuestos = atoi(argv[6]);
                }
                else
                    ayuda();
                if(argc>7)
                {
                    if(strcmp(argv[7],"-o") == 0)   //Indica cantidad Puestos
                    {
                        filename_salida = argv[8];
                        bandera_archivo=1;
                    }
                }
            }
            else
            {
                if(strcmp(argv[1],"-i") == 0)  //Se indica archivo entrada
                {
                    filename_entrada = argv[2];
                }
                else
                    ayuda();
                if(strcmp(argv[3],"-n") == 0)   //Indica cantidad Puestos
                {
                    cantPuestos = atoi(argv[4]);
                }
                else
                    ayuda();
                if(argc>5)
                {
                    if(strcmp(argv[5],"-o") == 0)   //Indica cantidad Puestos
                    {
                        filename_salida = argv[6];
                        bandera_archivo=1;
                    }
                }
            }
        }
        else
            ayuda();


    if(cantPuestos==0){
        ayuda();
    }

    FILE *archivo = fopen(filename_entrada,"r");
    if(archivo==NULL)
    {
        printf("No se pudo cargar el archivo.");
        ayuda();
    }

    if(bandera_archivo)
        archivo_registro = fopen(filename_salida,"w");
    else
        archivo_registro=stdout;

    tListaOrdenada listaLlegadas = crearLista((int(*)(void*,void*))comp);

    char* name=malloc(250);
    int duracion=0;

    while (!feof(archivo))
    {
        fscanf(archivo, "%s %d", name, &duracion);
        asignarLlegadas(name, duracion, &listaLlegadas);
    }


    fclose(archivo);

    tDeque** colas=generarColasDeEspera(cantPuestos);
    tTramite** puestos=generarPuestos(cantPuestos);

    int tiempo;

    if(pasoTiempo)
    {
        tiempo = tiempoAux;
    }

    int i=0;

    int continuar=1;

    fprintf(archivo_registro, "Tiempo: 0 | Inicio del simulador\n");

    while(continuar)
    {
        int cambios = 0;
        if(i>0)
        {
            liberacionPuestos(puestos, cantPuestos,i,&cambios);
            comienzoAtencion(puestos,cantPuestos, colas,i, &cambios);
            arriboClientes(i, &listaLlegadas, puestos, cantPuestos, colas,&cambios);
            roboClientes(puestos, cantPuestos, colas,i, &cambios);

            if(cambios)
            {
                mostrar(puestos,cantPuestos,colas);
                fprintf(archivo_registro, "-------------------------------------------------------------\n");
            }
        }
        else
        {
            mostrar(puestos,cantPuestos,colas);
            fprintf(archivo_registro, "-------------------------------------------------------------\n");
        }
        if(pasoTiempo==0 && i!=0)
            chequearTiempo(puestos,colas,&listaLlegadas,i,cantPuestos,&continuar);

        i++;
        if(pasoTiempo==1)
        {
            if (i>tiempo)
            {
                continuar = 0;
            }
            else
            {
                continuar = 1;
            }
        }
    }
    fclose(archivo_registro);

    return 0;
}
