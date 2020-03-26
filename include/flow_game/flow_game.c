#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../var/var.hidden"
#include "../my_defines.h"
#include "./flow_game.h"
#include "../flow_strutture/flow_strutture.h"
#include "../flow_app/flow_app.h"
#include "../flow_grafica/flow_grafica.h"
#include "../flow/flow.h"

/* La libreria flow_game funziona nel seguente modo:
   > tramite flow_game_start(int b, int h, int ncoppie) si inizializza una
     griglia bxh e si posizionano casualmente ncoppie coppie su tale griglia.
   > mediante la funzione flow_game_verificasoluzione() (inclusa nella funzione
     flow_game_start si cerca di trovare UNA soluzione.
   > Se esiste una soluzione, viene dato il puzzle all'utente per essere risolto.
     Se non esiste una soluzione, si rigenerano delle coppie casuali e si vede
     se per loro ne esiste una.
   > Da terminale l'utente può risolvere il percorso, quando immette una
     soluzione flow_game_soluzioneumano() la verifica (mediante un algoritmo
     simile a quello della ricerca delle soluzioni classiche).
     Se è una soluzione la funzione ritorna 1, se la soluzione è errata ritorna
     0. */

int flow_game_trovapercorso(int cursore);

int flow_game_verificasoluzione()
 {

   cursorecoppia+=2;

   /* Se abbiamo finito i tubi allora in questo punto il cursore delle pcoppie
      è maggiore delle coppie x 2 (ogni coppia è caratterizzata da due numeri) */
   if(cursorecoppia>(*(pcoppie)*2))
     {
       return 1;
     }

   flow_struct_successivostruttureausiliarie();

   if(flow_game_trovapercorso(*(pcoppie+cursorecoppia)-riga_flow)) {return 1;}
   if(flow_game_trovapercorso(*(pcoppie+cursorecoppia)-1)) {return 1;}
   if(flow_game_trovapercorso(*(pcoppie+cursorecoppia)+riga_flow)) {return 1;}
   if(flow_game_trovapercorso(*(pcoppie+cursorecoppia)+1)) {return 1;}

   flow_struct_precedentestruttureausiliarie();

   cursorecoppia-=2;

   return 0;
 }

int flow_game_trovapercorso(int cursore)
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

        /*DEBUG*/
        /*flow_grafica_stampagrigliacolori();
          flow_grafica_stampastruttureausiliarie();
          usleep(WAITTIME);*/

        /* L'ordine di percorrenza della griglia è:
           SU SINISTRA GIU' DESTRA     */
        if(flow_game_trovapercorso(cursore-riga_flow)) {return 1;}
        if(flow_game_trovapercorso(cursore-1)) {return 1;}
        if(flow_game_trovapercorso(cursore+riga_flow)) {return 1;}
        if(flow_game_trovapercorso(cursore+1)) {return 1;}

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
       if(flow_game_verificasoluzione()){return 1;}
     }

   return 0;

 }

int flow_game_coppierand(int numerocoppie)
 {
   int x,y,i,j;

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

int flow_game_search()
 {
   /* Check sulle inizializzazioni delle strutture ausiliarie */
   if(!flow_struct_inizializzastruttureausiliarie())
     {
       return 0;
     }

   cursorecoppia=-1;

   /* La funzione ricorsiva parte da qui */
   if(!flow_game_verificasoluzione())
     {
       return 0;
     }

   return 1;

 }

int flow_game_trovapercorsoumano(int cursore)
 {
   /*printf("[%d,%d] -- (%d,%d)",cursore%riga_flow,cursore/(riga_flow),*(pgrid+cursore),*(pcoppie+(cursorecoppia+1)));*/
   if((*(pgrid+(cursore)))==(cursorecoppia+1))
     {
       return 1;
     }

   /*printf("%d=%d?",*(pgrid+cursore),(*(pcoppie+cursorecoppia)));*/
   if( *(pgrid+cursore)==(cursorecoppia)  )
     {
       flow_struct_modificacellasovrastante(-1,cursore);

       if(flow_game_trovapercorsoumano((cursore+riga_flow))){return 1;}
       if(flow_game_trovapercorsoumano((cursore-1))){return 1;}
       if(flow_game_trovapercorsoumano((cursore-riga_flow))){return 1;}
       if(flow_game_trovapercorsoumano((cursore+1))){return 1;}

       flow_struct_modificacellasovrastante(cursorecoppia,cursore);
     }


   return 0;
 }

int flow_game_nuovacoppiapercorsoumano()
 {
   cursorecoppia+=2;

   /*printf("%d<",*(pcoppie+(cursorecoppia)));*/
   if(flow_game_trovapercorsoumano(*(pcoppie+cursorecoppia)+riga_flow)){return 1;}
   if(flow_game_trovapercorsoumano(*(pcoppie+cursorecoppia)-1)){return 1;}
   if(flow_game_trovapercorsoumano(*(pcoppie+cursorecoppia)-riga_flow)){return 1;}
   if(flow_game_trovapercorsoumano(*(pcoppie+cursorecoppia)+1)){return 1;}

   return 0;
 }

int flow_game_soluzioneumano()
 {
   int i=1;
   cursorecoppia=-1;

   for(;i<=(*pcoppie);i++)
     {
       if(!flow_game_nuovacoppiapercorsoumano()){return 0;}
     }

   return 1;
 }

void flow_game_start(int b,int h,int ncoppie)
 {
   if(ncoppie>7){printf("Numero di coppie troppo alto! (<8)\n"); return;}

   int i,x,y,esc,colore,temp;

   colore=91;

   srand(time(NULL));

   do
     {
       flow_inizializzagriglia(b,h);
       flow_game_coppierand(ncoppie);
     } while(!flow_game_search());

   flow_inizializzagriglia(b,h);

   for(i=1;i<=numerocoppiexdue;i++)
     {
       /*printf("%d\n",*(pcoppie+i));*/
       flow_struct_modificacellasovrastante(i,(*(pcoppie+i)));

     }
   printf("\n");
   flow_grafica_stampagrigliacolori();
   printf(" +--+\n |");printf("\x1b[%dm",colore);printf("██");printf("\x1b[0m");printf("|\n +--+");
   printf("\n+--------------------------------------------------------------------+\n");
   printf("ISTRUZIONI:\nWASD per muoversi\nSPACE per scrivere, cancellare e cambiare colore (se usato su un estremo)\nK per uscire\nP per verificare la soluzione\n");
   printf("+----------------------------------------------------------------------+\n");

   x=0; y=0; esc=0;
   char input;
   system ("/bin/stty raw");

   RIGHT;
   for(i=0;i<=altezza_flow+10;i++){UP;}

   while(!esc)
     {
       temp=0;
       //printf("%d",colore);
       input=getchar();
       printf("\b");
       //printf("%d",colore);
       if(*(pgrid+pos(x,y))==0){printf("\x1b[0m");printf("  ");LEFT;}
       else
         {
           printf("\x1b[%dm",(90+(*(pgrid+pos(x,y))+1)/2));printf("██");LEFT;
           for(i=1;i<=numerocoppiexdue;i++)
             {
               if(pos(x,y)==(*(pcoppie+i)))
                 {
                   printf("\x1b[%dm",(30+(*(pgrid+pos(x,y))+1)/2));
                   printf("██");LEFT;
                 }
             }
         }

       switch(input)
         {

           case 'p':
             printf("\x1b[0m");
             if(flow_game_soluzioneumano())
               {
                 for(i=0;i<altezza_flow+13-y;i++){DOWN;}
                 for(i=0;i<x+2;i++){LEFT;}
                 printf("Bravo!\n");

                 for(i=0;i<altezza_flow+13-y;i++){UP;}
                 esc++;
                 break;
               }

             else
               {
                 for(i=0;i<=y;i++){UP;}
                 for(i=0;i<=x;i++){LEFT;}
                 flow_inizializzagriglia(b,h);

                 for(i=1;i<=numerocoppiexdue;i++)
                   {
                     flow_struct_modificacellasovrastante(i,(*(pcoppie+i)));
                   }
                 system ("/bin/stty cooked");
                 flow_grafica_stampagrigliacolori();
                 for(i=0;i<=altezza_flow;i++){UP;}
                 RIGHT;
                 system ("/bin/stty raw");
                 x=0; y=0;

               }

           case 'w':
             if((y<altezza_flow)&&(y>0)){y--;UP;printf("\x1b[%dm",colore);}
             break;

           case 's':
             if((y<altezza_flow-1)&&(y>=0)){y++;DOWN;printf("\x1b[%dm",colore);}
             break;

            case 'a':
             if((x<=(riga_flow-2))&&(x>0)){x--;LEFT;printf("\x1b[%dm",colore);}
             break;

            case 'd':
             if((x<(riga_flow-2))&&(x>=0)){x++;RIGHT;printf("\x1b[%dm",colore);}
             break;

            case 'k':
             printf("\x1b[0m");
             esc++;
             break;

            /*case 'z':
              if(colore<ncoppie){colore++;}
             break;

            case 'x':
              if(colore>0)
                {colore--;}
             break;*/

           case ' ':
             if(*(pgrid+pos(x,y))==0)
               {
                 flow_struct_modificacellasovrastante(2*(colore-90)-1,pos(x,y));
                 /*printf("%d",*(pgrid+pos(x,y)));*/
                 printf("\x1b[%dm",colore);printf("██");LEFT;
               }

             else
               {
                 for(i=1;i<=numerocoppiexdue;i++)
                   {
                     if(pos(x,y)==(*(pcoppie+i)))
                       {
                         temp++;
                       }
                   }

                 if(!temp){printf("  ");LEFT;flow_struct_modificacellasovrastante(0,pos(x,y));}
                 else
                   {
                     colore=(90+(*(pgrid+pos(x,y))+1)/2);
                     for(i=0;i<x;i++){LEFT;}
                     for(i=0;i<altezza_flow-y+2;i++){DOWN;}
                     printf("\x1b[%dm",colore);printf("██");LEFT;
                     for(i=0;i<x;i++){RIGHT;}
                     for(i=0;i<altezza_flow-y+2;i++){UP;}

                   }

               }

         }
     }

   for(i=0;i<altezza_flow+13-y;i++){DOWN;}
   for(i=0;i<x+2;i++){LEFT;}
   system ("/bin/stty cooked");
   //system ("clear");
  }
