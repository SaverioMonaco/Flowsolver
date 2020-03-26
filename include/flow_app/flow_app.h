#ifndef _FLOW_APP_H
#define _FLOW_APP_H
#include "../my_defines.h"

/* Imposta la griglia per essere risolta da flow_risolutoreprimopercorso();
   Legge il file levelpack_app/levelpack_'nfile'.txt e imposta il puzzle lvl-esimo */
int flow_app_risolutorelivello(int nfile, int lvl);


#endif
