CC = gcc -std=c99
CFLAGS = -g -Wall
TARGET = smallsh

output: main.o util.o smallsh.o
	$(CC) $(CFLAGS) main.o util.o smallsh.o -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

smallsh.o: smallsh.c smallsh.h
	$(CC) $(CFLAGS) -c smallsh.c

clean:
	rm -f *.o $(TARGET)

run:
	./$(TARGET)

check:
	valgrind --leak-check=yes ./$(TARGET)