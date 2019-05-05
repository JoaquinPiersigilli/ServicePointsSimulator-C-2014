#ifndef TRAMITE_H_INCLUDED
#define TRAMITE_H_INCLUDED


typedef struct Tramite tTramite;

//Signatures

tTramite* crearTramite(char* c, int d, int l);

char* getNombre(tTramite* tram);

int getDuracion(tTramite* tram);

int getLlegada(tTramite* tram);

void setDuracion(tTramite* tram, int nro);




#endif // TRAMITE_H_INCLUDED
