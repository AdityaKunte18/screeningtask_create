CC=gcc
CFLAGS=-Iinclude -Wall -g
DEPS = include/stream_processor.h include/tokenizer.h
SRCS = src/main.c src/stream_processor.c src/tokenizer.c
OBJ = $(SRCS:.c=.o)
EXECUTABLE=find_freq_items

# Default target
all: $(EXECUTABLE)

# Rule to create the executable
$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Generic rule for building objects
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXECUTABLE)

# Phony targets
.PHONY: all clean