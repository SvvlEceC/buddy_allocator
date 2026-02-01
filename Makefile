CC = gcc
CFLAGS = -Iinclude -Wall -std=c11
BIN_DIR = bin
SRC = src/buddy.c

test%: tests/test%.c $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@
	./$(BIN_DIR)/$@

clean: 
	rm -rf $(BIN_DIR)