#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char byte;

void EncryptAES128(const byte *msg, const byte *key, byte *c);

void DecryptAES128(const byte *c, const byte *key, byte *m);
