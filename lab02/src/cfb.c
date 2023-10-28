#include "cfb.h"

static uint64_t IV = 0x0101010101010101;
static uint64_t key = 0x1010101010101010;

void setIV(uint64_t newIV) {
    IV = newIV;
}

void set_key(uint64_t new_key) {
    key = new_key;
}

uint64_t getIV(void) {
    return IV;
}

uint64_t get_key(void) {
    return key;
}

uint64_t cfb(uint64_t input, char mode) {
    uint64_t result = des(IV, key, 'e');
    if (mode == 'e')
    {
        IV = result ^ input;
        return IV;
    }
    else
    {
        IV = input;
        return result ^ input;
    }
}
