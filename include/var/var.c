#include <stdio.h>
#include "../my_defines.h"

/*-----   VARIABILI GLOBALI   ----- */
pint pgrid; // Puntatore alla tabella
pint pcoppie; // Puntatore all'array delle coppie
pint pcontrolx, pcontroly; // Puntatore alle due strutture ausiliarie
int  altezza_flow,riga_flow; // Informazioni sulla griglia
int  cursorecoppia; // Informazione su la coppia attuale durante le funzioni ricorsive
int  numeropercorsi; // Informazione sul numero di percorsi durante le funzioni ricorsive
int  numerocoppiexdue;
/*--------------------------------- */
