#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../var/var.hidden"
#include "./flow_strutture.h"
#include "../my_defines.h"

/* Modifica il valore della cella puntata dalla variabile intera 'cursore
   con il valore di input.
   ATTENZIONE: Non fa nessun controllo */
void flow_struct_modificacellasovrastante(int valore, int cursore)
 {
   *(pgrid+cursore)=valore;
 }

/*
  Le strutture ausiliarie sono due array di interi, uno contenente 'base' interi
  l'altro contenente 'altezza_flow' interi, come in figura.
  Si inizializzano nella seguente maniera:
   1) Per ogni cella della struttura ausiliaria, si contano quante celle libere
      della tabella ci sono nella loro rispettiva riga_flow o colonna
   2) Dopo di che si leva a quel numero, il numero di coppie che quella riga_flow o
      colonna ha diviso
         ____________                      ____________
  1)    |_2|_3|_2|_1|               2)    |_2|_2|_1|_1|
 ___    ____________               ___    ____________
|_1|   |_1|__|_2|_2|     >>>      |_1|   |_1|__|_2|_2|
|_4|   |__|__|__|__|              |_3|   |__|__|__|__|
|_3|   |__|__|__|_1|              |_3|   |__|__|__|_1|

  Saranno costruite tale che se il numero di una cella diventa negativo, allora
  la soluzione è impossibile */
int flow_struct_inizializzastruttureausiliarie()
 {
   int i,j,k,t;

   /*printf("npunti: %d\n",numeropunti);*/

   /* Allochiamo lo spazio in memoria per i due array */
   if((pcontrolx=(pint)malloc(sizeof(int)*altezza_flow))==NULL)
     {
       printf("\n !! Impossibile inizializzare le strutture ausiliarie !!\n");

       return 0;
     }

   if((pcontroly=(pint)malloc(sizeof(int)*(riga_flow-1)))==NULL)
     {
       printf("\n !! Impossibile inizializzare le strutture ausiliarie !!\n");
       free(pcontrolx);

       return 0;
     }

  /* Qui inizia il ciclo per inizializzare l'array pcontrolx (quello verticale
     in figura)*/
   for(i=0;i<altezza_flow;i++)
     {
       k=0;

       for(j=0;j<(riga_flow-1);j++)
         {
           /* Il punto 1, conta le celle libera nella riga_flow i-esima */
           if( *(pgrid+pos(j,i))==0 ){k++;}
         }

       *(pcontrolx+i)=k;
       k=0;

       /* Si deve ora levare al valore il numero di coppie divise */
       for(t=1;t<=numerocoppiexdue;)
         {
           //printf("%d < %d e %d < %d ?\n",*(pcoppie+t),i*riga_flow,*(pcoppie+t+1),(i+1)*riga_flow);
           if(  (  (*(pcoppie+t)<(i*riga_flow))&&(*(pcoppie+t+1)>=((i+1)*riga_flow))  )||
                (  (*(pcoppie+t+1)<(i*riga_flow))&&(*(pcoppie+t)>=((i+1)*riga_flow))  ) )
             {
               //printf("aaa\n");
               k++;
             }

           t=t+2;
         }

       *(pcontrolx+i)-=k;
     }

  /* Ciclo per pcontroly, è tutto uguale, cambiano gli if per vedere se i punti
     sono a destra o a sinistra */
   for(i=0;i<(riga_flow-1);i++)
     {
       k=0;
       for(j=0;j<altezza_flow;j++)
         {
           if( *(pgrid+pos(i,j))==0 ){k++;}
         }

       *(pcontroly+i)=k;

       k=0;

       for(t=1;t<=numerocoppiexdue;)
         {
          /* Si è notato che per una griglia dotata di contorno come in
             'flow_inizializzagriglia()' per tutte le celle della colonna i-esima
              vale la seguente relazione:
                       (posizione)%(riga_flow)=i        */
           if((((*(pcoppie+t)%(riga_flow))<i)&&((*(pcoppie+t+1)%(riga_flow))>i))||
              (((*(pcoppie+t)%(riga_flow))>i)&&((*(pcoppie+t+1)%(riga_flow))<i)))
             {
               k++;
             }

           t+=2;
         }

       *(pcontroly+i)-=k;

     }


   return 1;
 }

int flow_struct_successivostruttureausiliarie()
 {
   int x1,x2,y1,y2,temp;

   x1= (*(pcoppie+cursorecoppia)%(riga_flow));
   y1= (*(pcoppie+cursorecoppia)/(riga_flow));

   x2= (*(pcoppie+cursorecoppia+1)%(riga_flow));
   y2= (*(pcoppie+cursorecoppia+1)/(riga_flow));

   //printf("(%d,%d)  (%d,%d)\n",x1,y1,x2,y2);

   if(x2<x1)
     {
       temp=x1;
       x1=x2;
       x2=temp;
     }

   //printf("x:(%d,%d)\n",x1,x2);

   if(y2<y1)
     {
       temp=y1;
       y1=y2;
       y2=temp;
     }

   temp=y1+1;

   for(;temp<y2;temp++)
     {
       (*(pcontrolx+temp))+=1;
     }

   temp=x1+1;
   for(;temp<x2;temp++)
     {
       (*(pcontroly+temp))+=1;
     }


   return 1;
 }


int flow_struct_precedentestruttureausiliarie()
{
  int x1,x2,y1,y2,temp;

  x1= (*(pcoppie+cursorecoppia)%(riga_flow));
  y1= (*(pcoppie+cursorecoppia)/(riga_flow));

  x2= (*(pcoppie+cursorecoppia+1)%(riga_flow));
  y2= (*(pcoppie+cursorecoppia+1)/(riga_flow));

  /*printf("(%d,%d)  (%d,%d)\n",x1,y1,x2,y2);*/

  if(x2<x1)
    {
      temp=x1;
      x1=x2;
      x2=temp;
    }

  //printf("x:(%d,%d)\n",x1,x2);

  if(y2<y1)
    {
      temp=y1;
      y1=y2;
      y2=temp;
    }

  temp=y1+1;

  for(;temp<y2;temp++)
    {
      (*(pcontrolx+temp))-=1;
    }

  temp=x1+1;
  for(;temp<x2;temp++)
    {
      (*(pcontroly+temp))-=1;
    }


  return 1;
}

int flow_struct_verificastruttureausiliarie(int cursore)
 {
   //printf("[%d ,%d]\n",(*(pcontroly+(cursore%(riga_flow)))),(*(pcontrolx+(cursore/(riga_flow)))));
   if(((*(pcontroly+(cursore%(riga_flow))))<1))
     {
       //printf("prova\n");
       return 0;
     }

  if(((*(pcontrolx+(cursore/(riga_flow)))))<1)
  {
    //printf("prova\n");
    return 0;
  }

   return 1;
 }
