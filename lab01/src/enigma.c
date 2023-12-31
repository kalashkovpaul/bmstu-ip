#include <stdio.h>

#include "enigma.h"
#include "rotors.h"
#include "menu.h"

int find_letter_in_rotor(char letter, int rotor);
void rotate_first_rotor(void);
char letter_distance(char first, char second);

char cypher(char let)
{
    if (let == ' ') let = 'X';
    if (!is_letter(let)) return let;
    if (let > 'Z') let = let - ('a' - 'A');

    int letter = (int)let;

    letter = commutation[letter - 'A'];
    letter = ((letter - 'A') + (rotors_positions[0] - 'A')) % 26;
    letter = rotors[chosen_rotors[0]][letter] - 'A';
    letter = (letter + (letter_distance(rotors_positions[1], rotors_positions[0]))) % 26;
    letter = rotors[chosen_rotors[1]][letter] - 'A';
    letter = (letter + (letter_distance(rotors_positions[2], rotors_positions[1]))) % 26;
    letter = rotors[chosen_rotors[2]][letter] - 'A';
    letter = letter - (rotors_positions[2] - 'A');
    if (letter < 0) letter = 26 + letter;

    letter = reflectors[chosen_reflector][letter] - 'A';
    letter = (letter + (rotors_positions[2] - 'A')) % 26;
    letter = find_letter_in_rotor((letter + 'A'), 2);
    letter = (letter - (letter_distance(rotors_positions[2], rotors_positions[1])));
    if (letter < 0) letter = 26 + letter;
    letter = find_letter_in_rotor((letter + 'A'), 1);
    letter = (letter - (letter_distance(rotors_positions[1], rotors_positions[0])));
    if (letter < 0) letter = 26 + letter;
    letter = find_letter_in_rotor((letter + 'A'), 0);
    letter = letter - (rotors_positions[0] - 'A');
    if (letter < 0) letter = 26 + letter;
    letter = commutation[letter];

    rotate_first_rotor();
    return (char) letter;
}

char letter_distance(char first, char second)
{
    char tmp = first;
    if (first > second) {
        first = second;
        second = tmp;
    }
    char lr = second - first;
    char rl = first + 26 - second;
    return lr > rl ? rl : lr;
}

int find_letter_in_rotor(char letter, int rotor)
{
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        if (rotors[rotor][i] == letter)
            return i;
    }
    return -1;
}

void rotate_rotor(int rotor_number)
{
    rotors_positions[rotor_number] = rotors[chosen_rotors[rotor_number]][(find_letter_in_rotor(rotors_positions[rotor_number], rotor_number) + 1) % 26];
}

void rotate_first_rotor(void) {
    rotate_rotor(0);
    if (rotors_positions[0] == 'A') {
        rotate_rotor(1);
        if (rotors_positions[1] == 'A') {
            rotate_rotor(2);
        }
    }
}
