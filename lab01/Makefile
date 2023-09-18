CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic -Wfloat-conversion -Wvla -Wfloat-equal -Wpedantic -Wextra -Iinc -g3
.PHONY = clean

app.exe: out/main.o
	$(CC) -o $@ $^

out/main.o: src/main.c
	$(CC) $(CFLAGS) -o $@ -c $<

# out/my_snprintf.o: src/my_snprintf.c
# 	$(CC) $(CFLAGS) -o $@ -c $<

# unit_tests.exe: out/unit_main.o out/test_my_snprintf.o out/my_snprintf.o
# 	$(CC) -o $@ $^

# out/unit_main.o: unit_tests/unit_main.c unit_tests/*.h
# 	$(CC) $(CFLAGS) -o $@ -c $<

# out/test_my_snprintf.o: unit_tests/test_my_snprintf.c inc/my_snprintf.h
# 	$(CC) $(CFLAGS) -o $@ -c $<

# unit: unit_tests.exe
# 	./unit_tests.exe
#	valgrind --tool=memcheck --leak-check=full ./unit_tests.exe

clean:
	rm out/*.o