#pragma once

#include <inttypes.h>

#define ENCRYPT 1
#define DECRYPT 2
#define USE_MD5 3
#define SIGN 4
#define CHECK_SIGN 5
#define INIT 6
#define TEST 7
#define EXIT 0

#define MAX_FILENAME_LEN 50
#define OUTPUT_FILE "encrypted"
#define SIGNATURE "signature"
#define TEST_TIMES 100
#define ZEROS "zeros.txt"

void menu(void);

int is_letter(char letter);
