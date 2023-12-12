#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "menu.h"
#include "lzw.h"

void printChoice(void);
void compress(void);
void decompress(void);
void test_lzw(void);

void menu(void)
{
    int choice = -1;
    int check = 0;
    while (choice != EXIT)
    {
        printChoice();
        check = scanf("%d", &choice);
        fflush(stdin);
        if (check != 1)
            continue;

        switch (choice)
        {
        case COMPRESS:
            compress();
            break;

        case DECOMPRESS:
            decompress();
            break;

        case TEST:
            test_lzw();
            break;

        case EXIT:
            printf("Спасибо, что пользовались LWZ от Калашкова П. :)\n");
            break;

        default:
            printf("К сожалению, такого пункта меню нет. Попробуйте ещё раз!\n");
            break;
        }
    }
}

void printChoice(void)
{
    printf("Выберите один из пунктов:\n");
    printf("1. Сжать файл файл (LWZ)\n");
    printf("2. Разжать файл (LWZ)\n");
    printf("3. Протестировать работу программы\n");
    printf("\n");
    printf("0. Выйти из меню\n");
    printf("\n");
    printf("Выберите пункт: ");
}

void lzw_compress_file(const char *srcfile, const char *destfile)
{
    FILE *ifile = fopen(srcfile, "rb");

    if (!ifile)
    {
        fprintf(stderr, "Error: %m\n");
        return;
    }
    fseek(ifile, 0, SEEK_END);
    long slen = ftell(ifile);
    fseek(ifile, 0, SEEK_SET);

    printf("Сжимаем %zu байт.\n", (size_t)slen);
    FILE *ofile = fopen(destfile, "wb");
    if (ofile)
    {
        uint8_t *src = malloc(slen);
        if (!src)
        {
            fprintf(stderr, "ERROR: memory allocation of %ld bytes failed.\n", slen);
            exit(1);
        }
        uint8_t dest[4096];
        struct lzw_state state = {0};
        if ((fread(src, slen, 1, ifile) != 1) && (ferror(ifile) != 0))
        {
            fprintf(stderr, "fread '%s': %s", srcfile, strerror(errno));
            exit(EXIT_FAILURE);
        }

        ssize_t res, written = 0;
        while ((res = lzw_compress(&state, src, slen, dest, sizeof(dest))) > 0)
        {
            fwrite(dest, res, 1, ofile);
            written += res;
        }
        if (res == 0)
        {
            printf("Получилось, %zd байт, уменьшили на %2.02f%% (набольшая длина = %zu).\n",
                   written,
                   (1.0f - ((float)written / slen)) * 100.0f,
                   state.longest_prefix);
        }
        else if (res < 0)
        {
            fprintf(stderr, "Compression returned error: %s (err: %zd)\n", lzw_strerror(res), res);
        }
        fclose(ofile);
        free(src);
    }
    else
    {
        fprintf(stderr, "Error: %m\n");
    }
    fclose(ifile);
}

void lzw_decompress_file(const char *srcfile, const char *destfile)
{
    FILE *ifile = fopen(srcfile, "rb");

    if (!ifile)
    {
        fprintf(stderr, "Error: %m\n");
        return;
    }
    fseek(ifile, 0, SEEK_END);
    long slen = ftell(ifile);
    fseek(ifile, 0, SEEK_SET);

    if (slen > 0)
    {
        printf("Разжимаем %zu байт.\n", (size_t)slen);
        FILE *ofile = stdout;
        if (strcmp(destfile, "-") != 0)
        {
            ofile = fopen(destfile, "wb");
        }
        if (ofile)
        {
            uint8_t dest[4096];
            size_t dest_len = sizeof(dest);
            uint8_t *src = malloc(slen);
            if (!src)
            {
                fprintf(stderr, "ERROR: memory allocation of %ld bytes failed.\n", slen);
                exit(1);
            }

            if ((fread(src, slen, 1, ifile) != 1) && (ferror(ifile) != 0))
            {
                fprintf(stderr, "fread '%s': %s", srcfile, strerror(errno));
                exit(EXIT_FAILURE);
            }

            struct lzw_state state = {0};

            ssize_t res, written = 0;

            while ((res = lzw_decompress(&state, src, slen, dest, dest_len)) > 0)
            {
                fwrite(dest, res, 1, ofile);
                written += res;
            }
            if (res == 0)
            {
                printf("Получилось %zd байт, расширили на %2.2f%% (longest prefix=%zu).\n",
                       written,
                       ((float)written / slen - 1.0f) * 100.0f,
                       state.longest_prefix);
            }
            else if (res < 0)
            {
                fprintf(stderr, "Decompression returned error: %s (err: %zd)\n", lzw_strerror(res), res);
            }
            fclose(ofile);
            free(src);
        }
        else
        {
            fprintf(stderr, "Error: %m\n");
        }
    }
    fclose(ifile);
}

void compress(void)
{
    char filename[MAX_FILENAME_LEN] = {0};
    char dest_filename[MAX_FILENAME_LEN] = {0};
    printf("Введите название файла (который сжать). После ввода нажмите Enter\n");
    printf("Имя файла: ");
    scanf("%s", filename);
    printf("Введите название файла (итогового). После ввода нажмите Enter\n");
    printf("Имя файла: ");
    scanf("%s", dest_filename);
    lzw_compress_file(filename, dest_filename);
}

void decompress(void)
{
    char filename[MAX_FILENAME_LEN] = {0};
    char dest_filename[MAX_FILENAME_LEN] = {0};
    printf("Введите название файла (который разжать). После ввода нажмите Enter\n");
    printf("Имя файла: ");
    scanf("%s", filename);
    printf("Введите название файла (итогового). После ввода нажмите Enter\n");
    printf("Имя файла: ");
    scanf("%s", dest_filename);
    lzw_decompress_file(filename, dest_filename);
}

void test_lzw(void)
{
    uint8_t *to_encrypt = calloc(TEST_LEN, sizeof(uint8_t));
    uint8_t *compressed_buff = calloc(TEST_LEN, sizeof(uint8_t));
    uint8_t *compressed = calloc(TEST_LEN*8, sizeof(uint8_t));
    uint8_t *decrypted = calloc(TEST_LEN, sizeof(uint8_t));

    for (int i = 0; i < TEST_TIMES; i++)
    {
        for (int j = 0; j < TEST_LEN; j++)
        {
            to_encrypt[j] = rand() % 256 - 128;
            if (to_encrypt[j] == 0) to_encrypt[j] = 1;
        }
        struct lzw_state state = {0};
        ssize_t compressed_len = 0;
        ssize_t decompressed_len = 0;
        ssize_t res = 0;
        while ((res = lzw_compress(&state, to_encrypt, TEST_LEN, compressed_buff, TEST_LEN)) > 0)
        {
            memcpy(compressed + compressed_len, compressed_buff, res * sizeof(uint8_t));
            compressed_len += res;
        }
        memset(compressed_buff, 0, TEST_LEN * sizeof(uint8_t));
        struct lzw_state state1 = {0};
        while ((res = lzw_decompress(&state1, compressed, compressed_len, compressed_buff, sizeof(compressed_buff))) > 0)
        {
            memcpy(decrypted + decompressed_len, compressed_buff, res * sizeof(uint8_t));
            decompressed_len += res;
        }
        if (decompressed_len == 0 && res <=0)
        {
            printf("res = %d\n", (int) res);
        }

        for (int j = 0; j < TEST_LEN; j++)
        {
            if (to_encrypt[j] != decrypted[j])
            {
                printf("Длина сжатого: %zu\n", compressed_len);
                printf("Длина разжатого: %zu\n", decompressed_len);
                printf("Было: ");
                for (int k = 0; k < TEST_LEN; k++)
                {
                    printf("%d", to_encrypt[k]);
                }
                printf("\nСжали: ");
                for (int k = 0; k < compressed_len; k++)
                {
                    printf("%d", compressed[k]);
                }
                printf("\nРазжали: ");
                for (int k = 0; k < TEST_LEN; k++)
                {
                    printf("%d", decrypted[k]);
                }
                free(to_encrypt);
                free(compressed);
                free(decrypted);
                printf("Тест №%i упаль...\n", i);
                return;
            }
        }
        memset(to_encrypt, 0, TEST_LEN * sizeof(char));
        memset(to_encrypt, 0, 8 * TEST_LEN * sizeof(char));
        memset(compressed, 0, TEST_LEN * sizeof(char));
        memset(compressed_buff, 0, TEST_LEN * sizeof(char));
        printf("Тест №%d успешен, сжали на %2.02f%%!\n", i, (1.0f - ((float)compressed_len / TEST_LEN)) * 100.0f);
    }
    free(to_encrypt);
    free(compressed);
    free(decrypted);
    free(compressed_buff);
}

