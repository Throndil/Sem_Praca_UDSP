CC=gcc
CFLAGS=-I.
LDFLAGS=-lm
TARGET=simple_database
OBJ=main.o server.o client.o manager.o table.o
DEPS=server.h client.h manager.h table.h

all:$(TARGET)

#kompilacia o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	echo "spravil kompilaciu"
#linkovanie
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	echo "spravil linkovanie"

#clean
clean:
	rm -rf $(TARGET)
	rm -rf *.o
	echo "spravil clean"
