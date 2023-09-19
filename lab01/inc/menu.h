#pragma once

#define CYPHER 1
#define DECYPHER 2
#define SHOW_CONGIFURATION 3
#define SET_ROTORS_STATES 4
#define SET_ROTORS 5
#define SET_REFLECTOR 6
#define SET_COMMUTATORS 7
#define BACKUP_COMMUTATORS 8
#define EXIT 0

void menu(void);

int is_letter(char letter);

#include "constants.h"
