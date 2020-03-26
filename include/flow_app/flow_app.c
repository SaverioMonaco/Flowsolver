#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../var/var.hidden"
#include "../my_defines.h"
#include "./flow_app.h"
#include "../flow/flow.h"
#include "../flow_strutture/flow_strutture.h"

/*
   I file originali dell'app contenenti i livelli sono salvati in file di testo
   (levelpack_4.txt ad esempio), ogni file contiente 150 stringhe, corrispondenti
   a 150 livelli, ad eccezzione di
   levelpack_0.txt che ne contiene 300,
   levelpack_11.txt che ne contiene 120 e
   levelpack_12.txt che ne contiene 120,

   Ciascuna stringa è fatta da interi, intervallati da virgole e punti e virgola
   col seguente senso:

   LATO DELLA GRIGLIA, * , * , NUMERO DI COPPIE ;
   POSIZIONE PUNTO 1 COPPIA 1, * , * ... * , POSIZIONE PUNTO 2 COPPIA 1;
   POSIZIONE PUNTO 1 COPPIA 2, * , * ... * , POSIZIONE PUNTO 2 COPPIA 2;...

   (Tra le coppie ci sono degli interi che rappresentano il percorso giusto da
    fare, infatti nell'app di flow i livelli sono tali che esiste solo un
    percorso ).

*/


/* Funzione che serve per la lettura dei file di testo dei livelli dell'app */
int flow_app_getintfromstring(pchar buffer, char inizio, char fine, pint pindex)
 {
   int i,j,k,number;

   i=(*(pindex));
   while(buffer[i]!=fine)
     {
       i++;
     }

   (*(pindex))=i;

   j=0;

   while(buffer[i]!=inizio)
     {
       j++;
       i--;
     }

   j--;


   number=0;
   k=j-1;

   while(k>=0)
     {
       number+=((buffer[i+1]-'0')*pow(10,k));
       /*printf("%d\n",number);*/

       k--;
       i++;
     }

   return number;
 }

/* Funzione che serve per la lettura dei file di testo dei livelli dell'app */
int flow_app_getfinalintfromstring(pchar buffer, char inizio, char fine, pint pindex)
 {
   int i,j,k,number;

   i=(*(pindex));
   while(buffer[i]!=fine)
     {
       i++;
     }

   (*(pindex))=i;

   j=0;
   while(buffer[i]!=inizio)
     {
       j++;
       i--;
     }

   j--;

   number=0;
   k=j-3;

   while(k>=0)
     {
       number+=((buffer[i+1]-'0')*pow(10,k));

       /*printf("%d\n",number);*/
       k--;
       i++;
     }

   return number;
 }

int flow_app_risolutorelivello(int nfile, int lvl)
 {
   pfile f;
   int line,num,i;

   /* Mi serve un indice per la lettura delle stringhe, in modo da evitare
      riletture quando chiamo flow_app_getintfromstring */
   int pindex[1];

   /* Qui salviamo tutta la stringa */
   char buffer[512];

   /* Cerchiamo di contemplare tutti i casi di un immissione sbagliata di
      numerofile e/o livello */
   if((lvl<0)||(nfile<0)||(nfile>22)){printf("Il livello non esiste!\n"); return 0;}
   switch(nfile)
     {
       case 0:
         if(lvl>300){printf("Il livello non esiste!\n"); return 0;}
         break;
       case 11:
         if(lvl>120){printf("Il livello non esiste!\n"); return 0;}
         break;
       case 12:
         if(lvl>120){printf("Il livello non esiste!\n"); return 0;}
         break;
       default:
         if(lvl>150){printf("Il livello non esiste!\n"); return 0;}
         break;
    }
   snprintf(buffer, sizeof(char)*512, "../include/flow_app/levelpack_app/levelpack_%i.txt", nfile);
   if ((f=fopen(buffer,"r"))==NULL){printf("Errore file!\n"); return 0;}

   /* Leggiamo il file di testo fino ad arrivare alla linea (quindi livello)
      giusta */
   line=0;
   while( fgets(buffer,sizeof(buffer),f) )
     {
       line++;
       if(line==lvl)
         {
           /*printf("%s",buffer);*/
           fclose(f);
         }
     }

   *(pindex)=0;

   altezza_flow=flow_app_getintfromstring(buffer,buffer[-1],',',pindex);

   riga_flow=altezza_flow+1;

   /* Le griglie sono sempre quadrate*/
   flow_inizializzagriglia((riga_flow-1),altezza_flow);

   num=flow_app_getintfromstring(buffer,',',';',pindex);
   /*printf("\n%d\n",num);*/

   if((pcoppie=(pint)malloc(sizeof(int)*(2*num+1)))==NULL)
     {
       printf("\n !! Errore di memoria: impossibile inizializzare l'array delle coppie !!\n");

       return 0;
     }

   *(pcoppie)=num;

   /* Leggiamo dalla stringa le varie posizioni dei punti, quindi le scriviamo
      nell'array delle coppie */
   for(i=1;i<(2*(*(pcoppie))-1);)
     {
       num=flow_app_getintfromstring(buffer,';',',',pindex);

       /* Nel file di testo le posizioni dei punti sono salvate in maniera simile
          però non viene considerato il contorno, quindi bisogna aggiungere cose
          al valore letto */
       *(pcoppie+i)=(num+num/(riga_flow-1));
       i++;
       num=flow_app_getintfromstring(buffer,',',';',pindex);
       *(pcoppie+i)=(num+num/(riga_flow-1));
       i++;
     }

   /* Facciamo a parte le ultime due poiché l'ultimo punto non ha come
      char finale ';' ma '\0' */
   num=flow_app_getintfromstring(buffer,';',',',pindex);
   *(pcoppie+i)=(num+num/(riga_flow-1));
   i++;

   num=flow_app_getfinalintfromstring(buffer,',','\0',pindex);
   *(pcoppie+i)=(num+num/(riga_flow-1));

   /*for(i=1;i<(2*(*(pcoppie))+1);i++){printf("\n>%d",*(pcoppie+i));}*/

   for(i=1;i<(2*(*(pcoppie))+1);i++)
     {
       /*printf("%d\n",*(pcoppie+i));*/
       flow_struct_modificacellasovrastante(i,(*(pcoppie+i)));
     }

   flow_struct_inizializzastruttureausiliarie();

   numerocoppiexdue=(*(pcoppie)*2);
   flow_risolutoreprimopercorso();

   return 1;
 }
