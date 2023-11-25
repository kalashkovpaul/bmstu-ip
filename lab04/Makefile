CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic -Wfloat-conversion -Wvla -Wfloat-equal -Wpedantic -Wextra -Iinc -g3
.PHONY = clean

app.exe: out/main.o out/rsa.o out/menu.o out/md5.o
	$(CC) -o $@ $^ -lgmp

out/main.o: src/main.c inc/menu.h inc/rsa.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/rsa.o: src/rsa.c inc/rsa.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/menu.o: src/menu.c inc/menu.h inc/rsa.h inc/md5.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/md5.o: src/md5.c
	$(CC) $(CFLAGS) -o $@ -c $<

# out/enigma.o: src/enigma.c inc/menu.h inc/rotors.h inc/constants.h inc/enigma.h
# 	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm out/*.o