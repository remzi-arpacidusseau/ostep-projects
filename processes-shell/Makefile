CC=gcc
CFLAGS=-I. -g -O0
DEPS = executor.h parser.h tokenizer.h types.h utils.h
OBJ = executor.o parser.o tokenizer.o utils.o wish.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

wish: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
