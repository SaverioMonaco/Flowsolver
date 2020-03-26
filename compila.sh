#!/bin/bash

cd main

gcc ../include/var/var.c -c
gcc ../include/flow/flow.c  -c -lm -Wall --pedantic
gcc ../include/flow_grafica/flow_grafica.c  -c -Wall --pedantic
gcc ../include/flow_strutture/flow_strutture.c  -c -Wall --pedantic
gcc ../include/flow_app/flow_app.c  -c -Wall --pedantic
gcc ../include/flow_game/flow_game.c  -c -Wall --pedantic

gcc main.c var.o flow.o flow_grafica.o flow_strutture.o flow_game.o flow_app.o -o ./flow -Wall --pedantic -lm
./flow
