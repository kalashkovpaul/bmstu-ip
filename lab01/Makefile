CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic -Wfloat-conversion -Wvla -Wfloat-equal -Wpedantic -Wextra -Iinc -g3
.PHONY = clean

app.exe: out/main.o out/menu.o out/enigma.o out/rotors.o
	$(CC) -o $@ $^

out/main.o: src/main.c inc/menu.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/rotors.o: src/rotors.c inc/rotors.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/menu.o: src/menu.c inc/menu.h inc/constants.h inc/enigma.h inc/rotors.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/enigma.o: src/enigma.c inc/menu.h inc/rotors.h inc/constants.h inc/enigma.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm out/*.o