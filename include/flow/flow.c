#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "../var/var.hidden"
#include "../my_defines.h"
#include "./flow.h"
#include "./flow_hidden.h"
#include "../flow_grafica/flow_grafica.h"
#include "../flow_strutture/flow_strutture.h"

/*  Inizializza un array monodimensionale corrispondente ad una griglia
    di dimensione base x altezza
    _______________
    |//|//|//|//|//|
    |__|__|__|__|//|
    |__|__|__|__|//|   altezza
    |__|__|__|__|//|
    |//|//|//|//|//|
         base

   Le celle |__| sono quelle proprie della griglia, mentre quelle
   |//| sono celle occupate che fungono da contorno */
int flow_inizializzagriglia(int base, int altezza)
 {
   /* Numero di int da mettere nella malloc (Contorno compreso) */
   int numerocelle;
   int i,j; /* Due variabili per il ciclo */
   altezza_flow=altezza;
   riga_flow=base+1; /* Base + 1 : comprende anche il contorno laterale */
                  /* Inizializzo riga_flow e non base perché
                     nelle funzioni della ricerca dei percorsi serve
                     quasi esclusivamente base+1 */

   if( (altezza_flow<1)||(riga_flow<2) ){printf("Errore: tabella non valida!\n"); return 0;}

   numerocelle=(altezza_flow+2)*(riga_flow); /* Vedi figura */

   if((pgrid=(pint)malloc(sizeof(int)*numerocelle))==NULL)
     {
       printf("\n !! Errore di memoria: impossibile inizializzare la griglia !!\n");
       return 0;
     }

   /* Inizializziamo quindi ogni singola cella:
    * Le celle libere sono definite dall'intero 0
    * Il contorno è dato da -1 */
   for(i=0;i<riga_flow;i++){*(pgrid+i)=CONTORNO;}

   /* Sposto il puntatore per segnare la prima cella libera */
   pgrid=pgrid+riga_flow;

   for(j=0;j<altezza_flow;j++)
     {
       for(i=0;i<(riga_flow-1);i++)
         {
           *(pgrid+pos(i,j))=0;
         }

       *(pgrid+pos(i,j))=CONTORNO;
     }

   for(i=0;i<riga_flow;i++){*(pgrid+pos(i,j))=CONTORNO;/*printf("%d\n",pos(i,j));*/}

   return 1;
 }

/* Inizializza un array che contiene le seguenti informazioni
    > Numero di coppie;
    > Posizione dei singoli punti da collegare.
    __________________________________
   | 3  |  0 |  2 |  6 | 11 |  5 | 13 |
   |____|____|____|____|____|____|____|
     ^     ^    ^    ^    ^
     ^     ^    ^   Posizioni del primo e del secondo punto della seconda coppia
     ^    Posizioni del primo e del secondo punto della prima coppia
    Numero di coppie */
/* Inizializza i punti in maniera del tutto casuale */
int flow_generacoppie_rand(int numerocoppie)
 {
   int x,y,i,j;

   if(numerocoppie<1){printf("Errore: numero di coppie troppo basso!"); return 0;}

   numerocoppiexdue=2*numerocoppie;

   /*Controlliamo che il numero di punti non ecceda il numero di celle disponibili*/
   if(numerocoppiexdue>((riga_flow-1)*altezza_flow))
     {
       printf("Numero di coppie troppo grande per la tabella considerata!\n");

       return 0;
     }

   /*Allochiamo lo spazio in memoria*/
   if((pcoppie=(pint)malloc(sizeof(int)*(numerocoppiexdue+1)))==NULL)
     {
       printf("\n !! Errore di memoria: impossibile inizializzare l'array delle coppie !!\n");

       return 0;
     }

   srand(time(NULL));

   /* Come prima cella mettiamo il numero delle coppie */
   *(pcoppie)=numerocoppie;

   for(i=1;i<=numerocoppiexdue;i++)
     {
       x=(int)( (((float)rand())/RAND_MAX)*(riga_flow-1) );
       y=(int)( (((float)rand())/RAND_MAX)*altezza_flow );
       *(pcoppie+i)=pos(x,y);
       for(j=1;j<i;j++)
         {
           if( *(pcoppie+j)==(*(pcoppie+i)) ){i--; break;}
         }
     }

   for(i=1;i<=numerocoppiexdue;i++)
     {
       /*printf("%d\n",*(pcoppie+i));*/
       flow_struct_modificacellasovrastante(i,(*(pcoppie+i)));
     }

   return 1;
 }

/* Inizializza i punti immettendoli da tastiera */
int flow_generacoppie_scanf(int numerocoppie)
 {
   int x,y,i,j;

   if(numerocoppie<1){printf("Errore: numero di coppie troppo basso!"); return 0;}

   numerocoppiexdue=2*numerocoppie;

   /*Controlliamo che il numero di punti non ecceda il numero di celle disponibili*/
   if(numerocoppiexdue>((riga_flow-1)*altezza_flow))
     {
       printf("Numero di coppie troppo grande per la tabella considerata!\n");

       return 0;
     }

   /*Allochiamo lo spazio in memoria*/
   if((pcoppie=(pint)malloc(sizeof(int)*(numerocoppiexdue+1)))==NULL)
     {
       printf("\n !! Errore di memoria: impossibile inizializzare l'array delle coppie !!\n");

       return 0;
     }

   *(pcoppie)=numerocoppie;
   /* Da qui inizia un ciclo abbastanza contorto */
   for(i=1;i<=numerocoppiexdue;)
     {
       /* j serve dopo, lo inizializzo ad 1 per motivi spiegati sotto */
       j=1;

       /* Facciamo la scanf della coordinata x del primo punto */
       printf("Coppia %d - Punto 1 x=",(i+i%2)/2);
       scanf("%d",&x);

       /* Prima verifica se il punto è lecito, non deve sforare i bordi! */
       while( (x<0)||(x>(riga_flow)))
         {
           printf("Inserire un punto valido!\n");
           printf("Coppia %d - Punto 1 x=",(i+i%2)/2);
           scanf("%d",&x);
         }

       /* Analogo per la coordinata y del primo punto */
       printf("Coppia %d - Punto 1 y=",(i+i%2)/2);
       scanf("%d",&y);

       while( (y<0)||(y>=altezza_flow))
         {
           printf("Inserire un punto valido!\n");
           printf("Coppia %d - Punto 1 y=",(i+i%2)/2);
           scanf("%d",&y);
         }

       /* Scriviamo nell'array delle coppie */
       *(pcoppie+i)=pos(x,y);

       /* Passiamo al secondo punto della coppia */
       i++;
       printf("\n");

       printf("Coppia %d - Punto 2 x=",(i-1+(i-1)%2)/2);
       scanf("%d",&x);

       while( (x<0)||(x>(riga_flow)))
         {
           printf("Inserire un punto valido!\n");
           printf("Coppia %d - Punto 1 x=",(i+i%2)/2);
           scanf("%d",&x);
         }

       printf("Coppia %d - Punto 2 y=",(i-1+(i-1)%2)/2);
       scanf("%d",&y);

       while( (y<0)||(y>=altezza_flow))
         {
           printf("Inserire un punto valido!\n");
           printf("Coppia %d - Punto 1 y=",(i+i%2)/2);
           scanf("%d",&y);
         }

       *(pcoppie+i)=pos(x,y);

       printf("\n");

       /* Da qui dobbiamo fare altre verifiche */

       /* I due punti della coppia sono uguali? */
       if( (*(pcoppie+i))==(*(pcoppie+i-1)))
         {
           printf("I punti della ultima coppia coincidono, riprovare:\n\n");
           i=i-2;

           /* Già i due punti non vanno bene, voglio resettare il ciclo senza cadere
              in quello dopo annidato */
           j=i;
         }

       /* Esistono dei punti già salvati nell'array con le stesse coordinate di
          quelli appena immessi? */
       for(;j<i-1;j++)
         {
           if( ((*(pcoppie+j))==(*(pcoppie+i)))||((*(pcoppie+j))==(*(pcoppie+i-1))) )
             {
               printf("Due punti coincidono, riprovare:\n");
               j=i;
               i=i-2;
             }
         }

       i++;
       /* fine ciclo */
     }

   /* Scriviamo tutto dall'array delle coppie alla griglia */
   for(i=1;i<=numerocoppiexdue;i++)
     {
       /*printf("%d\n",*(pcoppie+i));*/ /* Stampo l'array delle coppie per verificare */
       flow_struct_modificacellasovrastante(i,(*(pcoppie+i)));
     }

   return 1;
 }

/* Trova percorsi ed flow_inizializzaprossimacoppia si includono a vicenda, quindi
   devo predichiarare prima uno dei due */
void flow_inizializzaprossimacoppia();
void flow_inizializzaprossimacoppiasenzastampa();
int flow_inizializzaprossimacoppiaprimopercorso();

void flow_trovapercorsi(int cursore)
 {
   /* Verifica se la cella sovrastante è libera */
   if(*(pgrid+cursore)==0)
     {
       /* Una volta visto che la cella è libera, controlla se può fare la mossa
          verificando le strutture ausiliarie */
       if(!flow_struct_verificastruttureausiliarie(cursore))
         {

           /*printf(">>> ATTENZIONE <<<\n");
           flow_grafica_stampagrigliacolori();
           flow_grafica_stampastruttureausiliarie();*/

           return;
         }

       *(pgrid+cursore)=cursorecoppia;
       (*(pcontroly+(cursore%(riga_flow))))--;
       (*(pcontrolx+(cursore/(riga_flow))))--;

       // DEBUG
       /*flow_grafica_stampagrigliacolori();
       flow_grafica_stampastruttureausiliarie();
       usleep(WAITTIME);*/

       /* L'ordine di percorrenza della griglia è:

          SU SINISTRA GIU' DESTRA

      */
       flow_trovapercorsi(cursore-riga_flow);
       flow_trovapercorsi(cursore-1);
       flow_trovapercorsi(cursore+riga_flow);
       flow_trovapercorsi(cursore+1);

       /* Una volta arrivato qui non ci sono altre opzioni che tornare indietro */
       flow_struct_modificacellasovrastante(0,cursore);
       (*(pcontroly+(cursore%(riga_flow))))++;
       (*(pcontrolx+(cursore/(riga_flow))))++;

       // DEBUG
       /*flow_grafica_stampagrigliacolori();
       flow_grafica_stampastruttureausiliarie();
       usleep(WAITTIME);*/

       return;
     }

   /* Verifica se siamo arrivati alla soluzione */
   if((*(pgrid+cursore))==(cursorecoppia+1))
     {
       /* Se così fosse allora passiamo alle soluzioni della prossima coppia */
       flow_inizializzaprossimacoppia();
     }

 }

void flow_trovapercorsisenzastampa(int cursore)
 {
   /* Verifica se la cella sovrastante è libera */
   if(*(pgrid+cursore)==0)
     {
       /* Una volta visto che la cella è libera, controlla se può fare la mossa
          verificando le strutture ausiliarie */
       if(!flow_struct_verificastruttureausiliarie(cursore))
         {

           /*printf(">>> ATTENZIONE <<<\n");
           flow_grafica_stampagrigliacolori();
           flow_grafica_stampastruttureausiliarie();*/

           return;
         }

       *(pgrid+cursore)=cursorecoppia;
       (*(pcontroly+(cursore%(riga_flow))))--;
       (*(pcontrolx+(cursore/(riga_flow))))--;

       // DEBUG
       /*flow_grafica_stampagrigliacolori();
       flow_grafica_stampastruttureausiliarie();
      usleep(WAITTIME);*/

       /* L'ordine di percorrenza della griglia è:

          SU SINISTRA GIU' DESTRA     */
       flow_trovapercorsisenzastampa(cursore-riga_flow);
       flow_trovapercorsisenzastampa(cursore-1);
       flow_trovapercorsisenzastampa(cursore+riga_flow);
       flow_trovapercorsisenzastampa(cursore+1);

       /* Una volta arrivato qui non ci sono altre opzioni che tornare indietro */
       flow_struct_modificacellasovrastante(0,cursore);
       (*(pcontroly+(cursore%(riga_flow))))++;
       (*(pcontrolx+(cursore/(riga_flow))))++;

       // DEBUG
       /*flow_grafica_stampagrigliacolori();
       flow_grafica_stampastruttureausiliarie();
       usleep(WAITTIME);*/

       return;
    }

   /* Verifica se siamo arrivati alla soluzione */
   if((*(pgrid+cursore))==(cursorecoppia+1))
     {
       /* Se così fosse allora passiamo alle soluzioni della prossima coppia */
       flow_inizializzaprossimacoppiasenzastampa();
     }

 }

int flow_trovaprimopercorso(int cursore)
 {
   /* Verifica se la cella sovrastante è libera */
   if(*(pgrid+cursore)==0)
     {
       /* Una volta visto che la cella è libera, controlla se può fare la mossa
          verificando le strutture ausiliarie */
       if(!flow_struct_verificastruttureausiliarie(cursore))
         {

           /*printf(">>> ATTENZIONE <<<\n");
           flow_grafica_stampagrigliacolori();
           flow_grafica_stampastruttureausiliarie();*/

           return 0;
         }

       *(pgrid+cursore)=cursorecoppia;
       (*(pcontroly+(cursore%(riga_flow))))--;
       (*(pcontrolx+(cursore/(riga_flow))))--;

       // DEBUG
       /*flow_grafica_stampagrigliacolori();
       flow_grafica_stampastruttureausiliarie();
       usleep(WAITTIME);*/

       /* L'ordine di percorrenza della griglia è:

          SU SINISTRA GIU' DESTRA

      */
       if(flow_trovaprimopercorso(cursore-riga_flow)) {return 1;}
       if(flow_trovaprimopercorso(cursore-1)) {return 1;}
       if(flow_trovaprimopercorso(cursore+riga_flow)) {return 1;}
       if(flow_trovaprimopercorso(cursore+1)) {return 1;}

       /* Una volta arrivato qui non ci sono altre opzioni che tornare indietro */
       flow_struct_modificacellasovrastante(0,cursore);
       (*(pcontroly+(cursore%(riga_flow))))++;
       (*(pcontrolx+(cursore/(riga_flow))))++;

       // DEBUG
       /*flow_grafica_stampagrigliacolori();
       flow_grafica_stampastruttureausiliarie();
       usleep(WAITTIME);*/

       return 0;
     }

   /* Verifica se siamo arrivati alla soluzione */
   if((*(pgrid+cursore))==(cursorecoppia+1))
     {
       /* Se così fosse allora passiamo alle soluzioni della prossima coppia */
       if(flow_inizializzaprossimacoppiaprimopercorso()){return 1;}
     }

   return 0;

 }

void flow_inizializzaprossimacoppia()
 {

   cursorecoppia+=2;

   /* Se abbiamo finito i tubi allora in questo punto il cursore delle pcoppie
      è maggiore delle coppie x 2 (ogni coppia è caratterizzata da due numeri) */
   if(cursorecoppia>(*(pcoppie)*2))
     {
       /* Stampiamo quindi la soluzione complessiva */
       flow_grafica_stampagrigliacolori();

       /* Ed aggiungiamo +1 alla variabile globale dei percorsi */
       numeropercorsi++;
       printf("------------------\nPercorso: #%d\n------------------\n\n\n",numeropercorsi);

       /* Adesso si torna indietro per cercare altre soluzioni */
       cursorecoppia-=2;

       return;
     }

   flow_struct_successivostruttureausiliarie();

   flow_trovapercorsi(*(pcoppie+cursorecoppia)-riga_flow);
   flow_trovapercorsi(*(pcoppie+cursorecoppia)-1);
   flow_trovapercorsi(*(pcoppie+cursorecoppia)+riga_flow);
   flow_trovapercorsi(*(pcoppie+cursorecoppia)+1);

   flow_struct_precedentestruttureausiliarie();

   cursorecoppia-=2;

   return;
 }

void flow_inizializzaprossimacoppiasenzastampa()
 {

   cursorecoppia+=2;

   /* Se abbiamo finito i tubi allora in questo punto il cursore delle pcoppie
      è maggiore delle coppie x 2 (ogni coppia è caratterizzata da due numeri) */
   if(cursorecoppia>(*(pcoppie)*2))
     {
       /* Ed aggiungiamo +1 alla variabile globale dei percorsi */
       numeropercorsi++;

       /* Adesso si torna indietro per cercare altre soluzioni */
       cursorecoppia-=2;

       return;
     }

   flow_struct_successivostruttureausiliarie();

   flow_trovapercorsisenzastampa(*(pcoppie+cursorecoppia)-riga_flow);
   flow_trovapercorsisenzastampa(*(pcoppie+cursorecoppia)-1);
   flow_trovapercorsisenzastampa(*(pcoppie+cursorecoppia)+riga_flow);
   flow_trovapercorsisenzastampa(*(pcoppie+cursorecoppia)+1);

   flow_struct_precedentestruttureausiliarie();

   cursorecoppia-=2;

   return;
}

int flow_inizializzaprossimacoppiaprimopercorso()
 {
   flow_grafica_stampagrigliacolori();printf("\n");
   cursorecoppia+=2;

   /* Se abbiamo finito i tubi allora in questo punto il cursore delle pcoppie
      è maggiore delle coppie x 2 (ogni coppia è caratterizzata da due numeri) */
   if(cursorecoppia>(*(pcoppie)*2))
     {
       /* Stampiamo quindi la soluzione complessiva */
       flow_grafica_stampagrigliacolori();

       return 1;
     }

   flow_struct_successivostruttureausiliarie();

   if(flow_trovaprimopercorso(*(pcoppie+cursorecoppia)-riga_flow)) {return 1;}
   if(flow_trovaprimopercorso(*(pcoppie+cursorecoppia)-1)) {return 1;}
   if(flow_trovaprimopercorso(*(pcoppie+cursorecoppia)+riga_flow)) {return 1;}
   if(flow_trovaprimopercorso(*(pcoppie+cursorecoppia)+1)) {return 1;}

   flow_struct_precedentestruttureausiliarie();

   cursorecoppia-=2;

   return 0;
 }

int flow_risolutore()
 {
   /* Scriviamo all'inizio le informazioni basilari del caso */
   printf("\n%dx%d  %d coppie\n",riga_flow-1,altezza_flow,*(pcoppie));
   printf("\n");

   /* Prova a stampare la griglia a colori, se le coppie sono troppe li stampare
      coi numeri */
   flow_grafica_stampagrigliacolori();

   /* Check sulle inizializzazioni delle strutture ausiliarie */
   if(!flow_struct_inizializzastruttureausiliarie())
     {
      return 0;
     }

   /*flow_grafica_stampastruttureausiliarie();*/
   printf("\n____________________\n\n\n\n");

   /* Inizializzazione delle variabili */
   numeropercorsi=0;
   cursorecoppia=-1;

   /* Inizializzo delle variabili per dirmi il tempo di esecuzione */
   clock_t start,end;
   double cpu_time_used;

   start=clock();

   /* La funzione ricorsiva parte da qui */
   flow_inizializzaprossimacoppia();
   /* E termina qui */

   end=clock();

   cpu_time_used=((double) (end-start))/CLOCKS_PER_SEC;
   printf("\nPercorsi = %d\n   Tempo = %lf (s)\n",numeropercorsi,cpu_time_used);

   return 1;
 }

int flow_risolutorenumeropercorsi()
 {
   /* Scriviamo all'inizio le informazioni basilari del caso */
   printf("\n%dx%d  %d coppie\n",riga_flow-1,altezza_flow,*(pcoppie));
   printf("\n");

   /* Prova a stampare la griglia a colori, se le coppie sono troppe li stampare
      coi numeri */
   flow_grafica_stampagrigliacolori();

   /* Check sulle inizializzazioni delle strutture ausiliarie */
   if(!flow_struct_inizializzastruttureausiliarie())
     {
      return 0;
     }

   /*flow_grafica_stampastruttureausiliarie();*/
   printf("\n____________________\n\n\n\n");

   /* Inizializzazione delle variabili */
   numeropercorsi=0;
   cursorecoppia=-1;

   /* Inizializzo delle variabili per dirmi il tempo di esecuzione */
   clock_t start,end;
   double cpu_time_used;

   start=clock();

   /* La funzione ricorsiva parte da qui */
   flow_inizializzaprossimacoppiasenzastampa();
   /* E termina qui */

   end=clock();

   cpu_time_used=((double) (end-start))/CLOCKS_PER_SEC;
   printf("\nPercorsi = %d\n   Tempo = %lf (s)\n",numeropercorsi,cpu_time_used);

   return 1;

}

int flow_risolutoreprimopercorso()
 {
   /* Scriviamo all'inizio le informazioni basilari del caso */
   printf("\n%dx%d  %d coppie\n",riga_flow-1,altezza_flow,*(pcoppie));
   printf("\n");

   /* Prova a stampare la griglia a colori, se le coppie sono troppe li stampare
      coi numeri */
   flow_grafica_stampagrigliacolori();
   flow_grafica_stampastruttureausiliarie();
   /* Check sulle inizializzazioni delle strutture ausiliarie */
   if(!flow_struct_inizializzastruttureausiliarie())
     {
      return 0;
     }

   /*flow_grafica_stampastruttureausiliarie();*/
   printf("\n____________________\n\n\n\n");

   /* Inizializzazione delle variabili */
   numeropercorsi=0;
   cursorecoppia=-1;

   /* Inizializzo delle variabili per dirmi il tempo di esecuzione */
   clock_t start,end;
   double cpu_time_used;

   start=clock();

   /* La funzione ricorsiva parte da qui */
   if(!flow_inizializzaprossimacoppiaprimopercorso())
     {
       end=clock();

       cpu_time_used=((double) (end-start))/CLOCKS_PER_SEC;
       printf("Nessun percorso trovato\nTempo = %lf (s)\n",cpu_time_used);

       return 0;
     }
   /* E termina qui */

   end=clock();

   cpu_time_used=((double) (end-start))/CLOCKS_PER_SEC;
   printf("\nPercorso trovato!\nTempo = %lf (s)\n",cpu_time_used);

   return 1;
 }
