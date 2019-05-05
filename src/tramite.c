#include <stdio.h>
#include <stdlib.h>
#include "tramite.h"


typedef struct Tramite{
    char * nombre;
    int duracion;
    int llegada;
}tTramite;

tTramite* crearTramite(char* c, int d, int l)
{
    tTramite* tramite = malloc(sizeof(tTramite));
    tramite->nombre=c;
    tramite->duracion=d;
    tramite->llegada=l;

    return tramite;
}

char* getNombre(tTramite* tram){
    if(tram==NULL)
        return NULL;

    return tram->nombre;
}

int getDuracion(tTramite* tram)
{
    return tram->duracion;
}

int getLlegada(tTramite* tram)
{
    return tram->llegada;
}

void setDuracion(tTramite* tram, int nro){
    if(tram!=NULL)
        tram->duracion = nro;
}
