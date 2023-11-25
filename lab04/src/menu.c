#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "menu.h"
#include "rsa.h"
#include "md5.h"

void printChoice(void);
void show_configuration(void);
void rsa_encrypt_file(void);
void rsa_decrypt_file(void);

void use_md5(void);
void sign_file(void);
void check_signature(void);
void test_sign(void);

void menu(void) {
    int choice = -1;
    int check = 0;
    while (choice != EXIT) {
        printChoice();
        check = scanf("%d", &choice);
        fflush(stdin);
        if (check != 1) continue;

        switch (choice)
        {
        case ENCRYPT:
            rsa_encrypt_file();
            break;

        case DECRYPT:
            rsa_decrypt_file();
            break;

        case USE_MD5:
            use_md5();
            break;

        case SIGN:
            sign_file();
            break;

        case CHECK_SIGN:
            check_signature();
            break;

        case INIT:
            init();
            break;

        case TEST:
            test_sign();
            break;

        case EXIT:
            printf("Спасибо, что пользовались RSA (MD5) от Калашкова П. :)\n");
            break;

        default:
            printf("К сожалению, такого пункта меню нет. Попробуйте ещё раз!\n");
            break;
        }
    }
}

void printChoice(void) {
    printf("Выберите один из пунктов:\n");
    printf("1. Зашифровать файл (RSA)\n");
    printf("2. Расшифровать файл (RSA)\n");
    printf("3. Захешировать (MD5)\n");
    printf("4. Подписать файл\n");
    printf("5. Проверить подпись файла\n");
    printf("6. Установить RSA\n");
    printf("7. Протестировать работу программы\n");
    printf("\n");
    printf("0. Выйти из меню\n");
    printf("\n");
    printf("Выберите пункт: ");
}

void rsa_encrypt_file(void)
{
    char filename[MAX_FILENAME_LEN] = {0};
    printf("Введите название файла. После ввода нажмите Enter\n");
    printf("Имя файла: ");
    scanf("%s", filename);
    FILE* input = fopen(filename, "rb");
    if (!input)
    {
        printf("Что-то не так с файлом %s\n\n", filename);
        return;
    }
    FILE* output = fopen(OUTPUT_FILE, "wb");
    if (!output)
    {
        printf("Что-то не так с файлом%s\n\n", OUTPUT_FILE);
        return;
    }

    fseek(input, 0L, SEEK_END);
    size_t sz = 0;
    sz = ftell(input);
    fseek(input, 0L, SEEK_SET);
    char *to_encrypt = calloc(sz, sizeof(char));
    fread(to_encrypt, sizeof(char), sz, input);

    char *encrypted = calloc(8 * sz, sizeof(char));
    size_t len = encrypt(encrypted, to_encrypt, sz, get_kp());
    fwrite(encrypted, len, sizeof(char), output);
    free(to_encrypt);
    free(encrypted);
    fclose(input);
    fclose(output);
}

size_t get_lines(FILE* file)
{
    size_t sz = 0;
    char ch = '0';
    while(!feof(file))
    {
        ch = fgetc(file);
        if(ch == '\n')
        {
            sz++;
        }
    }
    rewind(file);
    return sz;
}

void rsa_decrypt_file(void)
{
    char filename[MAX_FILENAME_LEN] = {0};
    printf("Введите название файла (куда расшифровать). После ввода нажмите Enter\n");
    printf("Имя файла: ");
    scanf("%s", filename);
    FILE* output = fopen(filename, "wb");
    if (!output)
    {
        printf("Что-то не так с файлом %s\n\n", filename);
        return;
    }
    FILE* input = fopen(OUTPUT_FILE, "rb");
    if (!input)
    {
        printf("Что-то не так с файлом%s\n\n", OUTPUT_FILE);
        return;
    }

    fseek(input, 0L, SEEK_END);
    size_t sz = 0;
    sz = ftell(input);
    fseek(input, 0L, SEEK_SET);
    char *encoded = calloc(sz, sizeof(char));
    char *decoded = calloc(sz, sizeof(char));
    fread(encoded, sizeof(char), sz, input);
    int len = decrypt(decoded, encoded, sz, get_ku());
    fwrite(decoded, len, sizeof(char), output);
    free(encoded);
    free(decoded);
    fclose(input);
    fclose(output);
}

void use_md5(void)
{
    char value_to_hash[1024] = {0};
    printf("Введите строку, которую хешируем: ");
    scanf("%s", value_to_hash);

    uint8_t result[16];
    md5((uint8_t*)value_to_hash, strlen(value_to_hash) + 2, result);
    printf("Хеш MD5:\n");
    for (int i = 0; i < 16; i++)
        printf("%2.2x", result[i]);
    printf("\n");
}

void sign_file(void)
{
    char filename[MAX_FILENAME_LEN] = {0};
    printf("Введите название файла. После ввода нажмите Enter\n");
    printf("Имя файла: ");
    scanf("%s", filename);
    FILE* input = fopen(filename, "rb");
    if (!input)
    {
        printf("Что-то не так с файлом %s\n\n", filename);
        return;
    }
    FILE* output = fopen(SIGNATURE, "wb");
    if (!output)
    {
        printf("Что-то не так с файлом%s\n\n", SIGNATURE);
        return;
    }
    fseek(input, 0L, SEEK_END);
    size_t sz = 0;
    sz = ftell(input);
    fseek(input, 0L, SEEK_SET);
    char *to_sign = calloc(sz, sizeof(char));
    fread(to_sign, sizeof(char), sz, input);

    uint8_t result[16];
    md5((uint8_t*)to_sign, sizeof(char) * sz, result);

    char *encrypted = calloc(8 * 16 * sizeof(uint8_t), sizeof(char));
    size_t len = encrypt(encrypted, (char*)result, 16 * sizeof(uint8_t), get_kp());
    fwrite(encrypted, len, sizeof(char), output);

    free(encrypted);
    fclose(input);
    fclose(output);
}

void check_signature(void)
{
    printf("Введите название файла. После ввода нажмите Enter\n");
    printf("Имя файла: ");
    char filename[MAX_FILENAME_LEN] = {0};
    scanf("%s", filename);
    FILE* input = fopen(filename, "rb");
    if (!input)
    {
        printf("Что-то не так с файлом %s\n\n", filename);
        return;
    }
    FILE* signature = fopen(SIGNATURE, "rb");
    if (!signature)
    {
        printf("Что-то не так с файлом%s\n\n", SIGNATURE);
        return;
    }

    fseek(input, 0L, SEEK_END);
    size_t sz = 0;
    sz = ftell(input);
    fseek(input, 0L, SEEK_SET);
    char *to_sign = calloc(sz + 1, sizeof(char));
    fread(to_sign, sizeof(char), sz, input);

    uint8_t result[16];
    md5((uint8_t*)to_sign, sizeof(char) * sz, result);

    printf("Файл:\n");
    for (int i = 0; i < 16; i++)
        printf("%2.2x", result[i]);
    printf("\n");


    fseek(signature, 0L, SEEK_END);
    size_t s = 0;
    s = ftell(signature);
    fseek(signature, 0L, SEEK_SET);
    char *encrypted = calloc(s, sizeof(char));
    fread(encrypted, sizeof(char), s, signature);

    uint8_t *decrypted = calloc(16, sizeof(uint8_t));
    decrypt((char*)decrypted, encrypted, s, get_ku());

    printf("Подпись:\n");
    for (int i = 0; i < 16; i++)
        printf("%2.2x", decrypted[i]);
    printf("\n");


    int match = 1;
    for (int i = 0; i < 16; i++)
    {
        if (result[i] != decrypted[i])
        {
            match = 0;
        }
    }
    if (match)
        printf("Ура! Подписи совпадают!\n");
    else
        printf("О нет! Подписи не совпадают!\n");
    free(encrypted);
    free(decrypted);
    fclose(input);
    fclose(signature);
}

void test_sign(void)
{
    char *to_encrypt = calloc(TEST_TIMES, sizeof(char));
    char *encrypted = calloc(TEST_TIMES*8, sizeof(char));
    char *decrypted = calloc(TEST_TIMES , sizeof(char));

    for (int i = 0; i < TEST_TIMES; i++)
    {
        for (int j = 0; j < TEST_TIMES; j++)
        {
            to_encrypt[j] = rand() % 256 - 128;
            if (to_encrypt[j] == 0) to_encrypt[j] = 1;
        }
        encrypt(encrypted, to_encrypt, TEST_TIMES, get_kp());
        decrypt(decrypted, encrypted, TEST_TIMES * 8, get_ku());

        for (int j = 0; j < TEST_TIMES; j++)
        {
            if (to_encrypt[j] != decrypted[j])
            {
                printf("Было:");
                for (int k = 0; k < TEST_TIMES; k++)
                {
                    printf("%d", to_encrypt[k]);
                }
                printf("\nСтало: ");
                for (int k = 0; k < TEST_TIMES; k++)
                {
                    printf("%d", decrypted[k]);
                }
                free(to_encrypt);
                free(encrypted);
                free(decrypted);
                printf("Тест №%i упаль...\n", i);
                return;
            }
        }
        memset(to_encrypt, 0, TEST_TIMES * sizeof(char));
        memset(to_encrypt, 0, 8 * TEST_TIMES * sizeof(char));
        memset(encrypted, 0, TEST_TIMES * sizeof(char));
        printf("Тест №%d успешен!\n", i);
    }
    free(to_encrypt);
    free(encrypted);
    free(decrypted);
}

