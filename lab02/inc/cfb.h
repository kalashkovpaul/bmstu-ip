#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "des.h"

void setIV(uint64_t newIV);

void set_key(uint64_t new_key);

uint64_t getIV(void);

uint64_t get_key(void);

uint64_t cfb(uint64_t input, char mode);
