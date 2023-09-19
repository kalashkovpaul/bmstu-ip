#include <stdio.h>
#include "enigma.h"
#include "menu.h"
#include "rotors.h"

void printChoice(void);
void set_reflector(void);
void set_rotors(void);
void set_commutation(void);
void backup_commutation(void);
void set_rotors_states(void);
void show_configuration(void);
void print_commutation(void);
void cypher_message(void);

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
            cypher_message();
            break;

        case DECYPHER:
            printf("TODO\n");
            break;

        case SHOW_CONGIFURATION:
            show_configuration();
            break;

        case SET_ROTORS_STATES:
            set_rotors_states();
            break;

        case SET_ROTORS:
            set_rotors();
            break;

        case SET_REFLECTOR:
            set_reflector();
            break;

        case SET_COMMUTATORS:
            set_commutation();
            break;

        case BACKUP_COMMUTATORS:
            backup_commutation();
            break;

        case EXIT:
            printf("Спасибо, что пользовались Энигмой от Калашкова П. :)\n");
            break;

        default:
            printf("К сожалению, такого пункта меню нет. Попробуйте ещё раз!\n");
            break;
        }
    }
}

void printChoice(void) {
    printf("Выберите один из пунктов:\n");
    printf("1. Зашифровать сообщение\n");
    printf("2. Расшифровать сообщение\n");
    printf("3. Посмотреть текущую конфигурацию\n");
    printf("4. Установить состояние роторов\n");
    printf("5. Установить роторы\n");
    printf("6. Установить рефлектор\n");
    printf("7. Поменять коммутационную панель\n");
    printf("8. Сбросить соединения на коммутационной панели\n");
    printf("\n");
    printf("0. Выйти из меню\n");
    printf("\n");
    printf("Выберите пункт: ");
}

void cypher_message(void)
{
    printf("Введите сообщение в файл to_cypher.txt. После ввода нажмите Enter");
    getchar();
    FILE* input = fopen(INPUT_FILE, "r");
    if (!input)
    {
        printf("Что-то не так с файлов to_cypher.txt\n\n");
        return;
    }
    FILE* output = fopen(OUTPUT_FILE, "w");
    if (!output)
    {
        printf("Что-то не так с файлов to_decypher.txt\n\n");
        return;
    }
    int letter = 0;
    char result = 0;
    while((letter = fgetc(input)) != EOF)
    {
        result = cypher((char)letter);
        printf("%c to %c\n", letter, result);
        fprintf(output, "%c", result);
    }
    fclose(input);
    fclose(output);
}

void show_configuration(void)
{
    printf("\n");
    printf("Текущее состояние:\n");
    printf("Выбраны роторы %d, %d и %d\n", chosen_rotors[0] + 1, chosen_rotors[1] + 1, chosen_rotors[2] + 1);
    printf("Выбран рефлектор %d\n", chosen_reflector + 1);
    printf("Состояние роторов: %c, %c и %c\n", rotors_positions[0], rotors_positions[1], rotors_positions[2]);
    printf("Коммутационная панель:\n");
    print_commutation();
}

int is_letter(char letter)
{
    return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
}

void set_rotors_states(void)
{
    int check = 0;
    char first, second, third;
    int done = 0;
    while (!done) {
        printf("Введите состояние роторов (3 латинские буквы подряд): ");
        check = scanf("%c%c%c", &first, &second, &third);
        if (check != 3 || !is_letter(first) || !is_letter(second) || !is_letter(third))
        {
            printf("Три. Латинские. Буквы. Подряд. Попробуем ещё раз...\n");
            fflush(stdin);
            continue;
        }
        if (first > 'Z')
            first = first - ('a' - 'A');
        if (second > 'Z')
            second = second - ('a' - 'A');
        if (third > 'Z')
            third = third - ('a' - 'A');
        rotors_positions[0] = first;
        rotors_positions[1] = second;
        rotors_positions[2] = third;
        printf("Позиции роторов установлены: %c, %c и %c.\n", first, second, third);
        done = 1;
    }
}

void set_rotors(void)
{
    int rotor = -1;
    int check = 0;
    int chosen = 0;
    int available_rotors[ROTORS_AMOUNT] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    while (check != 1 || chosen != CHOSEN_ROTORS)
    {
        printf("Нужно выбрать 3 ротора. Вы выбрали %d.\n", chosen);
        printf("Выберите один из роторов. Доступные роторы: ");
        for (int i = 0; i < ROTORS_AMOUNT; i++)
        {
            if (available_rotors[i])
                printf("%d ", i + 1);
        }
        printf("\n");
        printf("Ваш выбор: ");
        check = scanf("%d", &rotor);
        if (check != 1)
            printf("Это не номер ротора :( \n");
        else if (rotor < 1 || rotor > 10)
            printf("Такого ротора просто не существует\n");
        else if (!available_rotors[rotor - 1])
            printf("Этот ротор уже занят, выберите другой.\n");
        else
        {
            available_rotors[rotor - 1] = 0;
            chosen_rotors[chosen] = rotor - 1;
            chosen++;
        }
        fflush(stdin);
    }
    printf("Успех! Выбраны роторы %d, %d и %d\n", chosen_rotors[0] + 1, chosen_rotors[1] + 1, chosen_rotors[2] + 1);
}

void set_reflector(void)
{
    int reflector = -1;
    int check = 0;
    while (check != 1 || (reflector < 1 || reflector > 2))
    {
        printf("Выберите рефлектор: 1 или 2\n");
        printf("Ваш выбор: ");
        check = scanf("%d", &reflector);
        if (check != 1)
            printf("Это не рефлектор :( \n");
        else if (reflector < 1 || reflector > 2)
            printf("1 или 2...\n");
        else
            chosen_reflector = reflector;
        fflush(stdin);
    }
    printf("Успех! Выбран рефлектор %d\n", chosen_reflector);
}

void print_commutation(void)
{
    printf("Текущее соединение:\n");
    for (int i = 0; i < ALPHABET_LENGTH; i++)
        printf("%c ", alphabet[i]);
    printf("\n");
    for (int i = 0; i < ALPHABET_LENGTH; i++)
    {
        if (commutation_state[i])
            printf("%c ", commutation[i]);
        else
            printf("- ");
    }
    printf("\n");
}

int get_connected_commutation(void)
{
    int result = 0;
    for (int i = 0; i < ALPHABET_LENGTH; i++)
        if (commutation_state[i])
            result++;
    return result / 2;
}

void set_commutation(void)
{
    int check = 0;
    int connected = get_connected_commutation();
    int done = 0;
    char first = 0;
    char second = 0;

    while ((check != 1 || connected < 10) && !done)
    {
        print_commutation();
        printf("Если вы закончили, введите 0. Свободно %d соединений. Какую букву хотите соединить?\n", COMMUTATION_AMOUNT - connected);
        printf("Какую букву соединяем: ");
        check = scanf("%c", &first);
        fflush(stdin);
        if (check != 1 || !(first == '0' || (is_letter(first))))
        {
            printf("Нужна буква латинского алфавита. Попробуем ещё раз...%c\n\n", first);
            continue;
        }
        if (first == '0')
        {
            done = 1;
            continue;
        }
        if (first > 'Z')
            first = first - ('a' - 'A');
        printf("С какой буквой соединить букву %c: ", first);
        fflush(stdin);
        check = scanf("%c", &second);
        if (check != 1 || !(is_letter(second)))
        {
            printf("Нужна буква латинского алфавита. Попробуем ещё раз...\n\n");
            continue;
        }
        if (second > 'Z')
            second = second - ('a' - 'A');
        if (first == second)
        {
            commutation_state['Z' - first] = 0;
            commutation['Z' - first] = first;
            printf("Связь освобождена!\n");
        } else
        {
            if (commutation_state[first - 'A'])
            {
                commutation_state[commutation[first - 'A'] - 'A'] = 0;
                printf("Освобождена связь %c <-> %c\n", first, commutation[first - 'A']);
                connected--;
            }
            if (commutation_state[second - 'A'])
            {
                commutation_state[commutation[second - 'A'] - 'A'] = 0;
                printf("Освобождена связь %c <-> %c\n", second, commutation[second - 'A']);
                connected--;
            }
            commutation[first - 'A'] = second;
            commutation[second - 'A'] = first;
            commutation_state[first - 'A'] = 1;
            commutation_state[second - 'A'] = 1;
            connected++;
            printf("Соединили %c <-> %c.\n", first, second);
        }
    }
    printf("Успех! Коммунационная панель настроена. Её состояние: \n");
    print_commutation();
}

void backup_commutation(void)
{
    for (char letter = 'A'; letter <= 'Z'; letter++)
    {
        commutation[letter - 'A'] = letter;
        commutation_state[letter - 'A'] = 0;
    }
    printf("Коммункационная панель сброшена. Её состояние:\n");
    print_commutation();
}
