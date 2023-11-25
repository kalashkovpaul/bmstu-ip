#ifndef __RSA_H__
#define __RSA_H__

#include <gmp.h>

void init(void);

#define MODULUS_SIZE 1024                   /*  Modulus bits */
#define BLOCK_SIZE (MODULUS_SIZE/8)         /* en/decrypted block size*/
#define BUFFER_SIZE ((MODULUS_SIZE/8) / 2)  /*n and p bytes*/

typedef struct {
    mpz_t n;
    mpz_t e;
} public_key;

typedef struct {
    mpz_t n;
    mpz_t e;
    mpz_t d;
    mpz_t p;
    mpz_t q;
} private_key;


public_key get_kp(void);
private_key get_ku(void);
void block_encrypt(mpz_t C, mpz_t M, public_key kp);
void block_decrypt(mpz_t M, mpz_t C, private_key ku);
int encrypt(char cipher[], char message[], int length, public_key kp);
int decrypt(char* message, char* cipher, int length, private_key ku);

#endif
