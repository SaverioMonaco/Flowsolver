#ifndef _FLOW_GRAFICA_H
#define _FLOW_GRAFICA_H

/* Stampa su schermo lo stato corrente della griglia
   ritorna '1' se viene eseguito con successo, '0' altrimenti */
void flow_grafica_stampagriglia();

/* stampa su schermo lo stato corrente della griglia con diversi colori, e
   anziché usare i numeri, vengono stampati dei blocchi '██'
   Stampa mediante numeri se si prova a stampare una griglia con più di 7 coppie
   (perché sono finiti i colori)
   Per una migliore resa dei colori consiglio di impostare la seguente
   Terminale > Edit > Profile Preferences > Colors > Palette
   e selezionare "Tango"   */
void flow_grafica_stampagrigliacolori();

void flow_grafica_stampastruttureausiliarie();

#endif
