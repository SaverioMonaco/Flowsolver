#ifndef _MY_DEFINES
#define _MY_DEFINES

/* Strutture dati generiche */
typedef void*          pvoid;
typedef pvoid*         ppvoid;

typedef int*           pint;
typedef char*		       pchar;
typedef pchar*		     ppchar;

typedef int            my_int;
typedef my_int*        pmy_int;

typedef unsigned int   my_uint;
typedef my_uint*       pmy_uint;

typedef float          my_float;
typedef my_float*      pmy_float;

typedef double         my_double;
typedef my_double*     pmy_double;

typedef FILE*          pfile;


/* Il contorno nell'array della griglia viene salvato col valore sottostante */
#define CONTORNO -1
/* Macro per muoversi meglio tra le celle del puntatore della griglia */
#define pos(x,y) (x)+((riga_flow))*(y)

/* Per quando si debugga e si vuole far aspettare la macchina per alcune
   operazioni*/
#define WAITTIME 800000

/* Usati per il gioco di flow da terminale */
#define UP printf("\033[1A") // Move up X lines;
#define DOWN printf("\033[1B") // Move down X lines;
#define RIGHT printf("\033[2C") // Move right X column;
#define LEFT printf("\033[2D") // Move left X column;

/* Non usati */
#define my_clear() printf("\033[H\033[J")
#define printgoto(x,y) printf("\033[%d;%dH",x,y)

#endif
