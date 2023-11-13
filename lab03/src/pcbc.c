#include "pcbc.h"

static byte IV[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
static byte key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

void setIV(void)
{
    memcpy(IV, key, sizeof(byte) *  16);
}

void pcbc(byte input128[], byte output128[], byte mode) {
    if (mode == 'e')
    {
        byte to_cypher[16] = {0};
        for (int i = 0; i < 16; i++)
            to_cypher[i] = IV[i] ^ input128[i];

        EncryptAES128(to_cypher, key, output128);
        for (int i = 0; i < 16; i++)
            IV[i] = input128[i] ^ output128[i];
    }
    else
    {
        byte almost_decyphered[16] = {0};
        DecryptAES128(input128, key, almost_decyphered);
        for (int i = 0; i < 16; i++)
        {
            output128[i] = IV[i] ^ almost_decyphered[i];
            IV[i] = input128[i] ^ output128[i];
        }
    }
}
