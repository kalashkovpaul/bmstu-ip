CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic -Wfloat-conversion -Wvla -Wfloat-equal -Wpedantic -Wextra -Iinc -g3
.PHONY = clean

app.exe: out/main.o out/cfb.o out/des.o out/menu.o
	$(CC) -o $@ $^

out/main.o: src/main.c inc/menu.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/cfb.o: src/cfb.c inc/cfb.h inc/des.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/des.o: src/des.c inc/des.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/menu.o: src/menu.c inc/menu.h inc/cfb.h
	$(CC) $(CFLAGS) -o $@ -c $<

# out/enigma.o: src/enigma.c inc/menu.h inc/rotors.h inc/constants.h inc/enigma.h
# 	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm out/*.o