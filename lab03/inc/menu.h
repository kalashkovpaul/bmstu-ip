#pragma once

#include <inttypes.h>

#define CYPHER 1
#define DECYPHER 2
#define SET_IV 3
#define TEST 4
#define EXIT 0

#define MAX_FILENAME_LEN 50
#define OUTPUT_FILE "encrypted"
#define TEST_TIMES 100

void menu(void);

int is_letter(char letter);
