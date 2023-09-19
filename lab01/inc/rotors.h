#pragma once

#include "constants.h"

const char rotors[ROTORS_AMOUNT][ALPHABET_LENGTH];

const char reflectors[REFLECTORS_AMOUNT][ALPHABET_LENGTH];

char alphabet[ALPHABET_LENGTH];
char commutation[ALPHABET_LENGTH];
char commutation_state[ALPHABET_LENGTH];
char rotors_positions[CHOSEN_ROTORS];
int chosen_rotors[CHOSEN_ROTORS];
int chosen_reflector;
