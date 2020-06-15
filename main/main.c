#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "../include/flow/flow.h"
#include "../include/flow_grafica/flow_grafica.h"
#include "../include/flow_strutture/flow_strutture.h"
#include "../include/flow_app/flow_app.h"
#include "../include/flow_game/flow_game.h"

#define LATO 7
int main()
 {
/* ---------------------------------------------------------------------------*/
   /* ESEMPIO RISOLUTORE CLASSICO */
   //printf("Esempio risolutore classico\n");
   flow_inizializzagriglia(6,6);
   flow_generacoppie_scanf(1);
   flow_grafica_stampagrigliacolori();
   //flow_risolutore();
   /* Per trovare tutti i percorsi invece bisogna usare la Funzione
      flow_risolutoreprimopercorso(); */

/* ---------------------------------------------------------------------------*/

   /* ESEMPIO RISOLUTORE LIVELLI APP */

   //rintf("Esempio risolutore livelli app\n");
   //flow_app_risolutorelivello(1,33); // Pacchetto 6 - Livello 100


/* ---------------------------------------------------------------------------*/

   /* ESEMPIO GIOCO DA TERMINALE */
   /*printf("Esempio gioco da terminale\n");
   flow_game_start(7,7,7); // Griglia 7x7 con 7 coppie da collegare
   */
/* ---------------------------------------------------------------------------*/

   return 1;
 }
