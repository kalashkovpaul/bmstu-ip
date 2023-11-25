#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "rsa.h"

void print_hex(char* arr, int len)
{
    int i;
    for(i = 0; i < len; i++)
        printf("%02x", (unsigned char) arr[i]);
}

void generate_keys(private_key* ku, public_key* kp)
{
    char buf[BUFFER_SIZE];
    int i;
    mpz_t phi; mpz_init(phi);
    mpz_t tmp1; mpz_init(tmp1);
    mpz_t tmp2; mpz_init(tmp2);

    mpz_set_ui(ku->e, 3);

    for(i = 0; i < BUFFER_SIZE; i++)
        buf[i] = rand() % 0xFF;
    buf[0] |= 0xC0;
    buf[BUFFER_SIZE - 1] |= 0x01;
    mpz_import(tmp1, BUFFER_SIZE, 1, sizeof(buf[0]), 0, 0, buf);
    mpz_nextprime(ku->p, tmp1);
    mpz_mod(tmp2, ku->p, ku->e);
    while(!mpz_cmp_ui(tmp2, 1))
    {
        mpz_nextprime(ku->p, ku->p);   
        mpz_mod(tmp2, ku->p, ku->e);
    }

    do {
        for(i = 0; i < BUFFER_SIZE; i++)
            buf[i] = rand() % 0xFF;
        buf[0] |= 0xC0;
        buf[BUFFER_SIZE - 1] |= 0x01;
        mpz_import(tmp1, (BUFFER_SIZE), 1, sizeof(buf[0]), 0, 0, buf);
        mpz_nextprime(ku->q, tmp1);
        mpz_mod(tmp2, ku->q, ku->e);
        while(!mpz_cmp_ui(tmp2, 1))
        {
            mpz_nextprime(ku->q, ku->q);
            mpz_mod(tmp2, ku->q, ku->e);
        }
    } while(mpz_cmp(ku->p, ku->q) == 0);

    mpz_mul(ku->n, ku->p, ku->q);

    mpz_sub_ui(tmp1, ku->p, 1);
    mpz_sub_ui(tmp2, ku->q, 1);
    mpz_mul(phi, tmp1, tmp2);

    if(mpz_invert(ku->d, ku->e, phi) == 0)
    {
        mpz_gcd(tmp1, ku->e, phi);
        printf("gcd(e, phi) = [%s]\n", mpz_get_str(NULL, 16, tmp1));
        printf("Invert failed\n");
    }

    mpz_set(kp->e, ku->e);
    mpz_set(kp->n, ku->n);

    return;
}

void block_encrypt(mpz_t C, mpz_t M, public_key kp)
{
    mpz_powm(C, M, kp.e, kp.n);
    return;
}

int encrypt(char cipher[], char message[], int length, public_key kp)
{
    int block_count = 0;
    int prog = length;
    char mess_block[BLOCK_SIZE];
    mpz_t m; mpz_init(m);
    mpz_t c; mpz_init(c);

    size_t total = 0;
    size_t cur = 0;

    while(prog > 0)
    {
        int i = 0;
        int d_len = (prog >= (BLOCK_SIZE - 11)) ? BLOCK_SIZE - 11 : prog;

        mess_block[i++] = 0x00;
        mess_block[i++] = 0x02;
        while(i < (BLOCK_SIZE - d_len - 1))
            mess_block[i++] = (rand() % (0xFF - 1)) + 1;
        mess_block[i++] = 0x00;

        memcpy(mess_block + i, message + (length - prog), d_len);

        mpz_import(m, BLOCK_SIZE, 1, sizeof(mess_block[0]), 0, 0, mess_block);
        block_encrypt(c, m, kp);

        int off = block_count * BLOCK_SIZE;
        off += (BLOCK_SIZE - (mpz_sizeinbase(c, 2) + 8 - 1)/8);

        cur = 0;
        mpz_export(cipher + off, &cur, 1, sizeof(char), 0, 0, c);
        total += cur;

        block_count++;
        prog -= d_len;
    }
    return total;
}

void block_decrypt(mpz_t M, mpz_t C, private_key ku)
{
    mpz_powm(M, C, ku.d, ku.n);
    return;
}

int decrypt(char* message, char* cipher, int length, private_key ku)
{
    int msg_idx = 0;
    char buf[BLOCK_SIZE];
    *(long long*)buf = 0ll;
    mpz_t c; mpz_init(c);
    mpz_t m; mpz_init(m);

    int i;
    for(i = 0; i < (length / BLOCK_SIZE); i++)
    {
        mpz_import(c, BLOCK_SIZE, 1, sizeof(char), 0, 0, cipher + i * BLOCK_SIZE);
        block_decrypt(m, c, ku);

        int off = (BLOCK_SIZE - (mpz_sizeinbase(m, 2) + 8 - 1)/8);
        mpz_export(buf + off, NULL, 1, sizeof(char), 0, 0, m);

        int j;
        for(j = 2; ((buf[j] != 0) && (j < BLOCK_SIZE)); j++);
        j++;

        memcpy(message + msg_idx, buf + j, BLOCK_SIZE - j);

        msg_idx += BLOCK_SIZE - j;
    }
    return msg_idx;
}

// mpz_t M;  mpz_init(M);
// mpz_t C;  mpz_init(C);
// mpz_t DC;  mpz_init(DC);
private_key ku;
public_key kp;

public_key get_kp(void)
{
  return kp;
}

private_key get_ku(void)
{
  return ku;
}


void init(void)
{
  mpz_init(kp.n);
  mpz_init(kp.e);
  // Initialize private key
  mpz_init(ku.n);
  mpz_init(ku.e);
  mpz_init(ku.d);
  mpz_init(ku.p);
  mpz_init(ku.q);

  generate_keys(&ku, &kp);
  // printf("---------------Private Key-----------------");
  // printf("kp.n is [%s]\n", mpz_get_str(NULL, 16, kp.n));
  // printf("kp.e is [%s]\n", mpz_get_str(NULL, 16, kp.e));
  // printf("---------------Public Key------------------");
  // printf("ku.n is [%s]\n", mpz_get_str(NULL, 16, ku.n));
  // printf("ku.e is [%s]\n", mpz_get_str(NULL, 16, ku.e));
  // printf("ku.d is [%s]\n", mpz_get_str(NULL, 16, ku.d));
  // printf("ku.p is [%s]\n", mpz_get_str(NULL, 16, ku.p));
  // printf("ku.q is [%s]\n", mpz_get_str(NULL, 16, ku.q));
}

// int main()
// {
//     int i;

//     // Initialize public key
//     mpz_init(kp.n);
//     mpz_init(kp.e);
//     // Initialize private key
//     mpz_init(ku.n);
//     mpz_init(ku.e);
//     mpz_init(ku.d);
//     mpz_init(ku.p);
//     mpz_init(ku.q);

//     generate_keys(&ku, &kp);
//     printf("---------------Private Key-----------------");
//     printf("kp.n is [%s]\n", mpz_get_str(NULL, 16, kp.n));
//     printf("kp.e is [%s]\n", mpz_get_str(NULL, 16, kp.e));
//     printf("---------------Public Key------------------");
//     printf("ku.n is [%s]\n", mpz_get_str(NULL, 16, ku.n));
//     printf("ku.e is [%s]\n", mpz_get_str(NULL, 16, ku.e));
//     printf("ku.d is [%s]\n", mpz_get_str(NULL, 16, ku.d));
//     printf("ku.p is [%s]\n", mpz_get_str(NULL, 16, ku.p));
//     printf("ku.q is [%s]\n", mpz_get_str(NULL, 16, ku.q));

//     char buf[BLOCK_SIZE];
//     for(i = 0; i < BLOCK_SIZE; i++)
//         buf[i] = rand() % 0xFF;

//     mpz_import(M, BLOCK_SIZE, 1, sizeof(buf[0]), 0, 0, buf);
//     printf("original is [%s]\n", mpz_get_str(NULL, 16, M));
//     block_encrypt(C, M, kp);
//     printf("encrypted is [%s]\n", mpz_get_str(NULL, 16, C));
//     block_decrypt(DC, C, ku);
//     printf("decrypted is [%s]\n", mpz_get_str(NULL, 16, DC));
//     return 0;
// }
