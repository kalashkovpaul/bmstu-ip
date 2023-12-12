CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic -Wfloat-conversion -Wvla -Wfloat-equal -Wpedantic -Wextra -Iinc -g3
.PHONY = clean

app.exe: out/main.o out/pcbc.o out/aes.o out/menu.o
	$(CC) -o $@ $^

out/main.o: src/main.c inc/menu.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/pcbc.o: src/pcbc.c inc/pcbc.h inc/aes.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/aes.o: src/aes.c inc/aes.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/menu.o: src/menu.c inc/menu.h inc/pcbc.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm out/*.o