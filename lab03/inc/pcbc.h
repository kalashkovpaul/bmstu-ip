#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "aes.h"

void setIV(void);

void pcbc(byte input128[], byte output128[], byte mode);
