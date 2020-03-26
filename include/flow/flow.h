#ifndef _FLOW_H
#define _FLOW_H
#include "../my_defines.h"

/* Inizializza la griglia con tutte le celle libere
   ritorna '1' se viene eseguito con successo, '0' altrimenti */
int flow_inizializzagriglia(int base, int altezza);

/* Inizializza le coppie con le posizioni casuali
  ritorna '1' se viene eseguito con successo, '0' altrimenti */
int flow_generacoppie_rand(int numerocoppie);

/* Inizializza le coppie tramite input da tastiera
   ritorna '1' se viene eseguito con successo, '0' altrimenti */
int flow_generacoppie_scanf(int numerocoppie);

/* Imposta le strutture ausiliarie, variabili e fa partire la funzione
   ricorsiva
   ritorna '0' se c'è stato qualche problema nell'inizializzazione delle
   strutture */
int flow_risolutore();

/* Imposta le strutture ausiliarie, variabili e fa partire la funzione
   ricorsiva
   ritorna '0' se c'è stato qualche problema nell'inizializzazione delle
   strutture */
int flow_risolutorenumeropercorsi();

/* Imposta le strutture ausiliarie, variabili e fa partire la funzione
   ricorsiva
   ritorna '0' se c'è stato qualche problema nell'inizializzazione delle
   strutture */
int flow_risolutoreprimopercorso();



#endif
