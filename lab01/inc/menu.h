#pragma once

#define CYPHER 1
#define SHOW_CONGIFURATION 2
#define SET_ROTORS_STATES 3
#define SET_ROTORS 4
#define SET_REFLECTOR 5
#define SET_COMMUTATORS 6
#define BACKUP_COMMUTATORS 7
#define TEST 8
#define EXIT 0

void menu(void);

int is_letter(char letter);

#include "constants.h"
