#ifndef _FLOW_STRUTTURE_H
#define _FLOW_STRUTTURE_H
#include "../my_defines.h"

/* Modifica il valore della cella puntata dalla variabile intera 'cursore
   con il valore di input.
   ATTENZIONE: Non fa nessun controllo */
void flow_struct_modificacellasovrastante(int valore, int cursore);

/* Inizializza le due strutture che controllano che delle coppie non vengono
   divise rendendo la soluzione complessiva impossibile
   ritorna '1' se viene eseguito con successo, '0' altrimenti */
int flow_struct_inizializzastruttureausiliarie();

/* Ogni volta che si cambia coppia bisogna ritoccare alcuni valori dei due
   array di interi */
int flow_struct_successivostruttureausiliarie();

/* L'operazione inversa di 'flow_struct_successivostruttureausiliarie' */
int flow_struct_precedentestruttureausiliarie();

/* Verifica che nessun intero delle strutture ausiliarie sia negativo, se lo
  fosse, proseguendo con l'algoritmo, non esiste soluzione complessiva.
  ritorna '0' se almeno un valore è negativo, '1' altrimenti */
int flow_struct_verificastruttureausiliarie(int cursore);


#endif
