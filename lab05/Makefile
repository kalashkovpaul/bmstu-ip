CC = gcc
CFLAGS = -g -std=c11 -Wall -Werror -pedantic -Wfloat-conversion -Wfloat-equal -Wpedantic -Wextra -Iinc -g3
.PHONY = clean

app.exe: out/main.o out/menu.o out/lzw.o
	$(CC) -o $@ $^

out/main.o: src/main.c inc/menu.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/menu.o: src/menu.c inc/menu.h inc/lzw.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/lzw.o: src/lzw.c inc/lzw.h
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm out/*.o