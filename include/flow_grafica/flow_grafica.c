#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../var/var.hidden"
#include "../my_defines.h"
#include "./flow_grafica.h"
#include "../flow/flow.h"

int flow_grafica_stampagrigliaecontorno()
 {
   int i=-riga_flow,j;

   /* i parte da -base perchè pgrid punta alla prima cella libera e si vuole
      ora stampare pure il contorno superiore */
   for(;i<0;i++)
     {
       /* Stampa del contorno superiore */
       printf("%d ",*(pgrid+i));
     }

   printf("\n");

   for(j=0;j<altezza_flow;j++)
     {
       for(i=0;i<riga_flow;i++)
         {
           /* Stampa delle celle e del contorno laterale (0<=i<=base) */
           printf("%d ",*(pgrid+pos(i,j)));
         }
       printf("\n");
     }

   for(i=0;i<riga_flow;i++)
     {
       /* Stampa del contorno inferiore */
       printf("%d ",*(pgrid+pos(i,j)));
     }

   printf("\n");

   return 1;
 }

/* Non stampa il contorno */
void flow_grafica_stampagriglia()
 {
   int i,j,base;

   base=riga_flow-1;

   for(j=0;j<altezza_flow;j++)
     {
       for(i=0;i<base;i++)
         {
           /* Per stampare la griglia coi numeri nella maniera più ordinata
              possibile: se il numero stampato supera il 9, essendo a due cifre
              verrà stampato con uno spazio in meno */
           if( (*(pgrid+pos(i,j))/10)>0 )
             {
               printf(" %d",*(pgrid+pos(i,j)));
             }
           else{printf("  %d",*(pgrid+pos(i,j)));}
         }
       printf("\n");
     }

 }


/* Per una migliore resa dei colori consiglio di impostare la seguente
   Terminale > Edit > Profile Preferences > Colors > Palette
   e selezionare "Tango"   */
void flow_grafica_stampagrigliacolori()
 {
   //clear();
   if(*(pcoppie)>7)
     {
       flow_grafica_stampagriglia();

       return;
     }

   int i,j,k;

   k=1;

   printf("\x1b[30m");
   for(i=-1;i<riga_flow;i++){printf("██");}
   printf("\n");

   for(j=0;j<altezza_flow;j++)
     {
       i=0;
       printf("\x1b[30m");
       printf("██");

       for(i=0;i<riga_flow-1;i++)
         {
           /* Se la cella in questione è vuota stampa due spazi */
           if( (*(pgrid+pos(i,j))) ==0)
             {
               printf("\x1b[0m");
               printf("  ");
             }
           else
             {
               /* Se non è vuota le stampa col colore chiaro */
               printf("\x1b[%dm",(90+(*(pgrid+pos(i,j))+1)/2));

               printf("██");

               for(k=1;k<=numerocoppiexdue;k++)
                 {
                   /* Però se questa cella è occupata da uno dei due estremi di
                      una coppia le stampa col colore scuro */
                   if(pos(i,j)==(*(pcoppie+k)))
                     {
                       printf("\x1b[%dm",(30+(*(pgrid+pos(i,j))+1)/2));
                       printf("\b\b██");
                     }
                 }

             }
         }

         printf("\x1b[30m");
         printf("██\n");
     }

   for(i=-1;i<riga_flow;i++){printf("██");}
   printf("\n");
   printf("\x1b[0m"); /* reset del colore*/

  }

/* Usato esclusivamente a fini di debug */
void flow_grafica_stampastruttureausiliarie()
 {
   int i;

   printf("y: ");
   for(i=0;i<altezza_flow;i++)
    {
      printf("|%d",*(pcontrolx+i));
    }
   printf("|\n");

   printf("x: ");
   for(i=0;i<(riga_flow-1);i++)
    {
      printf("|%d",*(pcontroly+i));
    }
   printf("|\n\n");

 }
