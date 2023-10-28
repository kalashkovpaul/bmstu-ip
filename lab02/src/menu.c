#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "menu.h"
#include "cfb.h"

void printChoice(void);
void show_configuration(void);
void cypher_file(void);
void decypher_file(void);
void set_current_key(void);
void set_current_IV(void);
void test_cfb(void);

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

        case SHOW_CONFIGURATION:
            show_configuration();
            break;

        case SET_KEY:
            set_current_key();
            break;

        case SET_IV:
            set_current_IV();
            break;

        case TEST:
            test_cfb();
            break;

        case EXIT:
            printf("Спасибо, что пользовались CFB (DES) от Калашкова П. :)\n");
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
    printf("3. Показать конфигурацию файла\n");
    printf("4. Установить значение ключа\n");
    printf("5. Установить значение начального вектора\n");
    printf("6. Протестировать работу системы\n");
    printf("\n");
    printf("0. Выйти из меню\n");
    printf("\n");
    printf("Выберите пункт: ");
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
    uint64_t block = 0;
    uint64_t result = 0;
    ssize_t amount;
    while((amount = fread(&block, sizeof(char), sizeof(block) / sizeof(char), input)) > 0)
    {
        result = cfb(block, 'e');
        printf ("E: %016llx\n", result);
        fwrite(&result, sizeof(result), 1, output);
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
    uint64_t block = 0;
    uint64_t result = 0;
    ssize_t amount;
    while((amount = fread(&block, sizeof(block), 1, input)) > 0)
    {
        result = cfb(block, 'd');
        printf ("D: %016llx\n", result);
        fwrite(&result, sizeof(char), sizeof(block) / sizeof(char), output);
    }
    fclose(input);
    fclose(output);
}

void show_configuration(void)
{
    printf("\n");
    printf("Текущее состояние:\n");
    printf("Ключ равен %016llx\n", get_key());
    printf("Вектор инициализации равен %016llx\n", getIV());
}

int is_letter(char letter)
{
    return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
}

void set_current_key(void)
{
    uint64_t new_key = 0;
    printf("Введите новый ключ:\n");
    while ((scanf("%llx" SCNd64, &new_key)) != 1)
    {
        fflush(stdin);
        printf("Введите нормальный ключ: 16-ная СС, 16 символов максимум:\n");
    }
    set_key(new_key);
    printf("Ключ установлен: %016llx\n\n", get_key());
}

void set_current_IV(void)
{
    uint64_t new_IV = 0;
    printf("Введите новый вектор инициализации:\n");
    while ((scanf("%llx" SCNd64, &new_IV)) != 1)
    {
        fflush(stdin);
        printf("Введите нормальный вектор инициализации: 16-ная СС, 16 символов максимум:\n");
    }
    setIV(new_IV);
    printf("Вектор инициализации установлен %016llx\n", getIV());
}

uint64_t rand_uint64(void) {
  uint64_t r = 0;
  for (int i=0; i<64; i++) {
    r = r*2 + rand()%2;
  }
  return r;
}

void test_cfb(void)
{
    uint64_t values_to_cypher[TEST_TIMES] = {0};
    uint64_t values_to_decypher[TEST_TIMES] = {0};
    uint64_t values_decyphered[TEST_TIMES] = {0};
    for (int i = 1; i <= TEST_TIMES; i++)
    {
        printf("Тест для длины %d...", i);
        uint64_t IV = rand_uint64();
        uint64_t key = rand_uint64();
        setIV(IV);
        set_key(key);
        for (int j = 0; j < i; j++) {
            values_to_cypher[j] = rand_uint64();
            values_to_decypher[j] = cfb(values_to_cypher[j], 'e');
        }
        setIV(IV);
        set_key(key);
        for (int j = 0; j < i; j++) {
            values_decyphered[j] = cfb(values_to_decypher[j], 'd');
        }
        for (int j = 0; j < i; j++) {
            if (values_decyphered[j] != values_to_cypher[j])
            {
                printf("ОШИБКА: пытались зашифровать:\n");
                for (int k = 0; k < i; k++)
                {
                    printf("%016llx", values_to_cypher[k]);
                }
                printf("\nИ зашифровали:\n");
                for (int k = 0; k < i; k++)
                {
                    printf("%016llx", values_to_decypher[k]);
                }
                printf("\nНо, расшифровав, получили: \n");
                for (int k = 0; k < i; k++)
                {
                    printf("%016llx", values_decyphered[k]);
                }
                printf("\n");
                return;
            }
        }
        printf("... успешен\n");
    }
}
