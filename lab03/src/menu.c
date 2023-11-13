#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "menu.h"
#include "pcbc.h"

void printChoice(void);
void show_configuration(void);
void cypher_file(void);
void decypher_file(void);
void set_current_key(void);
void set_current_IV(void);
void test_pcbc(void);

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
        case CYPHER:
            cypher_file();
            break;

        case DECYPHER:
            decypher_file();
            break;

        case SET_IV:
            set_current_IV();
            break;

        case TEST:
            test_pcbc();
            break;

        case EXIT:
            printf("Спасибо, что пользовались PCBC (AES) от Калашкова П. :)\n");
            break;

        default:
            printf("К сожалению, такого пункта меню нет. Попробуйте ещё раз!\n");
            break;
        }
    }
}

void printChoice(void) {
    printf("Выберите один из пунктов:\n");
    printf("1. Зашифровать файл\n");
    printf("2. Расшифровать файл\n");
    printf("3. Сбросить значение начального вектора\n");
    printf("4. Протестировать работу системы\n");
    printf("\n");
    printf("0. Выйти из меню\n");
    printf("\n");
    printf("Выберите пункт: ");
}

// Pretty-print a key (or any smallish buffer) onto screen as hex
void Pretty(const byte* b,int len,char* label) {
    printf("%s", label);
    int i;
    for (i=0; i<len; i++) {
        printf("%02x", b[i]);
    }
    printf("\n");
}

void cypher_file(void)
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

    byte to_cypher[16] = {0};
    byte cyphered[16] = {0};
    size_t amount = 0;

    while((amount = fread(to_cypher, sizeof(byte), 16, input)) > 0)
    {
        Pretty(to_cypher, 16, "to_cypher: ");
        pcbc(to_cypher, cyphered, 'e');
        Pretty(cyphered, 16, "cyphered: ");
        fwrite(cyphered, sizeof(byte), 16, output);
    }

    fclose(input);
    fclose(output);
}

void decypher_file(void)
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

    byte to_decypher[16] = {0};
    byte decyphered[16] = {0};
    size_t amount = 0;

    while((amount = fread(to_decypher, sizeof(byte), 16, input)) > 0)
    {
        Pretty(to_decypher, 16, "to_decypher");
        pcbc(to_decypher, decyphered, 'd');
        Pretty(decyphered, 16, "decyphered");
        fwrite(decyphered, sizeof(byte), 16, output);
    }

    fclose(input);
    fclose(output);
}

int is_letter(char letter)
{
    return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
}

void set_current_IV(void)
{
    setIV();
    printf("Вектор инициализации сброшен!\n");
}

uint64_t rand_uint64(void) {
  uint64_t r = 0;
  for (int i=0; i<64; i++) {
    r = r*2 + rand()%2;
  }
  return r;
}

void test_pcbc(void)
{
    byte values_to_cypher[TEST_TIMES][16];
    byte values_to_decypher[TEST_TIMES][16] = {0};
    byte values_decyphered[TEST_TIMES][16] = {0};
    for (int i = 1; i <= TEST_TIMES; i++)
    {
        printf("Тест №%d...", i);
        setIV();
        for (int j = 0; j < 16; j++) {
            values_to_cypher[i][j] = (byte) rand_uint64();
        }
        pcbc(values_to_cypher[i], values_to_decypher[i], 'e');
        setIV();
        pcbc(values_to_decypher[i], values_decyphered[i], 'd');
        for (int j = 0; j < 16; j++) {
            if (values_decyphered[i][j] != values_to_cypher[i][j])
            {
                printf("ОШИБКА\n");
                return;
            }
        }
        printf("... успешен\n");
    }
}
